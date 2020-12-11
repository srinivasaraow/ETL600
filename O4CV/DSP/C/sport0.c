/*-------------------------------------------------------------------------*
 * $Workfile: sport0.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : serial port functions
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/sport0.c $
 * 
 * 15    08-02-15 9:34 Chrobif
 * + sport0_isExtTdmBusRunning() added.
 * 
 * 14    29.11.07 17:03 Chhamaa
 * Extended time slots of board #4 moved to basic time slots 8 and 9.
 * 
 * 13    13.09.07 17:52 Chhamaa
 * Restart bug fixed.
 * 
 * 12    12.09.07 15:12 Chhamaa
 * Init, start and stop functions modified.
 * 
 * 11    7.09.07 17:34 Chhamaa
 * Bug fixed in calcExtendedTimeSlotMask().
 * 
 * 10    10.08.07 11:57 Chhamaa
 * Function sport0_stopExtTdmBus() added.
 * 
 * 9     2.07.07 10:53 Chhamaa
 * ISR assignment and enable moved to sport0_initExtTdmBus().
 * 
 * 8     8.06.07 16:22 Chhamaa
 * Tx DMA disabled in case of only one external TDM bus time slot.
 * 
 * 7     1.06.07 9:55 Chhamaa
 * initSport0DMA() modified.
 * 
 * 6     29.05.07 13:26 Chrobif
 * extTdmBus_ commented in.
 * 
 * 5     7.05.07 17:31 Chhamaa
 * extTdmBus_ commented out
 * 
 * 4     26.04.07 14:28 Chrobif
 * Module name externalTDMBusServices replaced with extTdmBus.
 * 
 * 3     20.04.07 15:05 Chhamaa
 * EX_REENTRANT_HANDLER used for sport0 ISR
 * 
 * 2     20.04.07 14:19 Chrobif
 * Call of externalTDMBusServices_activate() added.
 * 
 * 1     19.04.07 16:19 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES ***********************************************************/

#include <ccblkfn.h>
#include <cdefBF538.h>   

#include "BASETYPE.H"
#include "O4CV_Const.h"
#include "sport0.h"
#include "extTdmBus.h"
#include "binInOut.h"


/* LOCAL CONSTANT DECLARATIONS *********************************************/

// DMA settings
#define FLOW_1							0x1000
#define SLEN_16							0x000f
#define TS_32_33						0x00000003	// bit mask for time slots 32, 33

// sport0 (external TDM bus)
#define EXT_TDM_N_BASIC_TIME_SLOTS		7	// number of basic time slots on external TDM bus
#define EXT_TDM_NUMBER_OF_RX_TS			3
#define EXT_TDM_NUMBER_OF_TX_TS			3	
#define EXT_TDM_BUFFER_SIZE_RX			(2 * EXT_TDM_NUMBER_OF_RX_TS)
#define EXT_TDM_BUFFER_SIZE_TX			(2 * EXT_TDM_NUMBER_OF_TX_TS)


/* LOCAL TYPE DECLARATIONS *************************************************/

/* LOCAL VARIABLE DECLARATIONS *********************************************/

// sport0 DMA tx/rx buffer (external TDM bus)
static UINT16 g_extTdmBusTxBuffer[EXT_TDM_BUFFER_SIZE_TX];
static UINT16 g_extTdmBusRxBuffer[EXT_TDM_BUFFER_SIZE_RX];

static UINT16 *g_pExtTdmTxBuffer0;
static UINT16 *g_pExtTdmTxBuffer1;

static UINT16 *g_pExtTdmRxBuffer0;
static UINT16 *g_pExtTdmRxBuffer1;

static bool g_dmaBuffer0Active;
static bool g_txDmaDisabled;


/* LOCAL MACRO DEFINITIONS *************************************************/

/* LOCAL FUNCTION DEFINITIONS **********************************************/

static void initSport0(UINT32 timeSlotMask[], bool txDmaDisabled);
static void initSport0DMA(bool txDmaDisabled);

static void calcTimeSlotMask(INT16 basicTimeSlot, INT16 boardNumber, UINT32 timeSlotMask[]);


/* IMPLEMENTATION **********************************************************/

/***************************************************************************
  Function:		sport_initExtTdmBus()		
  Description:	Initializes sport0 for external TDM bus
  Inputs:		basicTimeSlot [0...6]
  				boardNumber	[1...4]
  Outputs:		-		
  Return:		-		
  Globals:		g_extTdmBusTxBuffer, g_extTdmBusRxBuffer
  Important:	Call before sport_startExtTdmBus()
  
****************************************************************************/

void sport0_initExtTdmBus(INT16 basicTimeSlot, INT16 boardNumber)
{
    INT16 numOfTxTimeSlots;
    INT16 numOfRxTimeSlots;
    UINT32 timeSlotMask[2];
    
    
    //backplane TDM buffer disable
	binInOut_setOutput(1, e_Tdm_Input_En);
    
    // calculate time slot masks
    calcTimeSlotMask(basicTimeSlot, boardNumber, timeSlotMask);

    if ((boardNumber > 0) && (boardNumber <= N_O4CV_BOARDS))
	{
		g_txDmaDisabled = false;
	}
	else
	{
	    g_txDmaDisabled = true;
	}    

    // init sport
    initSport0(timeSlotMask, g_txDmaDisabled);

    // init DMA
    initSport0DMA(g_txDmaDisabled);

    // set buffer pointers for ISR
    g_pExtTdmTxBuffer0 = g_extTdmBusTxBuffer;
    g_pExtTdmTxBuffer1 = g_extTdmBusTxBuffer + EXT_TDM_NUMBER_OF_TX_TS;

    g_pExtTdmRxBuffer0 = g_extTdmBusRxBuffer;
    g_pExtTdmRxBuffer1 = g_extTdmBusRxBuffer + EXT_TDM_NUMBER_OF_RX_TS;

    g_dmaBuffer0Active = false;

    // assign ISRs to interrupt vectors
	// Sport0 RX ISR -> IVG 13
	register_handler(ik_ivg13, Sport0_RX_ISR);
	
	// assign interrupt
	*pSIC_IAR1 = (*pSIC_IAR1 & 0xFFFFFF0F) | 0x00000060;	// DMA1 Rx irq #13

	ssync();

}
    

/***************************************************************************
  Function:		sport_startExtTdmBus()	
  Description:	Enables DMA1, DMA2 and sport0 for external TDM bus
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		-
  Important:	sport_initExtTdmBus() has to be called before
  
****************************************************************************/

void sport0_startExtTdmBus(void)
{
    // enable rx interrupt
    *pSIC_IMASK0 = *pSIC_IMASK0 | 0x00000200;		// Sport0 RX

	// enable DMAs
	if (!g_txDmaDisabled)
	{
		*pDMA2_CONFIG	= (*pDMA2_CONFIG | DMAEN);
		*pDMA1_CONFIG	= (*pDMA1_CONFIG | DMAEN);
	}
	else 
	{
		*pDMA1_CONFIG	= (*pDMA1_CONFIG | DMAEN);
	}
	ssync();

	// enable Sport0 TX and RX
	*pSPORT0_TCR1 	= (*pSPORT0_TCR1 | TSPEN);
	ssync();
	*pSPORT0_RCR1 	= (*pSPORT0_RCR1 | RSPEN);
	ssync();

	//backplane TDM buffer enable
	binInOut_setOutput(0, e_Tdm_Input_En);
}


/***************************************************************************
  Function:		sport_stopExtTdmBus()	
  Description:	Disables rx interrupt, DMAs and sport0 for external TDM bus.
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		-
  Important:	
  
****************************************************************************/

void sport0_stopExtTdmBus(void)
{
    // disable sport0 rx interrupt
    *pSIC_IMASK0 = *pSIC_IMASK0 & ~0x00000200;	// Sport0 RX
    ssync();

    //backplane TDM buffer disable
	binInOut_setOutput(1, e_Tdm_Input_En);

    // disable DMAs
    *pDMA2_CONFIG	= (*pDMA2_CONFIG & ~DMAEN);
	*pDMA1_CONFIG	= (*pDMA1_CONFIG & ~DMAEN);
	ssync();

    // disable sport0 tx and rx
	*pSPORT0_TCR1 	= (*pSPORT0_TCR1 & ~TSPEN);
	ssync();
	*pSPORT0_RCR1 	= (*pSPORT0_RCR1 & ~RSPEN);
	ssync();
}

/***************************************************************************
  Function:		bool sport0_isExtTdmBusRunning(void)
  Description:	sport0_isExtTdmBusRunning() returns the state of the external
  				TDM bus.
  Inputs:		None
  Outputs:		None
  Return:		True, if external TDM bus is running.
  				False, otherwise.
  Globals:		None
  Important:	-
****************************************************************************/
bool sport0_isExtTdmBusRunning(void)
{
	if ( (*pSPORT0_TCR1 & TSPEN) && (*pSPORT0_RCR1 & RSPEN) )
		return true;
	else
		return false;
}

/***************************************************************************
  Function:		calcTimeSlotMask()
  Description:	Calculates time slot masks for basic and extended time slots
  Inputs:		basicTimeSlot [0...6], boardNumber
  Outputs:		-	
  Return:		timeSlotMask
  Globals:		-
  Important:	-
  
****************************************************************************/

static void calcTimeSlotMask(INT16 basicTimeSlot, INT16 boardNumber, UINT32 timeSlotMask[])
{

    // set basic time slot
    if ((basicTimeSlot >= 0) && (basicTimeSlot < EXT_TDM_N_BASIC_TIME_SLOTS))
	{
		timeSlotMask[0] = BIT0 << basicTimeSlot;
	}
	else
	{
	    timeSlotMask[0] = 0;	// no basic time slot
	}
	
	// set extended time slots
	if ((boardNumber > 0) && (boardNumber <= 3))
	{
		timeSlotMask[1] = (BIT0 | BIT1) << 2*(boardNumber - 1);
	}
	else if (boardNumber == 4)	// workaround for board #4	
	{
		timeSlotMask[0] |= (BIT8 | BIT9);	// unused basic time slots
		timeSlotMask[1] = 0;	   
	}
	else
	{
	    timeSlotMask[1] = 0;
	}
	
}	   


/***************************************************************************
  Function:		initSport0()
  Description:	Initializes sport0 for external TDM bus: TDM mode, 
				external clock and frame sync
  Inputs:		timeSlotMask0: time slots  0 - 31
  				timeSlotMask1: time slots 32 - 39
  Outputs:		-
  Return:		-
  Globals:		-
  Important:	-
  
****************************************************************************/

static void initSport0(UINT32 timeSlotMask[], bool txDmaDisabled)
{
	// Sport0 receive configuration
	// External CLK, External Frame sync, LSB first
	// 16-bit data
	*pSPORT0_RCR1 = RFSR | RLSBIT;
	*pSPORT0_RCR2 = SLEN_16; //0x000f
	
	// Sport1 transmit configuration
	// External CLK, External Frame sync, LSB first
	// 16-bit data
	*pSPORT0_TCR1 = LTFS | TFSR | TLSBIT;
	*pSPORT0_TCR2 = SLEN_16;
	
	// set tx time slots
	*pSPORT0_MTCS0 = timeSlotMask[0];
	*pSPORT0_MTCS1 = timeSlotMask[1];					   

	// set rx time slots
	*pSPORT0_MRCS0 = timeSlotMask[0];
	
	if (txDmaDisabled)
	{
		*pSPORT0_MRCS1 = TS_32_33;			// set extended time slots 32 and 33
	}
	else 
	{	
	    *pSPORT0_MRCS1 = timeSlotMask[1]; 	// set configured time slots
	}		
		
	// Set MCM configuration register and enable MCM mode
	*pSPORT0_MCMC1 = 0x4000;	// Window Size = 40, Offset = 0
	*pSPORT0_MCMC2 = 0x401c;	// 4 MFD, MCMEM, MCDTXPE, MCDRXPE
	
	// init tx FIFO in case of tx DMA disabled
	if (txDmaDisabled)	
	{
		// init tx FIFO
		*((short*)pSPORT0_TX) = 0;	
	}	
}


/***************************************************************************
  Function:		initSport0DMA()
  Description:	Initialize DMA1 and DMA2 to receive and send data on sport0
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		g_extTdmBusRxBuffer, g_extTdmBusTxBuffer
  Important:	-
  
****************************************************************************/

void initSport0DMA(bool txDmaDisabled)
{
	// Set up DMA1 to receive
	// Map DMA1 to Sport0 RX
	*pDMA1_PERIPHERAL_MAP = 0x1000;
	
	// Configure DMA1
	// 16-bit transfers, Interrupt on completion, Autobuffer mode, 2 Dimensional Buffer (Ping Pong)
	*pDMA1_CONFIG = WNR | WDSIZE_16 | DI_EN | FLOW_1 | DMA2D | DI_SEL | RESTART;     	
	// Start address of data buffer
	*pDMA1_START_ADDR  = (void*) g_extTdmBusRxBuffer;	

	// DMA inner loop count
	*pDMA1_X_COUNT = EXT_TDM_NUMBER_OF_RX_TS;
	
	// Inner loop address increment
	*pDMA1_X_MODIFY = 2; 				//for 16-bit data
	*pDMA1_Y_COUNT  = 2; 				//for two sub-buffers
	*pDMA1_Y_MODIFY = 2;				//same as X_MODIFY for contiguous sub-buffers
	
	
	// Set up DMA2 to transmit
	// Map DMA2 to Sport0 TX
	
	if (!txDmaDisabled)
	{
		*pDMA2_PERIPHERAL_MAP = 0x2000;
	
		// Configure DMA2
		// 16-bit transfers, Autobuffer mode, 2 Dimensional Buffer (Ping Pong)
		*pDMA2_CONFIG = WDSIZE_16 | FLOW_1 | DMA2D | DI_SEL | RESTART;	// DI_EN not used  
		// Start address of data buffer
		*pDMA2_START_ADDR = (void*) g_extTdmBusTxBuffer;

		// DMA inner loop count
		*pDMA2_X_COUNT = EXT_TDM_NUMBER_OF_TX_TS;
		// Inner loop address increment
		*pDMA2_X_MODIFY = 2; 				//for 16-bit data
		*pDMA2_Y_COUNT  = 2; 				//for two sub-buffers
		*pDMA2_Y_MODIFY = 2;				//same as X_MODIFY for contiguous sub-buffers
	}	
	
}


/***************************************************************************
  Function:		EX_INTERRUPT_HANDLER(Sport0_RX_ISR)
  Description:	sport0 rx interrupt service routine
  Inputs:		-
  Outputs:		-
  Return:		-
  Globals:		g_pExtTdmTxBuffer0, g_pExtTdmTxBuffer1
  Important: 	-
                                          	
****************************************************************************/

EX_REENTRANT_HANDLER(Sport0_RX_ISR)
{
    static UINT16 *pExtTdmRxBuffer;
	static UINT16 *pExtTdmTxBuffer;

    
	// confirm interrupt handling
	*pDMA1_IRQ_STATUS = 0x0001;
	
	if(g_dmaBuffer0Active)
	{
	    pExtTdmTxBuffer = g_pExtTdmTxBuffer1;
		pExtTdmRxBuffer = g_pExtTdmRxBuffer1;
		
	}		
	else
	{
	    pExtTdmTxBuffer = g_pExtTdmTxBuffer0;
		pExtTdmRxBuffer = g_pExtTdmRxBuffer0;
		
	}
	
	g_dmaBuffer0Active = !g_dmaBuffer0Active;
	
	extTdmBus_activate(pExtTdmRxBuffer, pExtTdmTxBuffer);
	
	if (g_txDmaDisabled)
	{
		// write 16-bit data to FIFO
		*((short*)pSPORT0_TX) = *pExtTdmTxBuffer;
	}
	
}

