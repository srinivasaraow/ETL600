/*-------------------------------------------------------------------------*
 * $Workfile: voiceData.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Sending and receiving of voice data subframes on external
 *                TDM bus.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/C/voiceData.c $
 * 
 * 17    12-04-18 10:50 Chrobif
 * cosmetics (comments modified)
 * 
 * 16    12-04-18 10:35 Chrobif
 * cosmetics (replaced tabs with spaces)
 * 
 * 15    11-08-24 16:45 Chhamaa
 * Sync failure handling in case of connection down modified.
 * 
 * 14    11-08-22 10:44 Chhamaa
 * voiceData_initRx(): SyncFailure warning only if channel is ON by
 * configuration.
 * 
 * 13    11-05-10 17:41 Chhamaa
 * voiceData_initRx(): status_clearWarning() for AIS added.
 * 
 * 12    11-05-04 13:37 Chhamaa
 * Individual channel sync added for transit operation in R4.
 * 
 * 11    23.06.10 14:52 Chhamaa
 * AIS detection added.
 * 
 * 9     22.09.08 14:33 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 8     08-06-20 14:53 Chrobif
 * Debugging of Problem 447 showed that voiceData_receive() is not robust
 * to corrupt data rate indices in basic time slot. This modification
 * improves this aspect.
 * 
 * 7     08-01-17 8:36 Chrobif
 * Some comments added.
 * 
 * 6     07-09-28 11:27 Chrobif
 * + Implementation of appAction unpack/interpolate/hold.
 * + Implementation of statistics gathering and bit error insertion (test
 * infrastructure)
 * + Implementation of receiver restart if MOD600 is out of sync or last
 *  received subframe is older than 15ms
 * 
 * 5     07-07-20 15:51 Chrobif
 * + Assembler function used to calculate Hamming Distance
 * + Criterion for forwarding decision changed from inSync to
 * frameAvailable
 * + inSync and frameAvailable members in subframe history removed and
 * ready member added.
 * + g_receiverInSync does now become false if MOD600 is down
 * + g_decoderDataReady is now reset in voiceData_getRxFrames()
 * + inSync member in t_RxFrameForDecoder replaced by ready member
 * 
 * 4     07-07-03 17:10 Chrobif
 * Some compiler switches for testing modified.
 * 
 * 3     2.07.07 10:19 Chhamaa
 * voiceData_getRxFrames() modified.
 * 
 * 2     07-06-22 13:22 Chrobif
 * Subframe synchronization and channel synchronisation added in
 * voiceData_receive().
 * 
 * 1     2.05.07 10:21 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* COMPILER SWITCHES *********************************************************/
// #define VOICE_DATA_STATS_RX  // If VOICE_DATA_STATS_RX is defined statistics
                                // about the required appAction in the delivered
                                // frames are gathered.

// #define VOICE_DATA_INTRODUCE_BIT_ERRORS  // If VOICE_DATA_INTRODUCE_BIT_ERRORS
                                            // is defined, bit errors are introduced
                                            // in shiftInSubFrameHeader() which cause
                                            // the receiver to require interpolate
                                            // or hold actions.
                                            
//#define VOICE_DATA_INSERT_AIS // If VOICE_DATA_INSERT_AIS is defined all ones
                                // are inserted in the header bits of one channel

//#define VOICE_DATA_VERSION_1 // ETL600 release 3.x
#define VOICE_DATA_VERSION_2 // ETL600 release 4

/* INCLUDE FILES ***********************************************************/

// C Run-Time Library
#include <assert.h>
#include <ccblkfn.h>
#include <cdefBF538.h>

// O4CV firmware
#include "BASETYPE.H"
#include "O4CV_Const.h"
#include "voiceData.h"
#include "configuration.h"
#include "subframe.h"
#include "extTdmBus.h"
#include "status.h"

// Testing
#ifdef TEST_VOICE_DATA
    #include "testVoiceData.h"
#endif



/* LOCAL CONSTANT DECLARATIONS *********************************************/
#define N_SUBFRAMES_PER_FRAME       3   // number of subframes per voice data frame
#define N_SUBFRAMES_IN_RX_BUFFER    9   // size of rx subframe buffer [subframes]
#define N_SUBFRAMES_IN_RX_HISTORY   3   // size of rx subframe history [subframes]
#define SUBFRAME_START_BIT      BIT11   // flag in basic time slot
#define DISTRIBUTED_9BIT_HEADER_VOICE   ( ((UINT16)SUBFRAME_HEADER_VOICE_1  << 1) | \
                                          ((UINT16)SUBFRAME_HEADER_VOICE_2  >> 2) | \
                                          ((UINT16)SUBFRAME_HEADER_VOICE_3  >> 5) )
#define DISTRIBUTED_9BIT_HEADER_FAX     ( ((UINT16)SUBFRAME_HEADER_FAX_1    << 1) | \
                                          ((UINT16)SUBFRAME_HEADER_FAX_2    >> 2) | \
                                          ((UINT16)SUBFRAME_HEADER_FAX_3    >> 5) )
#define DISTRIBUTED_9BIT_HEADER_SIG     ( ((UINT16)SUBFRAME_HEADER_SIGNAL_1 << 1) | \
                                          ((UINT16)SUBFRAME_HEADER_SIGNAL_2 >> 2) | \
                                          ((UINT16)SUBFRAME_HEADER_SIGNAL_3 >> 5) )
#define SUBFRAME_RX_TIMEOUT     160 // 15ms
#define NO_START_FLAG_TIMEOUT   693 // 65ms (= 6 subframes, 2 voice frames)
#define LOST_SYNC_THRESHOLD     4   // This threshold defines how many frames can be corrupted
                                    // until the receiver falls out of sync     
#define FRAME_TYPE_VOICE        1 
#define CONNECTION_GO_DOWN_CONDITION_3_THRESHOLD 5

#define NINE_BIT_HEADER_MASK    0x01FF

// Thresholds for AIS detection
#define AIS_ON_TH   6   // number of subframes [10ms]
#define AIS_OFF_TH  6   // number of subframes [10ms]

/* LOCAL TYPE DECLARATIONS *************************************************/
typedef enum {

    e_WaitingTx,
    e_SendingStartFlag,
    e_SendingData,

} e_TxSubframeStates;


typedef struct {

    bool enabled;
    e_TxSubframeStates state;
    INT16 waitCounter;
    INT16 dataIx;
    INT16 subframeIx;
    bool bufferTx0Active;
    INT16 numOfSubframesSent;
    
    // pointers to channel data
    UINT8 *pDataCh[N_CHANNELS_ON_BOARD];

} t_SubframeSender;


typedef enum {

    e_WaitingRx,
    e_Receiving,

} e_RxSubframeStates;

typedef enum {
    
    e_Idle,
    e_Sync1,
    e_Sync2,
    e_Sync3,
    
} e_RxSubframeSyncStates;

typedef enum {
    
    e_OutOfSync,
    e_InSync1,
    e_InSync2,
    e_InSync3,
    
} e_ChannelSyncStates;

typedef struct {

    e_RxSubframeSyncStates state;
    UINT16 nineBitHeader;
    UINT16 hMin;
    UINT16 iMin;
    
} t_RxSubframeSync;

#ifdef VOICE_DATA_VERSION_2
typedef enum {
    
    e_ChannelDown,
    e_ChannelDeliversFrames,
    e_ChannelInterpolatesOrHolds,
    
} e_ChannelStates;
#endif

typedef struct {
    
    e_ChannelSyncStates state;
    bool currentActiveChannels[N_CHANNELS_ON_BOARD];
    UINT16 lostSyncCounter[N_CHANNELS_ON_BOARD];
#ifdef VOICE_DATA_VERSION_2
    UINT16 indexSubframeHistory[N_CHANNELS_ON_BOARD];   // index of the subframe history element describing the received frame
    UINT16 iMin[N_CHANNELS_ON_BOARD];                   // Frame type
    e_ChannelStates channelStates[N_CHANNELS_ON_BOARD]; // state of the channel up/down state machine
    e_Action appAction[N_CHANNELS_ON_BOARD];            // action to be taken by the application
#endif
    bool framesReady;
    bool receiverInSync;
    
} t_RxChannelSync;

typedef enum {
    
    e_AisWait,
    e_AisOnDelay,
    e_AisDetected,
    e_AisOffDelay
    
} e_AisDetectorStates;
    
typedef struct {
    
    bool detected;
    UINT16 delayCnt;
    UINT32 detCnt;
    e_AisDetectorStates state;
    
} t_AIS;

typedef struct t_RxSFHistoryElement {

    UINT8 index; // Index within the subframe history array. It is set in voiceData_initRx().
    bool ready;  // The ready flag is true if this subframe is the last
                 // subframe of a correctly received frame and false otherwise. It is set in subframeSync().
    e_Action appAction; // Action to be taken by the application
    UINT16 iMin; // iMin designates the detected frame type. It is set in subframeSync().
    t_Subframe *pSubframes[N_SUBFRAMES_PER_FRAME]; // pSubframes is an array of pointers which point to the three
                                                   // most recently received subframes. pSubframes[2] points to the
                                                   // most recently received subframe. The pointers are set in
                                                   // voiceData_receive().
    struct t_RxSFHistoryElement *pNext;     // pNext points to the next subframe history element.
                                            // It is set in voiceData_initRx().
    struct t_RxSFHistoryElement *pPrevious; // pPrevious points to the last subframe history element.
                                            // It is set in voiceData_initRx().
    
} t_RxSFHistoryElement;

typedef struct {

    // Subframe reception
    e_RxSubframeStates state;
    INT16 dataIx;
    INT16 subframeIx;
    UINT8 *pDataCh[N_CHANNELS_ON_BOARD];
    
    // Subframe synchronisation 
    t_RxSubframeSync subframeSync[N_CHANNELS_ON_BOARD];
    
    // Subframe history
    t_RxSFHistoryElement* pCurrentSFInHistory[N_CHANNELS_ON_BOARD];
    t_RxSFHistoryElement  subframeHistory[N_CHANNELS_ON_BOARD][N_SUBFRAMES_IN_RX_HISTORY];
    UINT16 subframeDisplacement[N_CHANNELS_ON_BOARD];

    // Channel synchronisation
    t_RxChannelSync channelSync;
    
    // Frames for decoder
    t_RxFrameForDecoder frameForDecoder[N_CHANNELS_ON_BOARD];

    // Connection state
    bool connectionIsUp;

    // Subframe receive timer
    UINT16 timeSinceLastRxSf;
    UINT16 timeSinceLastRxSfCDown;  // used if no voice connection is active
    
    // MOD600 data rate index monitoring
    UINT16 activeDRIndex;
    UINT16 lastDRIndex;
    UINT16 numOfConsecutiveDRIndices;
    
    // AIS detector
    t_AIS ais[N_CHANNELS_ON_BOARD];
    
} t_SubframeReceiver;

/* GLOBAL VARIABLE DECLARATIONS ********************************************/

t_Subframe *g_pSubframeBufferTxCh[N_CHANNELS_ON_BOARD];

bool g_decoderDataReady; // Utilization:
                         // g_decoderDataReady is set as soon as new frames are ready for
                         // decoding. If g_decoderDataReady is set voiceData_getRxFrames()
                         // can be called to retrieve access to the frames. A call of
                         // voiceData_getRxFrames() also clears the g_decoderDataReady flag.
                         // Implementation:
                         // g_decoderDataReady is set in voiceData_receive() if new frames
                         // are ready for decoding. It is cleared in voiceData_getRxFrames()
                         // and in voiceData_receive() if the MOD600 is down.
bool g_receiverInSync;   // Utilization:
                         // If the receiver is in sync new frames become available
                         // for decoding every 30ms. If it is out of sync no frames
                         // become available until the receiver gets in sync again.
                         // g_receiverInSync is true if the receiver is in sync.
                         // Implementation:
                         // g_receiverInSync is set or cleared in voiceData_receive()
                         // if new subframes have been received. Furthermore it is
                         // cleared in voiceData_receive() if the MOD600 is down

/* LOCAL VARIABLE DECLARATIONS *********************************************/

static t_Subframe g_subframeBufferTx0[N_CHANNELS_ON_BOARD][N_SUBFRAMES_PER_FRAME];
static t_Subframe g_subframeBufferTx1[N_CHANNELS_ON_BOARD][N_SUBFRAMES_PER_FRAME];
static t_Subframe g_subframeBufferRx[N_CHANNELS_ON_BOARD][N_SUBFRAMES_IN_RX_BUFFER];

static t_SubframeSender   g_sender;
static t_SubframeReceiver g_receiver;

// tx wait time between subframes in external TDM bus cycles [1/10666.7]
// 98 + 97 + 98 + 3*9 = 320 (= 30 ms), 9: length of subframe
static const INT16 g_txWaitTime[N_SUBFRAMES_PER_FRAME] = {98, 97, 98};

#ifdef VOICE_DATA_STATS_RX
    static t_VoiceDataStatsRx gs_voiceDataStatsRx;
#endif

#ifdef VOICE_DATA_INTRODUCE_BIT_ERRORS
    static UINT16 gs_bitErrorCtr;
#endif

#ifdef VOICE_DATA_INSERT_AIS
    static bool gs_insertAis = false;
    static INT16 gs_aisCounter = 0;
    static const INT16 gs_aisCh = CH4;
    static const INT16 gs_aisCycle = 3000;  // number of subframes (10 ms)
#endif

/* LOCAL MACRO DEFINITIONS *************************************************/

#ifdef VOICE_DATA_VERSION_2
#define IS_CHANNEL_UP(channelState) (((channelState) == e_ChannelDeliversFrames) || ((channelState) == e_ChannelInterpolatesOrHolds))
#endif

/* LOCAL FUNCTION DEFINITIONS **********************************************/

void swapSubframeBufferTx(void);
void shiftInSubFrameHeader(UINT16 *pRxTimeSlot);
void subframeSync(void);
void calcHMin(void);
#ifdef VOICE_DATA_VERSION_1
void channelSync(UINT16 *pRxTimeSlot);
bool findChannelSync(void);
bool checkChannelSync(void);
bool findReadySubframe(UINT16 channel);
#else
void receiverSync(UINT16 *pRxTimeSlot);
bool isAtLeastOneChannelUp(void);
bool isFrameFoundInChannel(UINT16 channel);
#endif
inline bool isValidDRIndex(UINT16 mod600DRIndex);
void detectAIS(void);
UINT16 getAISWarnBitMask(INT16 channel);
UINT16 getSyncFailureWarnBitMask(INT16 channel);

// for testing
void fillSubframeBufferTx(void);


/* IMPLEMENTATION **********************************************************/

    
/***************************************************************************
  Function:     void voiceData_initTx(void)
  Description:  Initializes subframe sender.
  Inputs:       None
  Outputs:      None
  Return:       None
  Globals:      g_sender, g_pSubframeBufferTxCh, g_subframeBufferTx0        
  Important:    -
****************************************************************************/
void voiceData_initTx(void)
{
    INT16 i;
    
    // init sender data
    g_sender.enabled = false;
    g_sender.state = e_SendingStartFlag;
    g_sender.waitCounter = 0;
    g_sender.dataIx = 0;
    g_sender.subframeIx = 0;
    g_sender.bufferTx0Active = false;
    
    // init pointers
    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    {
        g_sender.pDataCh[i] = (UINT8*)&g_subframeBufferTx1[i][0];
        g_pSubframeBufferTxCh[i] = &g_subframeBufferTx0[i][0];
    }
    
    #ifdef TEST_VOICE_DATA_TX
        testVoiceData_generateTestData();
    #endif
    
}


/***************************************************************************
  Function:     void voiceData_startTx(void)
  Description:  Starts the subframe sender.
  Inputs:       None
  Outputs:      None
  Return:       None
  Globals:      g_sender        
  Important:    The function voiceData_initTx() has to be called before
                voiceData_startTx() to correctly initialize the sender. 
****************************************************************************/
void voiceData_startTx(void)
{
    g_sender.enabled = true;
}
        

/***************************************************************************
  Function:     void voiceData_send(UINT16 *pTxTimeSlot)
  Description:  Transmits voice data subframes to P4LW on the external TDM Bus
  Inputs:       None
  Outputs:      pTxTimeSlot: pointer to first tx time slot
  Return:       None
  Globals:      g_sender    
  Important:    Call voiceData_initTx() and voiceData_startTx() to initialize
                and start the sending process.
****************************************************************************/
void voiceData_send(UINT16 *pTxTimeSlot)
{
    
    // init time slots
    *pTxTimeSlot &= ~SUBFRAME_START_BIT;    // basic time slot (for testing 0x3002)
    *(pTxTimeSlot + 1) = 0;                 // extended time slot
    *(pTxTimeSlot + 2) = 0;                 // extended time slot
    
    if (g_sender.enabled)
    {
        switch(g_sender.state)
        {
            case e_WaitingTx:
                                
                // send no data
                g_sender.waitCounter--;

                if (g_sender.waitCounter <= 0)
                {
                    g_sender.state = e_SendingStartFlag;
                }

                break;

            case e_SendingStartFlag:
            
                // swap buffer before sending first subframe
                if (g_sender.subframeIx == 0)
                {
                    swapSubframeBufferTx();
                }
                
                // send start flag
                *pTxTimeSlot |= SUBFRAME_START_BIT; // for testing 0xB802
                
                // send data
                *(pTxTimeSlot + 1)  = *g_sender.pDataCh[CH1]++;
                *(pTxTimeSlot + 1) |= *g_sender.pDataCh[CH2]++ << SB8;
                *(pTxTimeSlot + 2)  = *g_sender.pDataCh[CH3]++;
                *(pTxTimeSlot + 2) |= *g_sender.pDataCh[CH4]++ << SB8;
                
                g_sender.dataIx++;
                g_sender.state = e_SendingData;
                
                break;

            case e_SendingData:

                // send data
                *(pTxTimeSlot + 1)  = *g_sender.pDataCh[CH1]++;
                *(pTxTimeSlot + 1) |= *g_sender.pDataCh[CH2]++ << SB8;
                *(pTxTimeSlot + 2)  = *g_sender.pDataCh[CH3]++;
                *(pTxTimeSlot + 2) |= *g_sender.pDataCh[CH4]++ << SB8;
                
                g_sender.dataIx++;

                if (g_sender.dataIx >= SUBFRAME_LEN)
                {
                    g_sender.dataIx = 0;
                    g_sender.waitCounter = g_txWaitTime[g_sender.subframeIx];
                    g_sender.state = e_WaitingTx;

                    //g_sender.numOfSubframesSent++;    // for testing
                    g_sender.subframeIx++;
                    
                    if (g_sender.subframeIx >= N_SUBFRAMES_PER_FRAME)
                    {
                        g_sender.subframeIx = 0;
                    }
                }

                break;
        }
        
    }
    else
    {
        // do nothing
    } 
    
}


/***************************************************************************
  Function:     void swapSubframeBufferTx(void)     
  Description:  Swaps the subframe buffer pointers.
  Inputs:       None
  Outputs:      None
  Return:       None
  Globals:      g_sender, g_pSubframeBufferTxCh, g_subframeBufferTx0
                g_subframeBufferTx1 
  Important:    -
****************************************************************************/
void swapSubframeBufferTx(void)
{
    INT16 i;
    
    // toggle flag
    g_sender.bufferTx0Active = !g_sender.bufferTx0Active;
    
    // swap pointers
    if (g_sender.bufferTx0Active)
    {
        for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
        {
            g_sender.pDataCh[i] = (UINT8*)&g_subframeBufferTx0[i][0];
            g_pSubframeBufferTxCh[i] = &g_subframeBufferTx1[i][0];
        }   
    }
    else
    {
        for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
        {
            g_sender.pDataCh[i] = (UINT8*)&g_subframeBufferTx1[i][0];
            g_pSubframeBufferTxCh[i] = &g_subframeBufferTx0[i][0];
        }
    }
    
    #ifdef TEST_VOICE_DATA_TX
        testVoiceData_loadTxData(g_pSubframeBufferTxCh);
    #endif
       
}

/***************************************************************************
  Function      : void voiceData_initRx(void)
  Description   : voiceData_initRx() initializes the receiver.
  Inputs        : None
  Outputs       : None
  Return        : None
  Globals       : Various
  Important     : -
****************************************************************************/
void voiceData_initRx(void)
{
    UINT32 i, j, k;
    
    // Global flags
    g_decoderDataReady  = false;
    g_receiverInSync    = false;

    // Subframe reception
    g_receiver.state = e_WaitingRx;
    g_receiver.dataIx = 0;
    g_receiver.subframeIx = 0;
    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
        g_receiver.pDataCh[i] = (UINT8*)&g_subframeBufferRx[i][0];

    // Subframe synchronisation 
    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
    {
        g_receiver.subframeSync[i].state = e_Idle;
        g_receiver.subframeSync[i].nineBitHeader = 0;
        g_receiver.subframeSync[i].hMin = 0;
        g_receiver.subframeSync[i].iMin = 0;
    }
    
    // Channel synchronisation
    g_receiver.channelSync.state = e_OutOfSync;
    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
    {
        g_receiver.channelSync.currentActiveChannels[i] = false;
        g_receiver.channelSync.lostSyncCounter[i] = 0;
#ifdef VOICE_DATA_VERSION_2
        g_receiver.channelSync.indexSubframeHistory[i] = 0;
        g_receiver.channelSync.iMin[i]                 = FRAME_TYPE_VOICE;
        g_receiver.channelSync.channelStates[i]        = e_ChannelDown;
        g_receiver.channelSync.appAction[i]            = e_Undefined;
        if (configuration_isChannelOn(i))
            status_setWarning(getSyncFailureWarnBitMask(i));    // warning only if channel is ON by configuration
#endif
    }
    g_receiver.channelSync.framesReady =    false;
    g_receiver.channelSync.receiverInSync = false;
    
    // Subframe history
    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
    {
        g_receiver.pCurrentSFInHistory[i] = &g_receiver.subframeHistory[i][0];
        g_receiver.subframeDisplacement[i] = 0;
        for (j=0; j<N_SUBFRAMES_IN_RX_HISTORY; j++)
        {
            g_receiver.subframeHistory[i][j].index = j;
            g_receiver.subframeHistory[i][j].ready = false;
            g_receiver.subframeHistory[i][j].appAction = e_Undefined;
            g_receiver.subframeHistory[i][j].iMin = 0;
            for (k=0; k<N_SUBFRAMES_PER_FRAME; k++)
                g_receiver.subframeHistory[i][j].pSubframes[k] = NULL;
            if (j==0)
                g_receiver.subframeHistory[i][j].pPrevious = &g_receiver.subframeHistory[i][N_SUBFRAMES_IN_RX_HISTORY-1];
            else
                g_receiver.subframeHistory[i][j].pPrevious = &g_receiver.subframeHistory[i][j-1];
            if ( j == N_SUBFRAMES_IN_RX_HISTORY-1 )
                g_receiver.subframeHistory[i][j].pNext = &g_receiver.subframeHistory[i][0];
            else
                g_receiver.subframeHistory[i][j].pNext = &g_receiver.subframeHistory[i][j+1];
        }
    }
    
    // Frames for decoder
    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
    {
        g_receiver.frameForDecoder[i].pSubframe1 = NULL;
        g_receiver.frameForDecoder[i].pSubframe2 = NULL;
        g_receiver.frameForDecoder[i].pSubframe3 = NULL;
        g_receiver.frameForDecoder[i].iMin = 0;
        g_receiver.frameForDecoder[i].ready = false;
        g_receiver.frameForDecoder[i].appAction = e_Undefined;
    }

    // connection state
    g_receiver.connectionIsUp = false;
    
    // Subframe receive counter
    g_receiver.timeSinceLastRxSf = 0;
    g_receiver.timeSinceLastRxSfCDown = 0;

    // MOD600 data rate index monitoring
    g_receiver.activeDRIndex = BTS_MOD600_DR_INDEX_OUT_OF_SYNC;
    g_receiver.lastDRIndex   = BTS_MOD600_DR_INDEX_OUT_OF_SYNC;
    g_receiver.numOfConsecutiveDRIndices = 0;
    
    // AIS detector
    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
    {
        g_receiver.ais[i].detected = false;
        g_receiver.ais[i].delayCnt = 0;
        g_receiver.ais[i].detCnt = 0;
        g_receiver.ais[i].state = e_AisWait;
        status_clearWarning(getAISWarnBitMask(i));
    }
    
    #ifdef TEST_VOICE_DATA_RX
        // init testVoiceData module
        testVoiceData_generateTestData();
    #endif

    #ifdef VOICE_DATA_STATS_RX
        for (i=0; i<N_CHANNELS_ON_BOARD; i++)
        {
            gs_voiceDataStatsRx.unpackCounter[i] = 0;
            gs_voiceDataStatsRx.interpolateCounter[i] = 0;
            gs_voiceDataStatsRx.holdCounter[i] = 0;
        }
    #endif

    #ifdef VOICE_DATA_INTRODUCE_BIT_ERRORS
        gs_bitErrorCtr = 0;
    #endif
}

/***************************************************************************
  Function      : void voiceData_receive(UINT16 *pRxTimeSlot)
  Description   : voiceData_receive() receives subframe bytes, synchronizes
                  subframes, synchronizes frames and delivers them to the
                  application.
  Inputs        : pRxTimeSlot: Pointer to received time slots
                : cSValid:     True if checksum in basic timeslot is valid,
                               false otherwise.
  Outputs       : None
  Return        : None
  Globals       : Various
  Important     : -
****************************************************************************/
void voiceData_receive(UINT16 *pRxTimeSlot, bool cSValid)
{
    UINT32 i, j;
    UINT16 index;
    UINT16 mod600DRIndex;
    bool connectionGoDownCondition1 = false;
    bool connectionGoDownCondition2 = false;
    bool connectionGoDownCondition3 = false;
    
    #ifdef TEST_VOICE_DATA_RX
        testVoiceData_overWriteRxData(pRxTimeSlot);
    #endif
    
    // Get MOD600 data rate index from basic time slot
    mod600DRIndex = (*pRxTimeSlot & BTS_MOD600_DR_INDEX_MASK) >> BTS_MOD600_DR_INDEX_LSB_POSITION;
    
    // Save current MOD600 data rate index for next call of voiceData_receive()
    // and determine how many times in a row the current MOD600 data rate has
    // been received.
    if ( cSValid )
    {
        if ( mod600DRIndex == g_receiver.lastDRIndex )
        {
            if ( g_receiver.numOfConsecutiveDRIndices < UINT16_MAX )
                g_receiver.numOfConsecutiveDRIndices++;
        }
        else
        {
            g_receiver.numOfConsecutiveDRIndices = 0;
            g_receiver.lastDRIndex = mod600DRIndex;
        }
    }
    else
        g_receiver.numOfConsecutiveDRIndices = 0;
    
    // A note about MOD600, MUX600 and MOD600 data rate index:
    // Depending on the state of the channel, the MOD600 immediately falls out of sync and signals
    // this to its surroundings by setting the MOD600 data rate index to out of sync or the MOD600
    // tries to resync and waits for a configurable amount of time (Interruption timeout in MOD600
    // dialog) until it sets the MOD600 data rate index to out of sync. In the later case the MUX600
    // does not know that the MOD600 is out of sync until it sets the data rate index to out of sync
    // and therefore continues to set the SUBFRAME_START_BIT and sends random data.
    
    // If the connection is up, there are three conditions which lead to a transition to "connection
    // is down":
    // (1) MOD600 is out of sync.
    // (2) Reception of subframe start bit has timed out.
    // (3) The few most recent received MOD600 data rate indices do not correspond with
    //     active channels currently expected by the receiver.
    // If one or more of these conditions is true, the receiver is reinitialized.
    if ( g_receiver.connectionIsUp )
    {
        g_receiver.timeSinceLastRxSf++;

        if (
             (cSValid)
             &&
             (mod600DRIndex == BTS_MOD600_DR_INDEX_OUT_OF_SYNC)
           )
            connectionGoDownCondition1 = true;

        if ( g_receiver.timeSinceLastRxSf >= SUBFRAME_RX_TIMEOUT )
            connectionGoDownCondition2 = true;
            
        if (
             (g_receiver.activeDRIndex != g_receiver.lastDRIndex)
             &&
             (g_receiver.numOfConsecutiveDRIndices >= CONNECTION_GO_DOWN_CONDITION_3_THRESHOLD)
           )
            connectionGoDownCondition3 = true;

        if ( connectionGoDownCondition1 || connectionGoDownCondition2 || connectionGoDownCondition3 )
            voiceData_initRx();
    }
    else if (cSValid && (mod600DRIndex != BTS_MOD600_DR_INDEX_OUT_OF_SYNC))
    {
        // MOD in sync but no start flag received
        // (this profile has no active voice channel)
        g_receiver.timeSinceLastRxSfCDown++;
        
        if (g_receiver.timeSinceLastRxSfCDown >= NO_START_FLAG_TIMEOUT)
        {
            // clear sync failure warning for all channels
            for (i=0; i<N_CHANNELS_ON_BOARD; i++)
                status_clearWarning(getSyncFailureWarnBitMask(i));
                
            g_receiver.timeSinceLastRxSfCDown = 0;
        }
    }
    else if (cSValid && (mod600DRIndex == BTS_MOD600_DR_INDEX_OUT_OF_SYNC))
    {
        // set sync failure if MOD is out of sync 
        // (useful if profile before MOD sync failure had no active voice channel)
        for (i=0; i<N_CHANNELS_ON_BOARD; i++)
        {
            if (configuration_isChannelOn(i))
                status_setWarning(getSyncFailureWarnBitMask(i));    // warning only if channel is ON by configuration
        }
    }

    switch(g_receiver.state)
    {
        case e_WaitingRx:

            if (*pRxTimeSlot & SUBFRAME_START_BIT)
            {
                /////////////////////////////////////////
                // First byte of new subframe received //
                /////////////////////////////////////////
                
                if (
                     (!g_receiver.connectionIsUp)
                     &&
                     (cSValid)
                     &&
                     (isValidDRIndex(mod600DRIndex))
                   )
                {
                    // The connection just came up
                    // Determine which channels are active in the current MOD600 profile
                    configuration_getActiveChannels( mod600DRIndex, 
                                                     g_receiver.channelSync.currentActiveChannels);
                    g_receiver.activeDRIndex = mod600DRIndex;
                    g_receiver.connectionIsUp = true;
                }

                // Reset subframe receive timer
                g_receiver.timeSinceLastRxSf = 0;

                // Advance the current subframe in subframe history pointer
                for (i=0; i<N_CHANNELS_ON_BOARD; i++)
                    g_receiver.pCurrentSFInHistory[i] = g_receiver.pCurrentSFInHistory[i]->pNext;

                // Store pointer to last three subframes in subframe history of the current element.
                for (i=0; i<N_CHANNELS_ON_BOARD; i++)
                {
                    g_receiver.pCurrentSFInHistory[i]->pSubframes[0] = g_receiver.pCurrentSFInHistory[i]->pPrevious->pSubframes[1];
                    g_receiver.pCurrentSFInHistory[i]->pSubframes[1] = g_receiver.pCurrentSFInHistory[i]->pPrevious->pSubframes[2];
                    g_receiver.pCurrentSFInHistory[i]->pSubframes[2] = (t_Subframe*) g_receiver.pDataCh[i];
                }
                
                // Copy first received data byte into rx buffer
                *g_receiver.pDataCh[CH1]++ =  *(pRxTimeSlot + 1) & BYTE1;
                *g_receiver.pDataCh[CH2]++ = (*(pRxTimeSlot + 1) & BYTE2) >> SB8;
                *g_receiver.pDataCh[CH3]++ =  *(pRxTimeSlot + 2) & BYTE1;
                *g_receiver.pDataCh[CH4]++ = (*(pRxTimeSlot + 2) & BYTE2) >> SB8;
                
                g_receiver.dataIx = 1;
                g_receiver.state = e_Receiving; 
                
                // The first byte of a subframe has been received. This byte contains
                // the header of the entire subframe. Left shift this header into the 
                // 9 bit header buffer.
                shiftInSubFrameHeader(pRxTimeSlot);
                
                // detect AIS in the subframe header bits
                detectAIS();
            }
            break;

        case e_Receiving:

            // Copy received data byte into rx buffer
            *g_receiver.pDataCh[CH1]++ =  *(pRxTimeSlot + 1) & BYTE1;
            *g_receiver.pDataCh[CH2]++ = (*(pRxTimeSlot + 1) & BYTE2) >> SB8;
            *g_receiver.pDataCh[CH3]++ =  *(pRxTimeSlot + 2) & BYTE1;
            *g_receiver.pDataCh[CH4]++ = (*(pRxTimeSlot + 2) & BYTE2) >> SB8;
            
            g_receiver.dataIx++;

            if (g_receiver.dataIx >= SUBFRAME_LEN)
            {
                ////////////////////////////////////////////////////
                // Subframe completely received on all four channels
                ////////////////////////////////////////////////////
                g_receiver.state = e_WaitingRx;
                
                g_receiver.subframeIx++;
                if ( g_receiver.subframeIx >= N_SUBFRAMES_IN_RX_BUFFER)
                {
                    // We are at the end of the RX buffer, hence start over.
                    g_receiver.subframeIx = 0;
                    
                    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
                        g_receiver.pDataCh[i] = (UINT8*)&g_subframeBufferRx[i][0];
                }
                
                // Do subframe synchronization
                subframeSync();
                
#ifdef VOICE_DATA_VERSION_1
                // Do channel synchronization
                channelSync(pRxTimeSlot);
#else
                // do receiver synchronization
                receiverSync(pRxTimeSlot);
#endif
                
                // Deliver frames if frames are ready
                if ( g_receiver.channelSync.framesReady )
                {
#ifdef VOICE_DATA_VERSION_1
                    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
                    {
                        if ( g_receiver.channelSync.currentActiveChannels[i] )
                        {
                            index = g_receiver.subframeDisplacement[i];
                            g_receiver.frameForDecoder[i].pSubframe1 = g_receiver.subframeHistory[i][index].pSubframes[0];
                            g_receiver.frameForDecoder[i].pSubframe2 = g_receiver.subframeHistory[i][index].pSubframes[1];
                            g_receiver.frameForDecoder[i].pSubframe3 = g_receiver.subframeHistory[i][index].pSubframes[2];
                            g_receiver.frameForDecoder[i].iMin       = g_receiver.subframeHistory[i][index].iMin;
                            g_receiver.frameForDecoder[i].ready      = true;
                            g_receiver.frameForDecoder[i].appAction  = g_receiver.subframeHistory[i][index].appAction;
                            #ifdef VOICE_DATA_STATS_RX  
                                if ( g_receiver.subframeHistory[i][index].appAction == e_Unpack )
                                    if ( gs_voiceDataStatsRx.unpackCounter[i] < UINT32_MAX )
                                        gs_voiceDataStatsRx.unpackCounter[i]++; 
                                if ( g_receiver.subframeHistory[i][index].appAction == e_Interpolate )
                                    if ( gs_voiceDataStatsRx.interpolateCounter[i] < UINT32_MAX )
                                        gs_voiceDataStatsRx.interpolateCounter[i]++;    
                                if ( g_receiver.subframeHistory[i][index].appAction == e_Hold )
                                    if ( gs_voiceDataStatsRx.holdCounter[i] < UINT32_MAX )
                                        gs_voiceDataStatsRx.holdCounter[i]++;   
                            #endif
                        }
                        else
                        {
                            g_receiver.frameForDecoder[i].pSubframe1 = NULL;
                            g_receiver.frameForDecoder[i].pSubframe2 = NULL;
                            g_receiver.frameForDecoder[i].pSubframe3 = NULL;
                            g_receiver.frameForDecoder[i].iMin       = 0;
                            g_receiver.frameForDecoder[i].ready      = false;
                            g_receiver.frameForDecoder[i].appAction = e_Undefined;
                        }
                    }
#else
                    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
                    {
                        if (
                                g_receiver.channelSync.currentActiveChannels[i]
                                &&
                                IS_CHANNEL_UP(g_receiver.channelSync.channelStates[i])
                           )
                        {
                            if      (g_receiver.channelSync.channelStates[i] == e_ChannelDeliversFrames)
                            {
                                index = g_receiver.channelSync.indexSubframeHistory[i];
                                g_receiver.frameForDecoder[i].pSubframe1 = g_receiver.subframeHistory[i][index].pSubframes[0];
                                g_receiver.frameForDecoder[i].pSubframe2 = g_receiver.subframeHistory[i][index].pSubframes[1];
                                g_receiver.frameForDecoder[i].pSubframe3 = g_receiver.subframeHistory[i][index].pSubframes[2];
                                g_receiver.frameForDecoder[i].iMin       = g_receiver.channelSync.iMin[i];
                            }
                            else if (g_receiver.channelSync.channelStates[i] == e_ChannelInterpolatesOrHolds)
                            {
                                g_receiver.frameForDecoder[i].pSubframe1 = NULL;
                                g_receiver.frameForDecoder[i].pSubframe2 = NULL;
                                g_receiver.frameForDecoder[i].pSubframe3 = NULL;
                                g_receiver.frameForDecoder[i].iMin       = 0;
                            }
                            else {assert(false);}
                            g_receiver.frameForDecoder[i].ready      = true;
                            g_receiver.frameForDecoder[i].appAction  = g_receiver.channelSync.appAction[i];
                            #ifdef VOICE_DATA_STATS_RX  
                                if ( g_receiver.channelSync.appAction[i] == e_Unpack )
                                    if ( gs_voiceDataStatsRx.unpackCounter[i] < UINT32_MAX )
                                        gs_voiceDataStatsRx.unpackCounter[i]++; 
                                if ( g_receiver.channelSync.appAction[i] == e_Interpolate )
                                    if ( gs_voiceDataStatsRx.interpolateCounter[i] < UINT32_MAX )
                                        gs_voiceDataStatsRx.interpolateCounter[i]++;    
                                if ( g_receiver.channelSync.appAction[i] == e_Hold )
                                    if ( gs_voiceDataStatsRx.holdCounter[i] < UINT32_MAX )
                                        gs_voiceDataStatsRx.holdCounter[i]++;   
                            #endif
                        }
                        else
                        {
                            g_receiver.frameForDecoder[i].pSubframe1 = NULL;
                            g_receiver.frameForDecoder[i].pSubframe2 = NULL;
                            g_receiver.frameForDecoder[i].pSubframe3 = NULL;
                            g_receiver.frameForDecoder[i].iMin       = 0;
                            g_receiver.frameForDecoder[i].ready      = false;
                            g_receiver.frameForDecoder[i].appAction = e_Undefined;
                        }
                    }
#endif
                    g_decoderDataReady = true;
                    
                    #ifdef TEST_VOICE_DATA
                        testVoiceData_check(g_receiver.frameForDecoder);
                    #endif
                    
                    // GPIO E15 on (for testing)
                    //*pPORTEIO =(*pPORTEIO | 0x8000); 
                }
                else
                {
                    // GPIO E15 off (for testing)
                    //*pPORTEIO =(*pPORTEIO & 0x7FFF);
                }
                if ( g_receiver.channelSync.receiverInSync )
                    g_receiverInSync = true;
                else
                    g_receiverInSync = false;
            }
            break;
            
        default:
            assert(false);
    }
}

/***************************************************************************
  Function:     void voiceData_getRxFrames(t_RxFrameForDecoder rxFrames[N_CHANNELS_ON_BOARD])
  Description:  voiceData_getRxFrames() is a getter function to get
                access to the received frames.
  Inputs:       None
  Outputs:      rxFrames: Pointer to structure array which will be filled by the getter
  Return:       None
  Globals:      g_receiver, g_decoderDataReady
  Important: The pointers retrieved through this function are valid for at least 60 ms.
****************************************************************************/
void voiceData_getRxFrames(t_RxFrameForDecoder rxFrames[N_CHANNELS_ON_BOARD])
{
    UINT32 saveInts;
    
    saveInts = cli();   // Global Interrupt disable
    memcpy(rxFrames, g_receiver.frameForDecoder, N_CHANNELS_ON_BOARD*sizeof(t_RxFrameForDecoder));
    g_decoderDataReady = false;
    sti(saveInts);      // Global Interrupt enable
}

/***************************************************************************
  Function:     void shiftInSubFrameHeader(UINT16 *pRxTimeSlot)
  Description:  g_receiver.subframeSync[CH].nineBitHeader contains the subframe
                header bits of the three most recently received subframes.
                The oldest subframe header bits are on the left and the newest
                on the right. If a new subframe is received,
                shiftInSubFrameHeader() shifts in the three new subframe bits
                from the right and drops the oldest three subframe
                header bits on the left.
  Inputs:       pRxTimeSlot: Pointer to the received timeslots
  Outputs:      None
  Return:       None
  Globals:      gs_bitErrorCtr, g_receiver
  Important:    -
****************************************************************************/
void shiftInSubFrameHeader(UINT16 *pRxTimeSlot)
{
    UINT32 i;
    UINT16 firstByteOfSubFrame[N_CHANNELS_ON_BOARD];
    #ifdef VOICE_DATA_INTRODUCE_BIT_ERRORS
        bool introduceBitErrors;
    #endif
        
    firstByteOfSubFrame[0] =  *(pRxTimeSlot + 1) & BYTE1;
    firstByteOfSubFrame[1] = (*(pRxTimeSlot + 1) & BYTE2) >> SB8;
    firstByteOfSubFrame[2] =  *(pRxTimeSlot + 2) & BYTE1;
    firstByteOfSubFrame[3] = (*(pRxTimeSlot + 2) & BYTE2) >> SB8;
    
    #ifdef VOICE_DATA_INSERT_AIS
        if (gs_insertAis)
            firstByteOfSubFrame[gs_aisCh] = 0xff;   // insert ones
        
        gs_aisCounter++;
        if (gs_aisCounter > gs_aisCycle)
        {
            if (gs_insertAis)
                gs_insertAis = false;
            else
                gs_insertAis = true;
            
            gs_aisCounter = 0;
        }
    #endif

    #ifdef VOICE_DATA_INTRODUCE_BIT_ERRORS
        if ( gs_bitErrorCtr >= 9 )
        {
            gs_bitErrorCtr = 0;
            introduceBitErrors = true;
        }
        else
        {
            introduceBitErrors = false;
            gs_bitErrorCtr++;
        }
    #endif

    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    {
        // firstByteOfSubFrame[i] = [0000 0000 hhhx xxxx]
    
        firstByteOfSubFrame[i] >>= 5;
    
        // firstByteOfSubFrame[i] = [0000 0000 0000 0hhh]
    
        #ifdef VOICE_DATA_INTRODUCE_BIT_ERRORS
            if ( introduceBitErrors )
            {
                // Flip all three header bits and therefore introduce three bit errors
                
                firstByteOfSubFrame[i] = ~firstByteOfSubFrame[i]; 
                // firstByteOfSubFrame[i] = [1111 1111 1111 1eee]

                firstByteOfSubFrame[i] = firstByteOfSubFrame[i] & 0x0007;
                // firstByteOfSubFrame[i] = [0000 0000 0000 0eee]
            }
        #endif

        // g_receiver.subframeSync[i].nineBitHeader = [0000 000a aabb bccc]
    
        g_receiver.subframeSync[i].nineBitHeader <<= 3;
    
        // g_receiver.subframeSync[i].nineBitHeader = [0000 aaab bbcc c000]
    
        //g_receiver.subframeSync[i].nineBitHeader &= [0000 0001 1111 1000];
        g_receiver.subframeSync[i].nineBitHeader &= 0x01F8;
    
        // g_receiver.subframeSync[i].nineBitHeader = [0000 000b bbcc c000]
    
        g_receiver.subframeSync[i].nineBitHeader |= firstByteOfSubFrame[i];
    
        // g_receiver.subframeSync[i].nineBitHeader = [0000 000b bbcc chhh]
        
    }
}

/***************************************************************************
  Function:     void subframeSync(void)
  Description:  subFrameSync() runs the state machine for header detection as
                described in "Framing for O4CV" (TM-PSND06-1564) for each
                channel.
                subFrameSync() is called if a new subframe was received
                on the four channels.
                A subframe which is the last subframe of a correctly
                received frame is marked with the ready flag in the subframe
                history.
                subFrameSync() sets iMin in the subframe history for
                all four received subframes.
                subFrameSync() does not take into account whether a channel
                is active. If it is called, it assumes that a new subframe
                was received on all four channels.
  Inputs:       None
  Outputs:      None
  Return:       None
  Globals:      Within subframeSync()
                In:
                g_receiver.subframeSync[i].state
                g_receiver.subframeSync[i].hMin
                g_receiver.subframeSync[i].iMin
                Out:
                g_receiver.subframeSync[i].state
                g_receiver.pCurrentSFInHistory[i]->ready
                g_receiver.pCurrentSFInHistory[i]->iMin

                Within calcHMin()
                In:
                g_receiver.subframeSync[i].nineBitHeader
                Out:
                g_receiver.subframeSync[i].hMin
                g_receiver.subframeSync[i].iMin
  Important:    -
****************************************************************************/
void subframeSync(void)
{
    UINT32 i;
    bool frameAvailable;
    bool inSync;
    
    // Calculate hMin and iMin for each channel
    calcHMin();

    // Run header detection state machine
    for (i=0; i<N_CHANNELS_ON_BOARD; i++)
    {
        switch (g_receiver.subframeSync[i].state)
        {
            
            case e_Idle:
                if (g_receiver.subframeSync[i].hMin == 0)
                {
                    frameAvailable = true;
                    inSync = true;
                    
                    g_receiver.subframeSync[i].state = e_Sync1;
                }
                else
                {
                    frameAvailable = false;
                    inSync = false;
                }
                break;
                
            case e_Sync1:
                if (g_receiver.subframeSync[i].hMin >= 1)
                {
                    frameAvailable = false;
                    inSync = false;
                    
                    g_receiver.subframeSync[i].state = e_Sync2;
                }
                else
                {
                    frameAvailable = true;
                    inSync = false;
                }
                break;
            
            case e_Sync2:
                if (g_receiver.subframeSync[i].hMin >= 1)
                {
                    frameAvailable = false;
                    inSync = false;
                    
                    g_receiver.subframeSync[i].state = e_Sync3;
                }
                else
                {
                    frameAvailable = true;
                    inSync = false;
                    
                    g_receiver.subframeSync[i].state  = e_Sync1;
                }
                break;
                        
            case e_Sync3:
                if (g_receiver.subframeSync[i].hMin <= 2)
                {
                    frameAvailable = true;
                    inSync = true;
                }
                else
                {
                    frameAvailable = false;
                    inSync = false;
                }   
                g_receiver.subframeSync[i].state = e_Sync1;
                break;
                        
            default:
                assert(false);
        } // switch over state

        // Set the ready flag in the subframe history
        g_receiver.pCurrentSFInHistory[i]->ready = frameAvailable;

        // Set iMin in the subframe history
        g_receiver.pCurrentSFInHistory[i]->iMin = g_receiver.subframeSync[i].iMin;

    } // for loop over channels
}


#ifdef VOICE_DATA_VERSION_1
/***************************************************************************
  Function:     void channelSync(UINT16 *pRxTimeSlot)
  Description:  channelSync() determines whether it is possible to deliver
                a frame to the application for all active channels and, if
                this is the case, it provides access to this frames. In this
                context a channel is active, if it is configured and switched
                on in the current FB/FF profile.
                channelSync() is tolerant to corrupt frames to a certain
                degree. If a frame is ready for a certain channel it is
                marked with the tag e_Unpack to make the application unpack
                and decode. If at some time no frame is available for
                delivery, a dummy frame with a e_Interpolate or e_Hold is
                forwarded to the application. e_Interpolate is used in case
                of a phone call and makes the decoder interpolate the missing
                frame, whereas e_Hold is used for fax calls which makes the
                decoder hold the last value. If some number of consecutive
                frames are missing the receiver goes out of sync and therefore
                stops delivering frames for all channels.
                channelSync() and its subfunctions findChannelSync(),
                checkChannelSync() and findReadySubframe() do there job
                based on the following input:
                    (1) g_receiver.subframeHistory[][]
                    (2) g_receiver.pCurrentSFInHistory[]
                    (3) g_receiver.channelSync.currentActiveChannels[]
                Several fields of (1) are used. However, the ready flag is the
                most important, since it is used to determine whether a
                frame is ready for delivery for a certain channel.
                channelSync() and its subfunctions produce the following
                output:
                    (1) g_receiver.channelSync.framesReady
                    (2) g_receiver.channelSync.reiceiverInSync
                    (3) g_receiver.subframeDisplacement[]
                    (4) g_receiver.subframeHistory[][].appAction
                channelSync() and its subfunctions have the following
                state:
                    (1) g_receiver.channelSync.state[]
                    (2) g_receiver.channelSync.lostSyncCounter[]
  Inputs:       pRxTimeSlot: Pointer to received timeslots
  Outputs:      None
  Return:       None
  Globals:      g_receiver
  Important:    -
****************************************************************************/
void channelSync(UINT16 *pRxTimeSlot)
{
    UINT32 i;
    
    g_receiver.channelSync.framesReady = false;
    
    switch (g_receiver.channelSync.state)
    {
        
        case e_OutOfSync:
            if ( findChannelSync() )
            {
                // Ready subframe was found for each active channel.
                g_receiver.channelSync.framesReady = true;
                g_receiver.channelSync.receiverInSync = true;
                g_receiver.channelSync.state = e_InSync1;
            }
            break;
            
        case e_InSync1:
            g_receiver.channelSync.state = e_InSync2;
            break;
            
        case e_InSync2:
            g_receiver.channelSync.state = e_InSync3;
            break;
            
        case e_InSync3:
            if ( checkChannelSync() )
            {
                // Ready subframe was found for each active channel.
                g_receiver.channelSync.framesReady = true;
                g_receiver.channelSync.state = e_InSync1;
            }
            else
            {
                // For at least one active channel, no ready subframe
                // was found in the subframe history.
                g_receiver.channelSync.receiverInSync = false;
                g_receiver.channelSync.state = e_OutOfSync;
            }
            
            break;
            
        default:
            assert(false);
    } // switch
}

/***************************************************************************
  Function:     bool findChannelSync(void)
  Description:  findChannelSync() searches the subframe history for ready
                flagged subframes.
                If there was a ready flagged subframe found for all active
                channels, true is returned and false otherwise. Additionally,
                if true is returned, findChannelSync() sets
                g_receiver.subframeDisplacement[] for all active channels.
  Inputs:       None
  Outputs:      None
  Return:       True, if ready flagged subframe was found for all active
                channels and false otherwise.
  Globals:      g_receiver
  Important:    -
****************************************************************************/
bool findChannelSync(void)
{
    UINT16 i;
    bool areAllActiveAvailable = true;
    bool atLeastOneActive = false;
    
    for ( i=0; i<N_CHANNELS_ON_BOARD; i++ )
    {
        if ( g_receiver.channelSync.currentActiveChannels[i] )
        {
            atLeastOneActive = true;
            if ( !findReadySubframe(i) )
            {
                areAllActiveAvailable = false;
                break;
            }
            else
            {
                g_receiver.subframeHistory[i][g_receiver.subframeDisplacement[i]].appAction = e_Unpack;
            }
        }
    }
    
    if ( atLeastOneActive )
        return areAllActiveAvailable;
    else
        return false;
}

/***************************************************************************
  Function:     bool checkChannelSync(void)
  Description:  checkChannelSync() searches the subframe history for ready flagged
                frames. It returns true, if one of the following conditions is met
                and false otherwise:
                    (1) All active channels have a ready flagged frame in their
                        subframe history.
                    (2) The active channels which do not have a ready flagged
                        frame in their subframe history have not failed to deliver
                        a frame more than LOST_SYNC_THRESHOLD times.
                Furthermore checkChannelSync() determines and writes the appAction
                field of the subframe history element, provided the function
                returns true.
  Inputs:       None
  Outputs:      None
  Return:       True, if a ready flagged frame was found in the subframe history
                for all active channels or if the number of consecutive times
                no ready flagged frame was found did not exceed the lost sync
                threshold. False, otherwise.
  Globals:      In:
                g_receiver.channelSync.currentActiveChannels[]
                g_receiver.subframeHistory[][].ready
                g_receiver.channelSync.lostSyncCounter[]
                g_receiver.subframeHistory[][].iMin
                Out:
                g_receiver.channelSync.lostSyncCounter[]
                g_receiver.subframeHistory[][].appAction
  Important:    -
****************************************************************************/
bool checkChannelSync(void)
{
    UINT16 i;
    bool areAllActiveAvailable = true;
    bool atLeastOneActive = false;
    
    for ( i=0; i<N_CHANNELS_ON_BOARD; i++ )
    {
        if ( g_receiver.channelSync.currentActiveChannels[i] )
        {
            atLeastOneActive = true;

            if ( !g_receiver.subframeHistory[i][g_receiver.subframeDisplacement[i]].ready )
            {
                // Not same position as last time
                // Search ready tag
                if ( !findReadySubframe(i) )
                {
                    // No ready tagged subframe found
                    if ( g_receiver.channelSync.lostSyncCounter[i] >= LOST_SYNC_THRESHOLD )
                    {
                        // Lost sync threshold exceeded. Synchronisation is lost.
                        areAllActiveAvailable = false;
                        break;
                    }
                    else
                    {
                        // Keep synchronisation by making the application interpolate or hold
                        g_receiver.channelSync.lostSyncCounter[i]++;
                        if ( g_receiver.subframeHistory[i][g_receiver.subframeDisplacement[i]].iMin == FRAME_TYPE_VOICE )
                            g_receiver.subframeHistory[i][g_receiver.subframeDisplacement[i]].appAction = e_Interpolate;
                        else
                            g_receiver.subframeHistory[i][g_receiver.subframeDisplacement[i]].appAction = e_Hold;
                    }
                }
                else
                {
                    // A ready flagged subframe history element was found at another position
                    g_receiver.subframeHistory[i][g_receiver.subframeDisplacement[i]].appAction = e_Unpack;
                    g_receiver.channelSync.lostSyncCounter[i] = 0;
                }
            }
            else
            {
                // There is a ready flagged subframe history element at the same position as last time
                g_receiver.subframeHistory[i][g_receiver.subframeDisplacement[i]].appAction = e_Unpack;
                g_receiver.channelSync.lostSyncCounter[i] = 0;
            }
        }
    }
    
    if (!areAllActiveAvailable || !atLeastOneActive)
    {
        // The receiver will fall out of sync. Reset lost sync counter.
        for ( i=0; i<N_CHANNELS_ON_BOARD; i++ )
            g_receiver.channelSync.lostSyncCounter[i] = 0;
    }

    if ( atLeastOneActive )
        return areAllActiveAvailable;
    else
        return false;
}

/***************************************************************************
  Function:     bool findReadySubframe(UINT16 channel)
  Description:  findReadySubframe() traverses through the subframe history
                from the most recent subframe history element to the oldest.
                It does that for the channel specified by the input argument
                channel. If a ready flagged subframe history element is found,
                traversal is stopped, its index is stored in
                g_receiver.subframeDisplacement[channel] and true is returned.
                If no ready flagged subframe is found, false is returned.
  Inputs:       UINT16 channel: Channel for which a ready flagged subframe is to
                be searched.
  Outputs:      None
  Return:       True, if a ready flagged subframe history element was found.
                False otherwise.
  Globals:      In:
                g_receiver.pCurrentSFInHistory[]
                g_receiver.subframeHistory[][].ready
                g_receiver.subframeHistory[][].index
                g_receiver.subframeHistory[][].pPrevious
                Out:
                g_receiver.subframeDisplacement[]
  Important:    -
****************************************************************************/
bool findReadySubframe(UINT16 channel)
{
    UINT32 i;
    bool readySubframeFound = false;
    t_RxSFHistoryElement *pSFHistoryElement = g_receiver.pCurrentSFInHistory[channel];
        
    for (i=0; i<N_SUBFRAMES_IN_RX_HISTORY; i++)
    {
        if (pSFHistoryElement->ready)
        {
            readySubframeFound = true;
            // Store position of ready subframe
            g_receiver.subframeDisplacement[channel] = pSFHistoryElement->index;
            break;
        }
        pSFHistoryElement = pSFHistoryElement->pPrevious;
    }
    
    return readySubframeFound;
}
#else

/***************************************************************************
  Function:     void receiverSync(UINT16 *pRxTimeSlot)
  Description:  receiverSync() determines if the receiver is able to
                synchronize to the incoming data and if it is possible to
                deliver data to the application.
                The receiver can synchronize if it finds a frame in at least
                one of the active channels. The receiver falls out of synchronization
                if it was not able to find a frame in one one the active channels
                for a certain time.
                receiverSync() is called after reception of a new subframe.
                It delivers frames when it gets in sync and every 3 times
                when it is in sync.
  Inputs:       pRxTimeSlot: Pointer to received timeslots
  Outputs:      None
  Return:       None
  Globals:      Out:
                    g_receiver.channelSync.framesReady
                    g_receiver.channelSync.receiverInSync
                State:
                    g_receiver.channelSync.state
  Important:    -
****************************************************************************/
void receiverSync(UINT16 *pRxTimeSlot)
{
    UINT32 i;
    
    g_receiver.channelSync.framesReady = false;
    
    switch (g_receiver.channelSync.state)
    {
        
        case e_OutOfSync:
            if ( isAtLeastOneChannelUp() )
            {
                // Ready subframe was found for each active channel.
                g_receiver.channelSync.framesReady = true;
                g_receiver.channelSync.receiverInSync = true;
                g_receiver.channelSync.state = e_InSync1;
            }
            break;
            
        case e_InSync1:
            g_receiver.channelSync.state = e_InSync2;
            break;
            
        case e_InSync2:
            g_receiver.channelSync.state = e_InSync3;
            break;
            
        case e_InSync3:
            if ( isAtLeastOneChannelUp() )
            {
                // Ready subframe was found for each active channel.
                g_receiver.channelSync.framesReady = true;
                g_receiver.channelSync.state = e_InSync1;
            }
            else
            {
                // For at least one active channel, no ready subframe
                // was found in the subframe history.
                g_receiver.channelSync.receiverInSync = false;
                g_receiver.channelSync.state = e_OutOfSync;
            }
            
            break;
            
        default:
            assert(false);
    } // switch
}

/***************************************************************************
  Function:     bool isAtLeastOneChannelUp(void)
  Description:  This function determines if at least one channel is up.
  Inputs:       None
  Outputs:      None
  Return:       True, if at least one frame was found in the subframe history
                or if the number of consecutive times
                no ready flagged frame was found did not exceed the lost sync
                threshold. False, otherwise.
  Globals:      In:
                g_receiver.channelSync.currentActiveChannels[]
                g_receiver.channelSync.indexSubframeHistory[]
                g_receiver.subframeHistory[][]
                g_receiver.channelSync.iMin[]
                Out:
                g_receiver.channelSync.appAction[]
                State:
                g_receiver.channelSync.channelStates[]
                g_receiver.channelSync.lostSyncCounter[]
  Important:    -
****************************************************************************/
bool isAtLeastOneChannelUp(void)
{
    UINT16 i;
    bool isAtLeastOneChannelUp = false;
    
    // execute state machine for all active channels
    for ( i=0; i<N_CHANNELS_ON_BOARD; i++ )
    {
        if ( g_receiver.channelSync.currentActiveChannels[i] )
        {
            switch ( g_receiver.channelSync.channelStates[i] )
            {
            case e_ChannelDown:
            
                if ( isFrameFoundInChannel(i) )
                {
                    g_receiver.channelSync.appAction[i] = e_Unpack;
                    g_receiver.channelSync.channelStates[i] = e_ChannelDeliversFrames;  //>>> transition
                    status_clearWarning(getSyncFailureWarnBitMask(i));
                }
                else
                {
                    g_receiver.channelSync.appAction[i] = e_Undefined;
                }
                break;
                
            case e_ChannelDeliversFrames:
            
                if (g_receiver.subframeHistory[i][g_receiver.channelSync.indexSubframeHistory[i]].ready)
                {
                    // frame found at same position as last time
                    g_receiver.channelSync.iMin[i] = g_receiver.subframeHistory[i][g_receiver.channelSync.indexSubframeHistory[i]].iMin;
                    g_receiver.channelSync.appAction[i] = e_Unpack;
                }
                else if ( isFrameFoundInChannel(i) )
                {
                    // frame found at new position
                    g_receiver.channelSync.appAction[i] = e_Unpack;
                }
                else
                {
                    if ( g_receiver.channelSync.iMin[i] == FRAME_TYPE_VOICE )
                        {g_receiver.channelSync.appAction[i] = e_Interpolate;}
                    else
                        {g_receiver.channelSync.appAction[i] = e_Hold;}
                    g_receiver.channelSync.channelStates[i]   = e_ChannelInterpolatesOrHolds; //>>> transition
                    g_receiver.channelSync.lostSyncCounter[i] = 1;
                }
                break;
                
            case e_ChannelInterpolatesOrHolds:
            
                if ( isFrameFoundInChannel(i) )
                {
                    g_receiver.channelSync.appAction[i] = e_Unpack;
                    g_receiver.channelSync.channelStates[i] = e_ChannelDeliversFrames; //>>> transition
                }
                else if ( g_receiver.channelSync.lostSyncCounter[i] > LOST_SYNC_THRESHOLD )
                {
                    g_receiver.channelSync.appAction[i] = e_Undefined;
                    g_receiver.channelSync.channelStates[i] = e_ChannelDown; //>> transition
                    status_setWarning(getSyncFailureWarnBitMask(i));
                }
                else
                {
                    if ( g_receiver.channelSync.iMin[i] == FRAME_TYPE_VOICE )
                        {g_receiver.channelSync.appAction[i] = e_Interpolate;}
                    else
                        {g_receiver.channelSync.appAction[i] = e_Hold;}
                    g_receiver.channelSync.lostSyncCounter[i]++;
                }
                break;
                
            default: assert(false); break;
            }
        }
        else // channel not active
        {
            status_clearWarning(getSyncFailureWarnBitMask(i));
        }
    }
    
    // determine if at least one channel is up
    for ( i=0; i<N_CHANNELS_ON_BOARD; i++ )
    {
        if ( g_receiver.channelSync.currentActiveChannels[i] )
        {
            if (g_receiver.channelSync.channelStates[i] != e_ChannelDown)
                {isAtLeastOneChannelUp = true; break;}
        }
    }

    return isAtLeastOneChannelUp;
}

/***************************************************************************
  Function:     bool isFrameFoundInChannel(UINT16 channel)
  Description:  
                isFrameFoundInChannel() searches the subframe history
                of a channel for ready flagged subframes. The search starts
                at the most recent subframe received and ends at the oldest
                subframe received.
  Inputs:       UINT16 channel: Channel for which a ready flagged subframe is to
                                be searched.
  Outputs:      None
  Return:       True, if a ready flagged subframe history element was found.
                False otherwise.
  Globals:      In:
                g_receiver.pCurrentSFInHistory[]
                g_receiver.subframeHistory[][].ready
                g_receiver.subframeHistory[][].index
                g_receiver.subframeHistory[][].iMin
                g_receiver.subframeHistory[][].pPrevious
                Out:
                g_receiver.channelSync.indexSubframeHistory[]
                g_receiver.channelSync.iMin[]
  Important:    -
****************************************************************************/
bool isFrameFoundInChannel(UINT16 channel)
{
    UINT32 i;
    bool isFrameFoundInChannel = false;
    t_RxSFHistoryElement *pSFHistoryElement = g_receiver.pCurrentSFInHistory[channel];
        
    for (i=0; i<N_SUBFRAMES_IN_RX_HISTORY; i++)
    {
        if (pSFHistoryElement->ready)
        {
            isFrameFoundInChannel = true;
            g_receiver.channelSync.indexSubframeHistory[channel]  = pSFHistoryElement->index;
            g_receiver.channelSync.iMin[channel]                  = pSFHistoryElement->iMin;
            break;
        }
        pSFHistoryElement = pSFHistoryElement->pPrevious;
    }
    
    return isFrameFoundInChannel;
}

#endif

/***************************************************************************
  Function:     void calcHMin(void)
  Description:  calcHMin() calculates hMin and iMin for each channel bases on the
                current 9 bit distributed header.
  Inputs:       None    
  Outputs:      None
  Return:       None
  Globals:      In:
                g_receiver.subframeSync[i].nineBitHeader
                Out:
                g_receiver.subframeSync[i].hMin
                g_receiver.subframeSync[i].iMin
  Important:    -
****************************************************************************/
void calcHMin(void)
{
    UINT32 i, j;
    UINT16 h[3];
    UINT16 hMin;
    UINT16 iMin;
    // const UINT16 nineBitHeader[3] = {DISTRIBUTED_9BIT_HEADER_VOICE, DISTRIBUTED_9BIT_HEADER_FAX, DISTRIBUTED_9BIT_HEADER_SIG}; //RBi todo
    const UINT16 nineBitHeader[3] = {0x00AD, 0x0076, 0x01C0};
                                     
    // Loop over all channels
    for ( i=0; i<N_CHANNELS_ON_BOARD; i++ )
    {
        // Loop over frame types (Voice, Fax and Signalling)
        hMin = 9;
        iMin = 0;
        for ( j=0; j<3; j++ )
        {
            // The asm function must be volatile otherwise it will not work correctly after the optimization
            asm volatile( "     \
            %1 = %1 ^ %2;       \
            %0 = ones %1;   " : "=l" (h[j]) : "d" (g_receiver.subframeSync[i].nineBitHeader) , "d" (nineBitHeader[j]) );
        
            // Keep track of the minimum Hamming distance
            if ( h[j] < hMin )
            {
                hMin = h[j];
                iMin = j;
            }
        } // Loop over frame types (Voice, Fax and Signalling)
        
        g_receiver.subframeSync[i].hMin = h[iMin];
        g_receiver.subframeSync[i].iMin = iMin + 1; // g_receiver.subframeSync[i].iMin is one based

    } // Loop over all active channels
}

/***************************************************************************
  Function:     inline bool isValidDRIndex(UINT16 mod600DRIndex)
  Description:  isValidDRIndex() check if the MOD600 data rate index is valid.
                mod600DRIndex = [0,4] is valid.
  Inputs:       mod600DRIndex: MOD600 data rate index   
  Outputs:      None
  Return:       True, if MOD600 data rate index is valid.
                False, otherwise.
  Globals:      None
  Important:    -
****************************************************************************/
inline bool isValidDRIndex(UINT16 mod600DRIndex)
{
    if ( mod600DRIndex <= BTS_MOD600_DR_INDEX_PROFILE_5 )
        return true;
    else
        return false;
}


/***************************************************************************
  Function:     void detectAIS(void)
  Description:  Detects an alarm indecation signal (AIS = all ones) in the 
                the subframe header bits of the voice channels.
  Inputs:       Subframe header bits    
  Outputs:      AIS detected
  Return:       None
  Globals:      g_receiver.
  Important:    -
****************************************************************************/
void detectAIS(void)
{
    INT16 i;
    bool isAis;
                     
    // Loop over all channels
    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    {
        if (g_receiver.channelSync.currentActiveChannels[i])    // channel is activated by configuration
        {
            // check if AIS in the last three subframe header bits
            isAis = (g_receiver.subframeSync[i].nineBitHeader & NINE_BIT_HEADER_MASK) 
                     == NINE_BIT_HEADER_MASK;
                     
            switch (g_receiver.ais[i].state)
            {
                case e_AisWait:
                
                    if (isAis)
                    {
                        g_receiver.ais[i].delayCnt++;
                        g_receiver.ais[i].state = e_AisOnDelay;
                    }   
                    break;
                
                case e_AisOnDelay:
                
                    if (isAis)
                    {
                        g_receiver.ais[i].delayCnt++;
                        if (g_receiver.ais[i].delayCnt >= AIS_ON_TH)
                        {
                            // AIS detected
                            g_receiver.ais[i].detected = true;
                            g_receiver.ais[i].delayCnt = 0;
                            g_receiver.ais[i].detCnt++;
                            g_receiver.ais[i].state = e_AisDetected;
                            
                            status_setWarning(getAISWarnBitMask(i));
                        }
                    }
                    else
                    {
                        g_receiver.ais[i].delayCnt = 0;
                        g_receiver.ais[i].state = e_AisWait;
                    }       
                    break;
                
                case e_AisDetected:
                
                    g_receiver.ais[i].detCnt++;
                    
                    if (!isAis)
                    {
                        g_receiver.ais[i].delayCnt++;
                        g_receiver.ais[i].state = e_AisOffDelay;
                    }   
                    
                    break;
                
                case e_AisOffDelay:
                
                    if (!isAis)
                    {
                        g_receiver.ais[i].delayCnt++;
                        if (g_receiver.ais[i].delayCnt >= AIS_OFF_TH)
                        {
                            // AIS cleared
                            g_receiver.ais[i].delayCnt = 0;
                            g_receiver.ais[i].detected = false;
                            g_receiver.ais[i].state = e_AisWait;
                            
                            status_clearWarning(getAISWarnBitMask(i));
                        }
                    }
                    else
                    {
                        g_receiver.ais[i].delayCnt = 0;
                        g_receiver.ais[i].state = e_AisDetected;
                    }       
                    break;
                
                default:
                    assert(false);
            
            } // switch
        }
        else // channel not active -> reset AIS detector
        {
            g_receiver.ais[i].detected = false;
            g_receiver.ais[i].delayCnt = 0;
            g_receiver.ais[i].state = e_AisWait;
            
            status_clearWarning(getAISWarnBitMask(i));
        }
        
    } // for loop
        
}


/***************************************************************************
  Function:     UINT16 getAISWarnBitMask(INT16 channel)
  Description:  Returns the the AIS warnig bit mask for the selected channel.   
  Outputs:      -
  Return:       AIS warning bit mask for channel.
  Globals:      -.
  Important:    -
****************************************************************************/
UINT16 getAISWarnBitMask(INT16 channel)
{
    switch (channel)
    {
        case CH1: return(O4CV_WARNING_AIS_ON_CH1);
        case CH2: return(O4CV_WARNING_AIS_ON_CH2);
        case CH3: return(O4CV_WARNING_AIS_ON_CH3);
        case CH4: return(O4CV_WARNING_AIS_ON_CH4);
        
        default:  return(O4CV_WARNING_NO_WARNING);
    }
}


/***************************************************************************
  Function:     UINT16 getSyncFailureWarnBitMask(INT16 channel)
  Description:  Returns the the sync failure warnig bit mask for the 
                selected channel.   
  Outputs:      -
  Return:       Sync failure warning bit mask for channel.
  Globals:      -.
  Important:    -
****************************************************************************/
UINT16 getSyncFailureWarnBitMask(INT16 channel)
{
    switch (channel)
    {
        case CH1: return(O4CV_WARNING_SYNC_FAILURE_CH1);
        case CH2: return(O4CV_WARNING_SYNC_FAILURE_CH2);
        case CH3: return(O4CV_WARNING_SYNC_FAILURE_CH3);
        case CH4: return(O4CV_WARNING_SYNC_FAILURE_CH4);
        
        default:  return(O4CV_WARNING_NO_WARNING);
    }
}


/* TEST FUNCTIONS **********************************************************/

/*void fillSubframeBufferTx()
{
    INT16 i, j, k;
    UINT8 *pData;
    
    
    pData = (UINT8*)&g_subframeBufferTx1[0][0];
    
    for (i = 0; i < 108; i++) *pData++ = i + 1;


    for (i = 0; i <  N_CHANNELS_ON_BOARD; i++)
    {
        pData = (UINT8*)g_pSubframeBufferTxCh[i];
        
        for (j = 0; j < 27; j++)
        {
            *pData++ = j + 1;
        }
    }     
} */


