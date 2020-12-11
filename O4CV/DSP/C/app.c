/*-------------------------------------------------------------------------*
 * $Workfile	: app.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/C/app.c $
 * 
 * 22    12-03-29 8:41 Chalnot
 * Enable/disable SW LEC according to configuration.
 * 
 * 18    10.12.07 13:03 Chhamaa
 * Functions DuSlic_stopLEC() and DuSlic_startLEC() replaced by
 * DuSlic_SpiWriteFIFO()
 * 
 * 17    6.12.07 17:44 Chhamaa
 * Remote DTMF monitoring added.
 * 
 * 16    07-11-23 10:41 Chrobif
 * Debug switch DEBUG_TELEPHONY used to change CP tone frequency in case
 * of debuging telephony.
 * 
 * 15    07-09-28 11:28 Chrobif
 * + Implementation of FB/FF via ready flag.
 * + Implementation of unpack/interpolate/hold via appAction flag.
 * 
 * 14    21.09.07 16:56 Chhamaa
 * Functions app_setVoiceState(), setFaxState() added.
 * 
 * 13    20.09.07 18:36 Chhamaa
 * payLoadDialTone modified.
 * 
 * 12    14.09.07 16:49 Chhamaa
 * app_faxControl() modified: LEC start/stop functions added.
 * 
 * 11    07-09-13 14:52 Chrobif
 * app_getFrameForDecoderPtr() added.
 * 
 * 10    27.08.07 18:22 Chhamaa
 * New functions added: app_faxControl(), app_getStatusPtr()
 * 
 * 9     22.08.07 14:01 Chhamaa
 * CP tone generation for service phone modified.
 * 
 * 8     17.08.07 17:41 Chhamaa
 * New functions added: calcEWireBufferPtrs(), calcMWireBufferPtrs()
 * 
 * 7     15.08.07 16:00 Chhamaa
 * Comment added.
 * 
 * 6     13.08.07 11:04 Chhamaa
 * CP tone generation added.
 * 
 * 5     25.07.07 13:26 Chhamaa
 * Call of configuration_getDataRate() added.
 * 
 * 4     25.07.07 10:10 Chhamaa
 * Speech filter and gain functions added.
 * 
 * 3     12.07.07 10:40 Chhamaa
 * New functions added: app_activateServicePhone(),
 * app_deactivateServicePhone(void), app_getPtrsTFDataMem(void)
 * 
 * 2     9.07.07 15:54 Chhamaa
 * Service phone added.
 * 
 * 1     4.07.07 11:48 Chhamaa
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

// C Run-Time Library
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "O4CV_Types.h"
#include "Basetype_O4CV.h"
#include "O4CV_Const.h"

#include "app.h"
#include "GaoTF.h"
#include "packext.h"
#include "voiceData.h"
#include "subframe.h"
#include "sport1.h"
#include "sport2.h"
#include "intDec.h"
#include "configuration.h"
#include "gain.h"
#include "speechFilter.h"
#include "binInOut.h"
#include "DuSlic.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// pure delay buffer length for SW LEC (in # of samples)
#define PURE_DELAY_LEN	240

// CP tone settings
#define FREQ_OFF		0xFF
#define T_NO_REPORT		4000	// [ms] with this setting CP tones are not reported
								// but still transferred
// number of FAX CP tones
#define N_FAX_CP_TONES	2

// FAX CP tone frequencies
#define CNG_TONE_FREQ	1100		// [Hz]
#define CED_TONE_FREQ	2100		// [Hz]

// FAX CP tone indexes
#define CNG_TONE_IX		0			
#define CED_TONE_IX		1

// FAX CP tone settings [ms]
#define CNG_T_ON1		500
#define CNG_T_OFF1		0
#define CNG_T_ON2		0
#define CNG_T_OFF2		0

#define CED_T_ON1		1000
#define CED_T_OFF1		0
#define CED_T_ON2		0
#define CED_T_OFF2		0

// number of voice CP tones
#define N_VOICE_CP_TONES	1

// voice CP tone frequencies
#ifdef DEBUG_TELEPHONY
	// Use an alternative dial tone frequency when debugging.
	// Otherwise signalisation (CP tone) is send as long as the dial tone is transmitted.
	#define DIAL_TONE_FREQ		525		// [Hz]
#else
	#define DIAL_TONE_FREQ		425		// [Hz]
#endif

// voice CP tone indexes
#define DIAL_TONE_IX		2

// voice CP tone settings [ms]
#define DIAL_T_ON1		T_NO_REPORT
#define DIAL_T_OFF1		0
#define DIAL_T_ON2		0
#define DIAL_T_OFF2		0

// state change
#define ALL_STATES_CHANGE	(TFCMD_CHANGE | LEC_CHANGE | VAGC_CHANGE | CP_DET_CHANGE | \
 							 CP_GEN_CHANGE | DTMF_DET_CHANGE | DTMF_GEN_CHANGE | CODEC_CHANGE)

// constant for bad voice frame generation (interpolation)
static const TFPayloadStruc payLoadBadVoice =	{PAYLOAD_BADVOICE,
											   	 0,0,0,0,0,0,0,0,0,0,0,0,
											   	 0,0,0,0,0,0,0,0,0,0,0,0};
											   	 
// constants and defines for ringing and busy tone generation						 
static const TFPayloadStruc payLoadDialTone = 	{PAYLOAD_CPTONE,
											   	 0x02,0x02,0xFF,0x00,0x00,0x00,
											   	 0x00,0x00,0x00,0x00,0x00,0x00,
											   	 0x00,0x00,0x00,0x00,0x00,0x00,
											   	 0x00,0x00,0x00,0x00,0x00,0x00};
											   	 
static const TFPayloadStruc payLoadEmpty = 		{PAYLOAD_NODATA,
											   	 0,0,0,0,0,0,0,0,0,0,0,0,
											   	 0,0,0,0,0,0,0,0,0,0,0,0};

#define RINGING_TONE_T_ON		33		// 1s (33*30 ms)
#define RINGING_TONE_T_OFF		133		// 4s (133*30 ms)

#define BUSY_TONE_T_ON			17		// 0.5s (17*30 ms)
#define BUSY_TONE_T_OFF			17		// 0.5s (17*30 ms)

#define ALL	5	// 0-3 CH1-4, 4 SPH

#define REMOTE_DTMF_MONITOR_HOLD_TIME	6	// 180 ms

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef UWORD t_TFDataMem[TF_DATAMEM_SIZE >>1];
typedef QWORD t_PureDelayBuf[PURE_DELAY_LEN];

typedef enum {

	e_ToneOn,
	e_ToneOff

} e_CpToneStates;

typedef struct
{
    bool active;
    UINT16 onTime;
    UINT16 offTime;
    UINT16 timer;
    e_CpToneStates state;
    
} t_CpTone;

typedef enum {
    
    e_Idle,
    e_ToneDetected
    
} e_DTMFDetState;

typedef struct
{
	e_DTMFDetState state;
	UINT16 holdCounter;
	
} t_DTMFMonitor;
    

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

// TF data memory
static section ("sdram") t_TFDataMem 	g_TFDataMem[N_CHANNELS_ON_BOARD_PLUS_SPH];
static UWORD *g_pTFDataMem[N_CHANNELS_ON_BOARD_PLUS_SPH];

// pure delay buffer for SW LEC
static section ("sdram") t_PureDelayBuf	g_pureDelayBuf[N_CHANNELS_ON_BOARD];

// TF config data
static section ("sdram") TFConfigStruc	g_TFConfigData[N_CHANNELS_ON_BOARD_PLUS_SPH];

// TF status data
static TFStatusStruc	g_TFStatus[N_CHANNELS_ON_BOARD_PLUS_SPH];

// TF control data
static TFControlStruc	g_TFControlData[N_CHANNELS_ON_BOARD_PLUS_SPH];

// TF payload data
static TFPayloadStruc	g_TFPayloadDataTx[N_CHANNELS_ON_BOARD];
static TFPayloadStruc	g_TFPayloadDataRx[N_CHANNELS_ON_BOARD_PLUS_SPH];
static TFPayloadStruc	g_TFPayloadDataRxHold[N_CHANNELS_ON_BOARD_PLUS_SPH];

// 4-wire interface buffer pointers
extern INT16 *g_pTxBuffer4wIf;
extern INT16 *g_pRxBuffer4wIf;

static INT16 *g_pTxBuffer4wIfCh[N_CHANNELS_ON_BOARD];
static INT16 *g_pRxBuffer4wIfCh[N_CHANNELS_ON_BOARD];

// temp buffer for 4 wire samples
static INT16 g_4wTempBufTx[N_CHANNELS_ON_BOARD][BUFFER_SIZE_2W_IF];
static INT16 g_4wTempBufRx[N_CHANNELS_ON_BOARD][BUFFER_SIZE_2W_IF];

// 2-wire interface buffer pointers
extern INT16 *g_pTxBuffer2wIf; 
extern INT16 *g_pRxBuffer2wIf;

static INT16 *g_pTxBuffer2wIfCh[N_CHANNELS_ON_BOARD_PLUS_SPH];
static INT16 *g_pRxBuffer2wIfCh[N_CHANNELS_ON_BOARD_PLUS_SPH];

// pointers to e- and m-wire data
static t_mWireData *g_pEWireBufferCh[N_CHANNELS_ON_BOARD];
static t_mWireData *g_pMWireBufferCh[N_CHANNELS_ON_BOARD];

// subframe buffer
static t_Subframe g_txSubframesCh1[3];
static t_RxFrameForDecoder g_rxSubframes[N_CHANNELS_ON_BOARD];

// interface configurations
static t_InterfaceCfg g_2wireCfg;
static t_InterfaceCfg g_4wireCfg;

static bool g_channelOn[N_CHANNELS_ON_BOARD];

static bool g_isCh2Wire[N_CHANNELS_ON_BOARD_PLUS_SPH];

// service phone
static bool g_servicePhoneActive;
static bool g_interfaceCh1Is2wire;

// cp tones
static t_CpTone g_cpTone[N_CHANNELS_ON_BOARD_PLUS_SPH];

// status
static t_AppStatus g_status;

// remote DTMF monitor
static t_DTMFMonitor g_remoteDTMFMonitor[N_CHANNELS_ON_BOARD];

// for testing
static UWORD g_payloadDataType;
static UBYTE g_payloadFirsByte;


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTION DEFINITIONS ************************************************/

static void setCPToneCfg(TFConfigStruc *pTFCfg);

static inline calc2WireIfBufferPtrsTx(void);
static inline calc2WireIfBufferPtrsRx(void);
static inline calc4WireIfBufferPtrsTx(void);
static inline calc4WireIfBufferPtrsRx(void);

static inline calcEWireBufferPtrs(void);
static inline calcMWireBufferPtrs(void);

static void setFaxState(UINT16 chNr);

static bool isLocalFaxToneDetected(TFStatusStruc *pStatus);
static bool isRemoteFaxToneDetected(TFStatusStruc *pStatus);
static bool isFaxEndOrErrorState(TFStatusStruc *pStatus);

static genCpTone(TFPayloadStruc *pPayLoad, t_CpTone *pCpTone);

static void muteOrCpTone(UINT16 chNr);
static void decodeFrame(UINT16 chNr);

static void initRemoteDTMFMonitor(void);
static void monitorRemoteDTMFDet(UINT16 chNr);
static bool isRemoteDTMFToneDetected(TFStatusStruc *pStatus);
static void enableLocalDTMFDet(UINT16 chNr);
static void disableLocalDTMFDet(UINT16 chNr);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		app_init()	
  Description:	Initialises application relevant functions and data structures.
  Inputs:		Pointer to configuration data.
  Outputs:
  Return:       	
  Globals:
  Important:
  
*******************************************************************************/

void app_init(t_O4CVConfig *pConfig)
{
    UINT16 i;
    UINT16 dataRate;
    UWORD err;
    
    
    // init status
    for (i = 0; i < N_CHANNELS_ON_BOARD_PLUS_SPH; i++)
    {
        g_status.initFailure[i] = false;
        g_status.runtimeFailure[i] = false;
    }

    // get data rate
    dataRate = configuration_getDataRate();
    
    // init voice channels 1 - 4
    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    {
	    // init TF relay
		g_pTFDataMem[i] = (UWORD*)&g_TFDataMem[i];
	    err = TF_Init(g_pTFDataMem[i]);
	    
	    if (err)
	    {
	    	g_status.initFailure[i] = true;
	    } 
    
	    // set CP tone config. data
		setCPToneCfg(&g_TFConfigData[i]);
		
		// input/ output filter
		if (pConfig->channel[i].settings & CH_INPUT_OUTPUT_FILTER_EN)
		{
		    g_TFConfigData[i].HPFilter = ON;
		    g_TFConfigData[i].PostFilter = ON;
		}
		else
		{
		    g_TFConfigData[i].HPFilter = OFF;
		    g_TFConfigData[i].PostFilter = OFF;
		}
		
		g_TFConfigData[i].DelayBuf = OFF;
		
		// SW LEC settings
		g_TFConfigData[i].PureDelay = PURE_DELAY_LEN;
		g_TFConfigData[i].pPureDelayBuf = g_pureDelayBuf[i];
		
		// config TF relay
		err = TF_Config(g_pTFDataMem[i], &g_TFConfigData[i]);
		
		if (err)
	    {
	    	g_status.initFailure[i] = true;
	    } 
		
		// set control data
		g_TFControlData[i].StateChange = 0;
		g_TFControlData[i].TFCmd = VOICE_CMD;
		g_TFControlData[i].VAGCState = OFF;
		g_TFControlData[i].CPToneDetState = ON;
		g_TFControlData[i].CPToneGenState = ON;
		g_TFControlData[i].DTMFDetState = ON;
		g_TFControlData[i].DTMFGenState = ON;
		
		if(configuration_isLecEnabled(i))
		{
		    g_TFControlData[i].LECState = ON;
		}
		else
		{
		    g_TFControlData[i].LECState = OFF;
		}
		
		switch(dataRate)
		{
		    case CH_VOICE_DR_5300:
		    
		    	g_TFControlData[i].CodecState = SET_VOCODER_53;
		    	break;
		    	
		    case CH_VOICE_DR_6300:
		    
		    	g_TFControlData[i].CodecState = SET_VOCODER_63;
		    	break;
		    	
		    default:
		    
		    	g_TFControlData[i].CodecState = OFF;
		}
		
		if (configuration_isChannelOn(i))
		{
		    g_channelOn[i] = true;
		}
		else
		{
			g_channelOn[i] = false;
			g_TFControlData[i].CodecState = OFF;
		}
		
		g_TFControlData[i].StateChange = ALL_STATES_CHANGE;
		
		err = TF_Control(g_pTFDataMem[i], &g_TFControlData[i]);
		
		if (err)
	    {
	    	g_status.initFailure[i] = true;
	    } 
    }
    
    // init service phone
    {
    	// init TF relay
		g_pTFDataMem[SPH] = (UWORD*)&g_TFDataMem[SPH];
	    err = TF_Init(g_pTFDataMem[SPH]);
	    
	    if (err)
	    {
	    	g_status.initFailure[SPH] = true;
	    } 
	    
	    // set CP tone config. data
		setCPToneCfg(&g_TFConfigData[SPH]);
    
	    // set config data
		g_TFConfigData[SPH].HPFilter = ON;
		g_TFConfigData[SPH].PostFilter = ON;
		g_TFConfigData[SPH].DelayBuf = OFF;
		g_TFConfigData[SPH].PureDelay = 0;
		g_TFConfigData[SPH].pPureDelayBuf = 0;
		
		// config TF relay
		err = TF_Config(g_pTFDataMem[SPH], &g_TFConfigData[SPH]);
		
		if (err)
	    {
	    	g_status.initFailure[SPH] = true;
	    } 
		
		// set control data
		g_TFControlData[SPH].StateChange = 0;
		g_TFControlData[SPH].TFCmd = VOICE_CMD;
		g_TFControlData[SPH].LECState = OFF;
		g_TFControlData[SPH].VAGCState = OFF;
		g_TFControlData[SPH].CPToneDetState = OFF;
		g_TFControlData[SPH].CPToneGenState = ON;
		g_TFControlData[SPH].DTMFDetState = OFF;
		g_TFControlData[SPH].DTMFGenState = OFF;
		
		if (configuration_isServicePhoneActive())
		{
			switch(dataRate)
			{
		
			    case CH_VOICE_DR_5300:
		    
			    	g_TFControlData[SPH].CodecState = SET_VOCODER_53;
			    	break;
		    	
			    case CH_VOICE_DR_6300:
		    
			    	g_TFControlData[SPH].CodecState = SET_VOCODER_63;
			    	break;
		    	
			    default:
		    
			    	g_TFControlData[SPH].CodecState = OFF;
			}
		}
		else
		{
			g_TFControlData[SPH].CodecState = OFF;  
		}
		
		g_TFControlData[SPH].StateChange = ALL_STATES_CHANGE;
		
		err = TF_Control(g_pTFDataMem[SPH], &g_TFControlData[SPH]);
		
		if (err)
	    {
	    	g_status.initFailure[SPH] = true;
	    } 
    }
    
    // get interface type of ch1
    g_interfaceCh1Is2wire = configuration_is2wireInterface(CH1);
	
    // initialize subframe packing/unpacking
    subframe_init(dataRate);
    
    // initialize interpolation/ decimation for 4-wire interface
    intDec_init();
    
    // initialize speech filters for 4-wire interfaces
    speechFilter_init(pConfig);
    
    // get configuration of activated interfaces
    configuration_get2wireCfg(&g_2wireCfg);
    configuration_get4wireCfg(&g_4wireCfg);
    
	// set channel type
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
	{
		if ( configuration_is2wireConfigured(i) )
			g_isCh2Wire[i] = true;
		else
			g_isCh2Wire[i] = false;
	}	
	g_isCh2Wire[SPH] = true;

	// initialize remote DTMF monitor
	initRemoteDTMFMonitor();
}


static void setCPToneCfg(TFConfigStruc *pTFCfg)
{
	    // set number of CP tone frequencies
	    pTFCfg->CPDef.num_freqs = N_FAX_CP_TONES + N_VOICE_CP_TONES;
    
	    // set CP tone frequencies
	    pTFCfg->CPDef.freq[CNG_TONE_IX] = CNG_TONE_FREQ;
	    pTFCfg->CPDef.freq[CED_TONE_IX] = CED_TONE_FREQ;
	    pTFCfg->CPDef.freq[DIAL_TONE_IX] = DIAL_TONE_FREQ;
	    
	    // set number of CP tones
		pTFCfg->CPDef.num_tones = N_FAX_CP_TONES + N_VOICE_CP_TONES;
		
		// set CP tones configuration
		pTFCfg->CPDef.cp_tone[CNG_TONE_IX].freq1_ind = CNG_TONE_IX; 
		pTFCfg->CPDef.cp_tone[CNG_TONE_IX].freq2_ind = FREQ_OFF;
		pTFCfg->CPDef.cp_tone[CNG_TONE_IX].t_on1  = CNG_T_ON1;
		pTFCfg->CPDef.cp_tone[CNG_TONE_IX].t_off1 = CNG_T_OFF1;
		pTFCfg->CPDef.cp_tone[CNG_TONE_IX].t_on2  = CNG_T_ON2;
		pTFCfg->CPDef.cp_tone[CNG_TONE_IX].t_off2 = CNG_T_OFF2;
		
		pTFCfg->CPDef.cp_tone[CED_TONE_IX].freq1_ind = CED_TONE_IX; 
		pTFCfg->CPDef.cp_tone[CED_TONE_IX].freq2_ind = FREQ_OFF;
		pTFCfg->CPDef.cp_tone[CED_TONE_IX].t_on1  = CED_T_ON1;
		pTFCfg->CPDef.cp_tone[CED_TONE_IX].t_off1 = CED_T_OFF1;
		pTFCfg->CPDef.cp_tone[CED_TONE_IX].t_on2  = CED_T_ON2;
		pTFCfg->CPDef.cp_tone[CED_TONE_IX].t_off2 = CED_T_OFF2;
		
		pTFCfg->CPDef.cp_tone[DIAL_TONE_IX].freq1_ind = DIAL_TONE_IX; 
		pTFCfg->CPDef.cp_tone[DIAL_TONE_IX].freq2_ind = FREQ_OFF;
		pTFCfg->CPDef.cp_tone[DIAL_TONE_IX].t_on1  = DIAL_T_ON1;
		pTFCfg->CPDef.cp_tone[DIAL_TONE_IX].t_off1 = DIAL_T_OFF1;
		pTFCfg->CPDef.cp_tone[DIAL_TONE_IX].t_on2  = DIAL_T_ON2;
		pTFCfg->CPDef.cp_tone[DIAL_TONE_IX].t_off2 = DIAL_T_OFF2;
}


/*****************************************************************************
  Function:		app_encode()	
  Description:	Performs voice data encoding of all channels.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:
  
*******************************************************************************/

void app_encode(void)
{
    static UINT16 i;
    static UINT16 startIx2w, startIx4w;
	UWORD err;    
    
    
    // calculate 2-wire channel ptrs
    calc2WireIfBufferPtrsRx();
    
    // calculate 4-wire channel ptrs
    calc4WireIfBufferPtrsRx();
    
    // calculate m-wire buffer ptrs
    calcMWireBufferPtrs();
    
    // process service phone input samples
    if (g_servicePhoneActive)
    {
        if (g_interfaceCh1Is2wire)
        {
        	startIx2w = 1;	// do not process 2-wire samples on ch1
        	startIx4w = 0;
        }
        else	// interface ch1 is 4-wire
        {
       		startIx2w = 0;
        	startIx4w = 1;	// do not process 4-wire samples on ch1
        }
        
        // digital gain
        gain_apply(g_pRxBuffer2wIfCh[SPH], SPH, gain_Rx);    
        
    	// encode samples
	    err = TF_Encode(g_pTFDataMem[SPH], g_pRxBuffer2wIfCh[SPH], &g_TFPayloadDataTx[CH1]);
	    TF_Packaging(&g_TFPayloadDataTx[CH1]);
	    
	    if (err)
	    {
	        g_status.runtimeFailure[SPH] = true;
	    }
    
	    // pack subframes
	    subframe_pack(&g_TFPayloadDataTx[CH1], g_pMWireBufferCh[CH1], g_pSubframeBufferTxCh[CH1]);
    }
    else
    {
    	startIx2w = 0;
        startIx4w = 0;    
    } 
    
    // process 2-wire input samples
    for (i = startIx2w; i < g_2wireCfg.nIf; i++)
    {
        // digital gain
        gain_apply(g_pRxBuffer2wIfCh[g_2wireCfg.ix[i]], g_2wireCfg.ix[i], gain_Rx);
        
	    // encode samples
	    err = TF_Encode(g_pTFDataMem[g_2wireCfg.ix[i]], g_pRxBuffer2wIfCh[g_2wireCfg.ix[i]], &g_TFPayloadDataTx[g_2wireCfg.ix[i]]);
	    TF_Packaging(&g_TFPayloadDataTx[g_2wireCfg.ix[i]]);
	    
	    if (err)
	    {
	        g_status.runtimeFailure[g_2wireCfg.ix[i]] = true;
	    }
    
	    // pack subframes
	    subframe_pack(&g_TFPayloadDataTx[g_2wireCfg.ix[i]], g_pMWireBufferCh[g_2wireCfg.ix[i]], g_pSubframeBufferTxCh[g_2wireCfg.ix[i]]);
    } 
    
    // process 4 wire input samples
    for (i = startIx4w; i < g_4wireCfg.nIf; i++)
    {
	    // decimate 4-wire input samples from 64 to 8 kSps
	    intDec_decimate(g_pRxBuffer4wIfCh[g_4wireCfg.ix[i]], &g_4wTempBufTx[g_4wireCfg.ix[i]][0], g_4wireCfg.ix[i]);
	    
	    // digital gain
	    gain_apply(&g_4wTempBufTx[g_4wireCfg.ix[i]][0], g_4wireCfg.ix[i], gain_Rx);
	    
	    // speech filter
	    speechFilter_apply(&g_4wTempBufTx[g_4wireCfg.ix[i]][0], &g_4wTempBufTx[g_4wireCfg.ix[i]][0], g_4wireCfg.ix[i], speechFilter_Tx);
    
	    // encode samples
	    err = TF_Encode(g_pTFDataMem[g_4wireCfg.ix[i]], &g_4wTempBufTx[g_4wireCfg.ix[i]][0], &g_TFPayloadDataTx[g_4wireCfg.ix[i]]);
	    TF_Packaging(&g_TFPayloadDataTx[g_4wireCfg.ix[i]]);
	    
	    if (err)
	    {
	        g_status.runtimeFailure[g_4wireCfg.ix[i]] = true;
	    }
    
	    // pack subframes
	    subframe_pack(&g_TFPayloadDataTx[g_4wireCfg.ix[i]], g_pMWireBufferCh[g_4wireCfg.ix[i]], g_pSubframeBufferTxCh[g_4wireCfg.ix[i]]);
    }
    
        
}


/*****************************************************************************
  Function:		app_decode()	
  Description:	Performs voice data decoding of all channels.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:
  
******************************************************************************/
void app_decode(void)
{
    static UINT16 i;
    static UINT16 startIx2w, startIx4w;
	UINT16 chNr;
    
    // calculate 2-wire channel ptrs
    calc2WireIfBufferPtrsTx();
    
	// calculate 4-wire channel ptrs
    calc4WireIfBufferPtrsTx();
    
    // calculate e-wire buffer ptrs
    calcEWireBufferPtrs();
    
    if (g_receiverInSync)
    {
		// receiver is in sync 

        voiceData_getRxFrames(g_rxSubframes);
        
		///////////////////////////
        // process service phone //
		///////////////////////////
    	if (g_servicePhoneActive)
    	{
			// set start indices for 2- and 4-wire processing
    	    if (g_interfaceCh1Is2wire)
	        {
	        	startIx2w = 1;	// do not process 2-wire samples on ch1
	        	startIx4w = 0;
	        }
	        else	// interface ch1 is 4-wire
	        {
	       		startIx2w = 0;
	        	startIx4w = 1;	// do not process 4-wire samples on ch1
	        }       
    	    
			if ( g_rxSubframes[CH1].ready )
			{
				// CH1 is currently ON due to the FB/FF profile
				// decode frame
				decodeFrame(SPH);
			}
			else
			{
				// CH1 is currently OFF due to the FB/FF profile
				// mute or CP tone SPH
				muteOrCpTone(SPH); 
			}
    	}
    	else
	    {
			// service phone is not activated

			// set start indices for 2- and 4-wire processing
	    	startIx2w = 0;
	        startIx4w = 0;

			// mute or CP tone SPH
	     	muteOrCpTone(SPH); 
	    } 
        
		/////////////////////////////
        // process 2-wire channels //
		/////////////////////////////
        for (i = startIx2w; i < g_2wireCfg.nIf; i++)
    	{
			chNr = g_2wireCfg.ix[i];
			if ( g_rxSubframes[chNr].ready )
			{
				// channel is currently ON due to the FB/FF profile
				// decode frame
				decodeFrame(chNr);
				
				monitorRemoteDTMFDet(chNr);
			}
			else
			{
				// channel is currently OFF due to the FB/FF profile
				// mute or CP tone channel
				muteOrCpTone(chNr); 
			}
    	}
        
		/////////////////////////////
        // process 4-wire channels //
		/////////////////////////////
        for (i = startIx4w; i < g_4wireCfg.nIf; i++)
    	{
			chNr = g_4wireCfg.ix[i];
			if ( g_rxSubframes[chNr].ready )
			{
				// channel is currently ON due to the FB/FF profile
				// decode frame
				decodeFrame(chNr);
			}
			else
			{
				// channel is currently OFF due to the FB/FF profile
				// mute or CP tone channel
				muteOrCpTone(chNr); 
			}
    	}
    }
    else
    {
		// receiver is out of sync
		// mute or CP tone all channels and service phone
		muteOrCpTone(ALL);
    }
}

/*****************************************************************************
  Function:		static void decodeFrame(UINT16 chNr)
  Description:	decodeFrame() performs the unpack/interolate/hold actions and
  				decodes the frame.
  Inputs:		UINT16 chNr: channel number (0-3 CH1-4, 4 SPH)
  Outputs:		None
  Return:       None	
  Globals:		multiple	
  Important:	-
******************************************************************************/
static void decodeFrame(UINT16 chNr)
{
	UINT16 chNrOnLink;
    UWORD err;

	if ( chNr == SPH )
		chNrOnLink = CH1;
	else
		chNrOnLink = chNr;
	
	// setup g_TFPayloadDataRx
	switch ( g_rxSubframes[chNrOnLink].appAction )
	{
		case e_Unpack:
			subframe_unpack(&g_rxSubframes[chNrOnLink], &g_TFPayloadDataRx[chNr], g_pEWireBufferCh[chNrOnLink]);
			TF_UnPackage(&g_TFPayloadDataRx[chNr]);
			break;

		case e_Interpolate:
				memcpy(&g_TFPayloadDataRx[chNr], &payLoadBadVoice, sizeof(TFPayloadStruc));
			break;

		case e_Hold:
				memcpy(&g_TFPayloadDataRx[chNr], &g_TFPayloadDataRxHold[chNr], sizeof(TFPayloadStruc));
			break;

		default:
			assert(false);
	}

	// check if cp tone has to be generated
	if (g_cpTone[chNr].active)
		genCpTone(&g_TFPayloadDataRx[chNr], &g_cpTone[chNr]);

	// hold old frame
	memcpy(&g_TFPayloadDataRxHold[chNr], &g_TFPayloadDataRx[chNr], sizeof(TFPayloadStruc));

	if ( g_isCh2Wire[chNr] )
	{
		// decode voice frame
		err = TF_Decode(g_pTFDataMem[chNr], &g_TFPayloadDataRx[chNr], g_pTxBuffer2wIfCh[chNr]);
		if (err)
			g_status.runtimeFailure[chNr] = true;

		// digital gain
		gain_apply(g_pTxBuffer2wIfCh[chNr], chNr, gain_Tx);   
	}
	else
	{
		// decode voice frame
		err = TF_Decode(g_pTFDataMem[chNr], &g_TFPayloadDataRx[chNr], &g_4wTempBufRx[chNr][0]);
		if (err)
			g_status.runtimeFailure[chNr] = true;

		// speech filter
		speechFilter_apply(&g_4wTempBufRx[chNr][0], &g_4wTempBufRx[chNr][0], chNr, speechFilter_Rx);

		// digital gain
		gain_apply(&g_4wTempBufRx[chNr][0], chNr, gain_Tx);

		// interpolate voice samples from 8 to 64 kSps
		intDec_interpolate(&g_4wTempBufRx[chNr][0], g_pTxBuffer4wIfCh[chNr], chNr);
	}
}

/*****************************************************************************
  Function:		static void muteOrCpTone(UINT16 chNr)
  Description:	muteOrCpTone() mutes a channel or generates CP tomes for the
  				channel
  Inputs:		UINT16 chNr: channel number (0-3: CH1-4, 4: SPH, 5: CH1-4 and SPH)
  Outputs:		None
  Return:       None	
  Globals:		multiple	
  Important:	-
******************************************************************************/
static void muteOrCpTone(UINT16 chNr)
{
	UINT16 i;
	UINT16 channel;
    UWORD err;

	// mute or CP tone service phone
	if ( chNr == SPH || chNr == ALL )
	{
		if (g_cpTone[SPH].active)
		{
			// generate CP tone
			genCpTone(&g_TFPayloadDataRx[SPH], &g_cpTone[SPH]);

			// decode payload
			err = TF_Decode(g_pTFDataMem[SPH], &g_TFPayloadDataRx[SPH], g_pTxBuffer2wIfCh[SPH]);
			if (err)
				g_status.runtimeFailure[SPH] = true;

			// digital gain
			gain_apply(g_pTxBuffer2wIfCh[SPH], SPH, gain_Tx);   
		}
		else
			memset(g_pTxBuffer2wIfCh[SPH], 0, BUFFER_SIZE_2W_IF*2);	// size in #bytes
	}

	// mute or CP tone 2-wire channels
	for (i=0; i<g_2wireCfg.nIf; i++)
	{
		channel = g_2wireCfg.ix[i];
		if ( chNr == channel || chNr == ALL)
		{
		    // check if CP tone has to be generated
		    if (g_cpTone[channel].active)
		    {
		    	genCpTone(&g_TFPayloadDataRx[channel], &g_cpTone[channel]);
		    
			    // decode payload
				err = TF_Decode(g_pTFDataMem[channel], &g_TFPayloadDataRx[channel], g_pTxBuffer2wIfCh[channel]);
				if (err)
			        g_status.runtimeFailure[channel] = true;
			
				// digital gain
				gain_apply(g_pTxBuffer2wIfCh[channel], channel, gain_Tx);
		    }
			else
				memset(g_pTxBuffer2wIfCh[channel], 0, BUFFER_SIZE_2W_IF*2);	// size in #bytes
		}
	}
	
	// mute 4-wire channels (no CP tones on 4-wire channels)
	for (i = 0; i < g_4wireCfg.nIf; i++)
	{
		channel = g_4wireCfg.ix[i];
		if ( chNr == channel || chNr == ALL )
			memset(g_pTxBuffer4wIfCh[channel], 0, BUFFER_SIZE_4W_IF*2);	// size in #bytes
	}
}

/*****************************************************************************
  Function:		app_faxControl()	
  Description:	Switches the local relay to fax mode in case of detected fax
  				tones (local or remote) and back to voice mode in case of fax
  				status End or Error.
  Inputs:
  Outputs:
  Return:       	
  Globals:		
  Important:
  
******************************************************************************/

void app_faxControl(void)
{
    UINT16 i;
    UWORD err;
    
    
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    {
        if (g_channelOn[i])
        {
	        // get relay status
	        err = TF_Status(g_pTFDataMem[i], &g_TFStatus[i]);
	        
	        if (err)
	        {
	            g_status.runtimeFailure[i] = true;
	        }
        
	        switch(g_TFStatus[i].TFState)
	        {
	            case VOICE_STATE:
	            
	            	if (isLocalFaxToneDetected(&g_TFStatus[i]) || isRemoteFaxToneDetected(&g_TFStatus[i]))
	            	{
	            		setFaxState(i);   	  
	            	}
	            
	            	break;
	            	
				case FAX_STATE:
				
					if (isFaxEndOrErrorState(&g_TFStatus[i]))
					{
					    app_setVoiceState(i);
					}
				
					break;
					
				default:
				
					assert(false);
					app_setVoiceState(i);
				
	        }
        }
    } 
}


static bool isLocalFaxToneDetected(TFStatusStruc *pStatus)
{
	if ((pStatus->StateChange & CP_LOCAL_CHANGE) &&
		((pStatus->CPLocalDet == CNG_TONE_IX) ||			
		 (pStatus->CPLocalDet == CED_TONE_IX)))
	{
	    return(true);
	}
	else
	{
	    return(false);
	} 
}


static bool isRemoteFaxToneDetected(TFStatusStruc *pStatus)
{
	if ((pStatus->StateChange & CP_REMOTE_CHANGE) &&
		((pStatus->CPRemoteDet == CNG_TONE_IX) ||			
		 (pStatus->CPRemoteDet == CED_TONE_IX)))
	{
	    return(true);
	}
	else
	{
	    return(false);
	} 
}


static bool isFaxEndOrErrorState(TFStatusStruc *pStatus)
{
    if ((pStatus->StateChange & FAXSTATE_CHANGE) &&
    	((pStatus->FaxState == FAX_END_STATE) ||
    	 (pStatus->FaxState == FAX_ERROR_STATE)))
    {	 
		return(true);
    }
    else 
    {
        return(false);
    }
}


static void setFaxState(UINT16 chNr)
{
    UWORD err;
    
	// set relay to fax state
	g_TFControlData[chNr].TFCmd = FAX_CMD;
	g_TFControlData[chNr].StateChange = TFCMD_CHANGE;
	
	// disable SW LEC in fax state
	if(configuration_isLecEnabled(chNr))
	{
		g_TFControlData[chNr].LECState = OFF;
		g_TFControlData[chNr].StateChange |= LEC_CHANGE;
	}
	
	err = TF_Control(g_pTFDataMem[chNr], &g_TFControlData[chNr]);
	
	if (err)
    {
        g_status.runtimeFailure[chNr] = true;
    }
	
	// switch off LEC
	DuSlic_SpiWriteFIFO(SLIC_STOP_LEC, chNr);
}
    

/*****************************************************************************
  Function:		app_setVoiceState()	
  Description:	This function switches the TF relay to voice state.
  Inputs:		chNr: channel number (0-3)		
  Outputs:
  Return:       	
  Globals:		g_TFControlData, g_pTFDataMem, g_status
  Important:
  
******************************************************************************/

void app_setVoiceState(UINT16 chNr)
{
    UWORD err;
    
    // set relay to voice state
    g_TFControlData[chNr].TFCmd = VOICE_CMD;
	g_TFControlData[chNr].StateChange = TFCMD_CHANGE;
	
	// enable SW LEC in voice state
	if(configuration_isLecEnabled(chNr))
	{
		g_TFControlData[chNr].LECState = ON;
		g_TFControlData[chNr].StateChange |= LEC_CHANGE;
	}
	
	err = TF_Control(g_pTFDataMem[chNr], &g_TFControlData[chNr]);
	
	if (err)
    {
        g_status.runtimeFailure[chNr] = true;
    }
	
	// switch on LEC
	DuSlic_SpiWriteFIFO(SLIC_START_LEC, chNr);
}


/*****************************************************************************
  Function:		app_activateServicePhone()	
  Description:	Activates encoding and decoding of service phone samples.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_servicePhoneActive
  Important:
  
******************************************************************************/

void app_activateServicePhone(void)
{
	g_servicePhoneActive = true;  
}


/*****************************************************************************
  Function:		app_deactivateServicePhone()	
  Description:	Dectivates encoding and decoding of service phone samples.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_servicePhoneActive
  Important:
  
******************************************************************************/

void app_deactivateServicePhone(void)
{
	g_servicePhoneActive = false;  
}


/*****************************************************************************
  Function:		app_activateRingingTone()	
  Description:	Activates the ringing tone on the specified output channel.	
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:
  Return:       	
  Globals:		g_cpTone
  Important:
  
******************************************************************************/

void app_activateRingingTone(UINT16 chNr)
{
    g_cpTone[chNr].active = true;
    g_cpTone[chNr].onTime = RINGING_TONE_T_ON;
    g_cpTone[chNr].offTime = RINGING_TONE_T_OFF;
    g_cpTone[chNr].timer = 0;
    g_cpTone[chNr].state = e_ToneOn;
    
}


/*****************************************************************************
  Function:		app_activateBusyTone()	
  Description:	Activates the busy tone on the specified output channel.	
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:
  Return:       	
  Globals:		g_cpTone
  Important:
  
******************************************************************************/

void app_activateBusyTone(UINT16 chNr)
{
	g_cpTone[chNr].active = true;
    g_cpTone[chNr].onTime = BUSY_TONE_T_ON;
    g_cpTone[chNr].offTime = BUSY_TONE_T_OFF;
    g_cpTone[chNr].timer = 0;
    g_cpTone[chNr].state = e_ToneOn;
}


/*****************************************************************************
  Function:		app_deactivateCpTone()	
  Description:	Deactivates the cp tone (ringing or busy tone) on the specified
  				output channel.	
  Inputs:		chNr: channel number (0-3, 4: service phone)
  Outputs:
  Return:       	
  Globals:		g_cpTone
  Important:
  
******************************************************************************/

void app_deactivateCpTone(UINT16 chNr)
{
	g_cpTone[chNr].active = false;   
}


/*****************************************************************************
  Function:		app_getTFDataMemPtrs()	
  Description:	Returns a pointer to the array of TFDataMem pointers 
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_pTFDataMem
  Important:
  
******************************************************************************/

UINT16 **app_getTFDataMemPtrs(void)
{
    return(g_pTFDataMem);
}


/*****************************************************************************
  Function:		t_RxFrameForDecoder* app_getFrameForDecoderPtr(void)
  Description:	app_getFrameForDecoderPtr() returns a pointer to the 
  				frames for decoder data structure (g_rxSubframes).
  Inputs:
  Outputs:
  Return:		t_RxFrameForDecoder*: Pointer to frames from decoder data structre	
  Globals:		g_rxSubframes
  Important:
  
******************************************************************************/

t_RxFrameForDecoder* app_getFrameForDecoderPtr(void)
{
	return g_rxSubframes;
}

/*****************************************************************************
  Function:		app_getStatusPtr()	
  Description:	Returns a pointer to the app status.
  Inputs:
  Outputs:
  Return:       	
  Globals:		g_status
  Important:
  
******************************************************************************/

t_AppStatus *app_getStatusPtr(void)
{
    return(&g_status);
}


/*****************************************************************************
  Function:		calc4WireIfBufferPtrsTx()	
  Description:	Calculates pointers to 4-wire tx channel buffers
  Inputs:		-
  Outputs:		-
  Return:       -	
  Globals:		g_pTxBuffer4wIf, g_pTxBuffer4wIfCh
  Important:	-
  
*******************************************************************************/

static inline calc4WireIfBufferPtrsTx(void)
{
	g_pTxBuffer4wIfCh[CH1] = g_pTxBuffer4wIf + BUFFER_SIZE_4W_IF*3;
	g_pTxBuffer4wIfCh[CH2] = g_pTxBuffer4wIf + BUFFER_SIZE_4W_IF*2;
	g_pTxBuffer4wIfCh[CH3] = g_pTxBuffer4wIf + BUFFER_SIZE_4W_IF;
	g_pTxBuffer4wIfCh[CH4] = g_pTxBuffer4wIf;
	
}


/*****************************************************************************
  Function:		calc4WireIfBufferPtrsRx()	
  Description:	Calculates pointers to 4-wire rx channel buffers
  Inputs:		-
  Outputs:		-
  Return:       -	
  Globals:		g_pRxBuffer4wIf, g_pRxBuffer4wIfCh
  Important:	-
  
*******************************************************************************/

static inline calc4WireIfBufferPtrsRx(void)
{
	g_pRxBuffer4wIfCh[CH1] = g_pRxBuffer4wIf + BUFFER_SIZE_4W_IF*3;
	g_pRxBuffer4wIfCh[CH2] = g_pRxBuffer4wIf + BUFFER_SIZE_4W_IF*2;
	g_pRxBuffer4wIfCh[CH3] = g_pRxBuffer4wIf + BUFFER_SIZE_4W_IF;
	g_pRxBuffer4wIfCh[CH4] = g_pRxBuffer4wIf;
	
}


/*****************************************************************************
  Function:		calc2WireIfBufferPtrsTx()	
  Description:	Calculates pointers to 2-wire tx channel buffers
  Inputs:		-
  Outputs:		-
  Return:       -	
  Globals:		g_pTxBuffer2wIf, g_pTxBuffer2wIfCh
  Important:	-
  
*******************************************************************************/

static inline calc2WireIfBufferPtrsTx(void)
{
	g_pTxBuffer2wIfCh[CH1] = g_pTxBuffer2wIf;
	g_pTxBuffer2wIfCh[CH2] = g_pTxBuffer2wIf + BUFFER_SIZE_2W_IF;
	g_pTxBuffer2wIfCh[CH3] = g_pTxBuffer2wIf + BUFFER_SIZE_2W_IF*2;
	g_pTxBuffer2wIfCh[CH4] = g_pTxBuffer2wIf + BUFFER_SIZE_2W_IF*3;
	g_pTxBuffer2wIfCh[SPH] = g_pTxBuffer2wIf + BUFFER_SIZE_2W_IF*4;
	
}


/*****************************************************************************
  Function:		calc2WireIfBufferPtrsTx()	
  Description:	Calculates pointers to 2-wire tx channel buffers
  Inputs:		-
  Outputs:		-
  Return:       -	
  Globals:		g_pTxBuffer2wIf, g_pTxBuffer2wIfCh
  Important:	-
  
*******************************************************************************/

static inline calc2WireIfBufferPtrsRx(void)
{
	g_pRxBuffer2wIfCh[CH1] = g_pRxBuffer2wIf;
	g_pRxBuffer2wIfCh[CH2] = g_pRxBuffer2wIf + BUFFER_SIZE_2W_IF;
	g_pRxBuffer2wIfCh[CH3] = g_pRxBuffer2wIf + BUFFER_SIZE_2W_IF*2;
	g_pRxBuffer2wIfCh[CH4] = g_pRxBuffer2wIf + BUFFER_SIZE_2W_IF*3;
	g_pRxBuffer2wIfCh[SPH] = g_pRxBuffer2wIf + BUFFER_SIZE_2W_IF*4;
	
}


/*****************************************************************************
  Function:		calcMWireBufferPtrs()	
  Description:	Calculates pointers to m-wire channel buffers.
  Inputs:		-
  Outputs:		-
  Return:       -	
  Globals:		g_pMWireBuffer, g_pMWireBufferCh
  Important:	-
  
*******************************************************************************/

static inline calcMWireBufferPtrs(void)
{
	g_pMWireBufferCh[CH1] = g_pMWireBuffer;
	g_pMWireBufferCh[CH2] = g_pMWireBuffer + 1;
	g_pMWireBufferCh[CH3] = g_pMWireBuffer + 2;
	g_pMWireBufferCh[CH4] = g_pMWireBuffer + 3;	
        
}


/*****************************************************************************
  Function:		calcEWireBufferPtrs()	
  Description:	Calculates pointers to e-wire channel buffers.
  Inputs:		-
  Outputs:		-
  Return:       -	
  Globals:		g_pEWireBuffer, g_pEWireBufferCh
  Important:	-
  
*******************************************************************************/

static inline calcEWireBufferPtrs(void)
{
	g_pEWireBufferCh[CH1] = g_pEWireBuffer;
	g_pEWireBufferCh[CH2] = g_pEWireBuffer + 1;
	g_pEWireBufferCh[CH3] = g_pEWireBuffer + 2;
	g_pEWireBufferCh[CH4] = g_pEWireBuffer + 3;   
    
}


/*****************************************************************************
  Function:		genCpTone()	
  Description:	Genrates the payload data for cp tones (ringing and busy tone).
  Inputs:		pPayLoad: pointer to payload data buffer
  				pCpTone: pointer to cp tone data
  Outputs:		cp tone payload data
  Return:       	
  Globals:		
  Important:	
  
*******************************************************************************/

static genCpTone(TFPayloadStruc *pPayLoad, t_CpTone *pCpTone)
{
    
    switch(pCpTone->state)
    {
        case e_ToneOn:
        	
        	memcpy(pPayLoad, &payLoadDialTone, sizeof(payLoadDialTone));
        	pCpTone->timer++;
        	
        	if (pCpTone->timer >= pCpTone->onTime)
        	{
        		pCpTone->timer = 0;
        		pCpTone->state = e_ToneOff;
        	}  
        	
        	break;
        	
		case e_ToneOff:
		
			memcpy(pPayLoad, &payLoadEmpty, sizeof(payLoadEmpty));
        	pCpTone->timer++;
        	
        	if (pCpTone->timer >= pCpTone->offTime)
        	{
        		pCpTone->timer = 0;
        		pCpTone->state = e_ToneOn;
        	}
		
			break;
			
		default:
		
			pCpTone->state = e_ToneOff;
    }
}


/*****************************************************************************
  Function:		monitorRemoteDTMFDet()	
  Description:	Monitors remote DTMF detector state. In case of active tones,
  				local DTMF detection is disabled. This function is useful at
  				startup when the line echo canceller is not yet in steady state.
  Inputs:		chNr: channel number
  				
  Outputs:		
  Return:       	
  Globals:		g_pTFDataMem, g_TFStatus, g_remoteDTMFMonitor		
  Important:	
  
*******************************************************************************/

static void monitorRemoteDTMFDet(UINT16 chNr)
{
    UWORD err;

    
    // get relay status
    err = TF_Status(g_pTFDataMem[chNr], &g_TFStatus[chNr]);
    
    if (err)
    {
        g_status.runtimeFailure[chNr] = true;
    }

    switch(g_remoteDTMFMonitor[chNr].state)
    {
    	case e_Idle:
    	
    		if (isRemoteDTMFToneDetected(&g_TFStatus[chNr]))
    		{
    			g_remoteDTMFMonitor[chNr].state = e_ToneDetected;
    			g_remoteDTMFMonitor[chNr].holdCounter = 0;
    			
    			disableLocalDTMFDet(chNr);
    		}  
    	
    		break;
    		
    	case e_ToneDetected:
    	
    		if (!isRemoteDTMFToneDetected(&g_TFStatus[chNr]))
    		{
    		    g_remoteDTMFMonitor[chNr].holdCounter++;
    		}
    		else
    		{
    		    g_remoteDTMFMonitor[chNr].holdCounter = 0;
    		}
    		
    		if (g_remoteDTMFMonitor[chNr].holdCounter >= REMOTE_DTMF_MONITOR_HOLD_TIME)
    		{
    			g_remoteDTMFMonitor[chNr].state =  e_Idle;
    			enableLocalDTMFDet(chNr);
    		}  
    	
    		break;
    		
    	default:
    	
    		g_remoteDTMFMonitor[chNr].state = e_Idle;
    }
}


static void initRemoteDTMFMonitor(void)
{
	UINT16 i;
	
	
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    {
        g_remoteDTMFMonitor[i].state = e_Idle;
    	g_remoteDTMFMonitor[i].holdCounter = 0;
    }
}
        

static bool isRemoteDTMFToneDetected(TFStatusStruc *pStatus)
{
	if ((pStatus->StateChange & DTMF_REMOTE_CHANGE) &&
		(pStatus->DTMFRemoteDet != 0xFF))
	{
	    return(true);
	}
	else
	{
	    return(false);
	} 
}


static void enableLocalDTMFDet(UINT16 chNr)
{
    UWORD err;
    
	// enable local DTMF detector
	g_TFControlData[chNr].DTMFDetState = CONTROL_ENABLE;
	g_TFControlData[chNr].StateChange = DTMF_DET_CHANGE;
	
	err = TF_Control(g_pTFDataMem[chNr], &g_TFControlData[chNr]);
	
	if (err)
    {
        g_status.runtimeFailure[chNr] = true;
    }
}


static void disableLocalDTMFDet(UINT16 chNr)
{
    UWORD err;
    
	// disable local DTMF detector
	g_TFControlData[chNr].DTMFDetState = CONTROL_DISABLE;
	g_TFControlData[chNr].StateChange = DTMF_DET_CHANGE;
	
	err = TF_Control(g_pTFDataMem[chNr], &g_TFControlData[chNr]);
	
	if (err)
    {
        g_status.runtimeFailure[chNr] = true;
    }
}

