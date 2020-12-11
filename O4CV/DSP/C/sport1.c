/*-------------------------------------------------------------------------*
 * $Workfile	: sport1.c$
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : V. Pogorilyy
 * Remarks      :  
 * Purpose      : 2 Wire interface driver
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/sport1.c $
 * 
 * 6     08-02-06 7:50 Chrobif
 * + sport1_isTxRunning() added.
 * 
 * 5     26.09.07 14:52 Chhamaa
 * Global buffer pointers g_pRxBuffer2wIf, g_pTxBuffer2wIf initialised.
 * 
 * 4     4.09.07 17:57 Chhamaa
 * Initialisation, start and stop functions modified: DMAs disabled.
 * 
 * 3     07-06-15 13:15 Chvopog
 * Multichannel SPORT1 register changed
 * 
 * 2     07-06-14 7:50 Chvopog
 * Interrupt priority changed, Interrupt handlers changed (execution time
 * 200ns)
 * 
 * 1     07-06-04 11:47 Chvopog
 * Initial version
 * 
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/
#include <cdefBF538.h>
#include <sys\exception.h>
#include "Basetype_O4CV.h"
#include "sport1.h"
#include "SPI.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL VARIABLE DECLARATIONS ***********************************************/

#pragma align 4				  
#pragma section("sdram0_bank2") // 30ms, 8kHz receive buffers 
static	INT16 RxBuffer_8k_A	[N_CHANNELS_2W] [BUFFER_SIZE_2W_IF];
#pragma align 4
#pragma section("sdram0_bank2")	// 30ms, 8kHz transmit buffers 
static	INT16 RxBuffer_8k_B	[N_CHANNELS_2W] [BUFFER_SIZE_2W_IF];
#pragma align 4				  
#pragma section("sdram0_bank2") // 30ms, 8kHz receive buffers 
static	INT16 TxBuffer_8k_A	[N_CHANNELS_2W] [BUFFER_SIZE_2W_IF];
#pragma align 4
#pragma section("sdram0_bank2")	// 30ms, 8kHz transmit buffers 
static	INT16 TxBuffer_8k_B	[N_CHANNELS_2W] [BUFFER_SIZE_2W_IF];


static	bool	g_activeBuffer2wRxIsA;		// flag for active set of buffers
static	bool	g_tWireDataRxAvailable;		// flag for availability of 2-wire data
static	bool	g_activeBuffer2wTxIsA;		// flag for active set of buffers
static	bool	g_tWireDataTxSent;			// flag for availability of 2-wire data

extern volatile bool flag_SPIsendfinisched;

INT16 *g_pRxBuffer2wIf,*g_pTxBuffer2wIf; 


/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/

EX_INTERRUPT_HANDLER(dma3_sport1_rx_irq);
EX_INTERRUPT_HANDLER(dma4_sport1_tx_irq);
//EX_INTERRUPT_HANDLER(ISR_SPI);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		sport1_init2wireInterface()
  Description:	initialises and starts the 2-wire interface of the O4CV,
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    
*******************************************************************************/

void sport1_init2wireInterface(void) {
    
    // disable DMA 3
	*pDMA3_CONFIG	= (*pDMA3_CONFIG & (~DMAEN));
	// disable DMA 4
	*pDMA4_CONFIG	= (*pDMA4_CONFIG & (~DMAEN)); 
   	// Sport1 receive configuration
	// 0 - Drive internal frame sync on rising edge of RSCLK. Sample data and external frame sync with falling edge of RSCLK.,0 - Early frame syncs,0 - Active high RFS
	// 0-Does not require RFS for every data word,1 - Internal RFS used
	// 0 - Receive MSB first,
	// 01 - Sign-extend,1 - Internal receive clock selected,0 - Receive disabled;	
	*pSPORT1_RCR1	= 0x0204;
	// 16-bit data
	*pSPORT1_RCR2 = SLEN_16; //0x001f
		
	// Sport1 transmit configuration	
	//0 - Drive data and internal frame syncs with rising edge of TSCLK. Sample external frame syncs with falling edge of TSCLK,0 - Early frame syncs,0 - Active high TFS,  
	//0 - Data-dependent TFS generated,0 - Does not require TFS for every data word, 1 - Internal TFS used,
	//0 - Transmit MSB first ;Normal operation 00,External transmit clock 0,Transmit disabled 0
	*pSPORT1_TCR1=ITFS;
	// 16-bit data
	*pSPORT1_TCR2 = SLEN_16;
	

	*pSPORT1_MTCS0 = SLIC_TDM_TIME_SLOT_MASK_TX; 	//  Tx 5 TimeSlots
	*pSPORT1_MTCS1 = 0x00000000;				   

	*pSPORT1_MRCS0 = SLIC_TDM_TIME_SLOT_MASK_RX; 	// Rx 5 TS
	*pSPORT1_MRCS1 = 0x00000000; 				
	
	// Set MCM configuration register and enable MCM mode
	
	*pSPORT1_MCMC1 = 0x0000;	// Window Size = 8; 0=(8/8)-1; Offset = 0	
	*pSPORT1_MCMC2 = 0x001c;	// 0 MFD, MCMEN, MCDTXPE, MCDRXPE
	
	//Transmit Frame Sync Divider Register 127 = (128-1) = 1.024MHz (serial clock)/8KHz (Frame sync time)
	*pSPORT1_RFSDIV = 0x007F;
	*pSPORT1_TFSDIV = 0x007F;
	
		
	register_handler(ik_ivg10, dma3_sport1_rx_irq);// assign function to irq 
	register_handler(ik_ivg11, dma4_sport1_tx_irq);// assign function to irq 
	register_handler(ik_ivg12, ISR_SPI);// assign function to irq 
	*pSIC_IAR1 = (*pSIC_IAR1 & 0xFF000FFF) | 0x00543000;// assign DMA3 Rx  ,DMA4 Tx & SPI IRQ to irq #11,#12,#10		
	*pSIC_IMASK0 = *pSIC_IMASK0 | 0x00003800;		// enable IRX SPI,  DMA3, DMA4
	ssync();     				

	//register_handler(ik_ivg11, dma3_sport1_rx_irq);// assign function to irq 
	//register_handler(ik_ivg12, dma4_sport1_tx_irq);// assign function to irq 
	//register_handler(ik_ivg8, ISR_SPI);// assign function to irq 
	//*pSIC_IAR1 = (*pSIC_IAR1 & 0xFF000FFF) | 0x00154000;// assign DMA3 Rx  ,DMA4 Tx & SPI IRQ to irq #11,#12,#8		
	//*pSIC_IMASK0 = *pSIC_IMASK0 | 0x00002000;		// enable SPI IRX
	//ssync();  
	
 	
}


/*****************************************************************************
  Function:		sport1_startRx2wireInterface
  Description:	enables DMA3
  				enables Sport 1 to get SPORT data flow  				
  				enables Interrupt from DMA 
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    
*******************************************************************************/

void sport1_startRx2wireInterface(void) {
    
    
   	// reconfigure DMA3 for in samples:
	//  - set-up DMA in 2D mode, such that buffers are written in the order:
	//	  buf[0][0], buf[0][1], ... buf[0][3], buf[1][0], buf[1][0], buf[1][1], ... buf[3][7]
	//  - ==> inner count = N_CHANNELS, adr. update is N_CHANNELS * 2Bytes	:   8
	//  - ==> outer count = #SAMPLES  , adr. update is ... 					: -46
	//  - DMA enabled, wordsize 16, retain FIFO mode
	//	- irq after completition of inner AND outer loop. IRQ enabled for DMA3 (Rx)!
	//	- DMA in STOP mode ==> needs reconfiguration in IRQ !!
     
	// Set up DMA3 to receive
	// Map DMA3 to Sport1 RX
	*pDMA3_PERIPHERAL_MAP = 0x3000;
	*pDMA3_START_ADDR	= &RxBuffer_8k_A[0][0];	// init to read from buffer A
	*pDMA3_X_COUNT		= N_CHANNELS_2W;
    *pDMA3_X_MODIFY		= BUFFER_SIZE_2W_IF * sizeof(INT16);
    *pDMA3_Y_COUNT		= BUFFER_SIZE_2W_IF;
    *pDMA3_Y_MODIFY		= ( -1 * (N_CHANNELS_2W - 1) * BUFFER_SIZE_2W_IF + 1) * 2;
    // 16-bit transfers, Interrupt on completion, Stop mode, 2 Dimensional Buffer (Ping Pong)
    *pDMA3_CONFIG 		= WNR |  WDSIZE_16 | DMA2D | DI_EN ;
          
    g_activeBuffer2wRxIsA = true;			// flag for active set of buffers
	g_tWireDataRxAvailable = false;			// flag for availability of 2-wire data
	g_pRxBuffer2wIf = &RxBuffer_8k_B[0][0];	// init global buffer pointer
	
	// enable  Sport1 RX interrupt
	*pSIC_IMASK0 = *pSIC_IMASK0 | 0x00000800;		 
    // enable DMA
	*pDMA3_CONFIG	= (*pDMA3_CONFIG | DMAEN);	
	// enable Sport1 and RX	
	*pSPORT1_RCR1 	= (*pSPORT1_RCR1 | RSPEN);   
	ssync();
	// on exit of the start function:    
    //	- Int on DMA 3 is enabled
    //	- DMA 3 is configured & enabled to receive buffers from SPROT    
    //	- Sport enabled
    //	- IRQ (dma rx irq) is configured and enabled 
 
}

/*****************************************************************************
  Function:		sport1_startTx2wireInterface
  Description:	enables DMA
  				enables Sport 1 to get SPORT data flow  				
  				enables Interrupt from DMA 
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    
*******************************************************************************/

void sport1_startTx2wireInterface(void) {  
    // reconfigure DMA4 for output of samples:
	//  - set-up DMA in 2D mode, such that buffers are written in the order:
	//	  buf[0][0], buf[0][1], ... buf[0][3], buf[1][0], buf[1][0], buf[1][1], ... buf[3][7]
	//  - ==> inner count = N_CHANNELS, adr. update is N_CHANNELS * 2Bytes	:   8
	//  - ==> outer count = #SAMPLES  , adr. update is ... 					: -46
	//  - DMA enabled, wordsize 16, retain FIFO mode
	//	- irq after completition of inner AND outer loop. IRQ enabled for DMA4 (Tx)
	//	- DMA in STOP mode ==> needs reconfiguration in IRQ !!
    
	// Set up DMA4 to transmit
	// Map DMA4 to Sport1 TX
	*pDMA4_PERIPHERAL_MAP = 0x4000;
	*pDMA4_START_ADDR	= &TxBuffer_8k_A[0][0];	// init to write to buffer A
	// DMA inner loop count
	*pDMA4_X_COUNT		= N_CHANNELS_2W;
    *pDMA4_X_MODIFY		= BUFFER_SIZE_2W_IF * sizeof(INT16);
    *pDMA4_Y_COUNT		= BUFFER_SIZE_2W_IF;
    *pDMA4_Y_MODIFY		= ( -1 * (N_CHANNELS_2W - 1) * BUFFER_SIZE_2W_IF + 1) * 2;	
	// Configure DMA4
	// 16-bit transfers, Autobuffer mode, 2 Dimensional Buffer (Ping Pong)
	*pDMA4_CONFIG = WDSIZE_16 | DI_EN | DMA2D ;
	
	g_activeBuffer2wTxIsA = true;			// flag for active set of buffers
    g_tWireDataTxSent    = false;			// flag for availability of 2-wire data
    g_pTxBuffer2wIf = &TxBuffer_8k_B[0][0];	// init global buffer pointer
 	
	// enable Sport1 TX interrupt
	*pSIC_IMASK0 = *pSIC_IMASK0 | 0x00001000;		
    // enable DMAs
	*pDMA4_CONFIG	= (*pDMA4_CONFIG | DMAEN);	
	// enable Sport1 TX and RX
	*pSPORT1_TCR1 	= (*pSPORT1_TCR1 | TSPEN);
	// on exit of the start function:  
    //	- Int DMA 4 is enabled   
    //	- DMA 4 is configured & enabled, transfer buffers to SPORT    
    //	- Sport enabled 
    //	- IRQ (dma tx irq) is configured and enabled 
	
}


/*****************************************************************************
  Function:		sport1_stopRx2wireInterface
  Description:	disables Int DMA 3 				
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/

void sport1_stopRx2wireInterface(void) {
       
    // Disable Int on DMA 3;    
    *pSIC_IMASK0 = (*pSIC_IMASK0 & 0xFFFFF7FF);
    // disable DMA 3
	*pDMA3_CONFIG	= (*pDMA3_CONFIG & (~DMAEN));
	// Disable Sport1 and RX	
	*pSPORT1_RCR1 	= (*pSPORT1_RCR1 & 0xFFFE);   
	ssync();    
    // on exit of the stop function:    
    //	- Int on DMA 3 is disabled        
    //  - Sport1 RX Disable
}


/*****************************************************************************
  Function:		sport1_stopTx2wireInterface
  Description:	Disables Int DMA  				
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/

void sport1_stopTx2wireInterface (void) { 
       
    // Disable Int on DMA 4;
    *pSIC_IMASK0 = (*pSIC_IMASK0 & 0xFFFFEFFF);		// disable DMA4 TX IRX
    // disable DMA 4
	*pDMA4_CONFIG	= (*pDMA4_CONFIG & (~DMAEN)); 		
	// Disable Sport1 and TX  
	*pSPORT1_TCR1	= (*pSPORT1_TCR1 & 0xFFFE);	    
	ssync();
	// on exit of the start function:  
    //	- Int DMA 4 is Disabled       
    //  - Sport1 TX Disabled
}

/*****************************************************************************
  Function:		bool sport1_isTxRunning(void)
  Description:	Returns the state of the sport 1 tx direction.				
  Inputs:		None
  Outputs:		None
  Return:		True, if sport 1 tx in enabled. False otherwise.
  Globals:      None
  Important:    -
*******************************************************************************/
bool sport1_isTxRunning(void)
{
    return (*pSPORT1_TCR1 & TSPEN);
}

volatile unsigned char *DataPtr;
volatile int dataLength;

/*****************************************************************************
  Function:		EX_INTERRUPT_HANDLER(dma4_sport1_tx_irq)
  Description:	Interrupt handler sport1_tx
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/
EX_INTERRUPT_HANDLER(dma4_sport1_tx_irq)
{

  	// confirm interrupt handling
	*pDMA4_IRQ_STATUS = 0x0001;   	
	//while ((*pDMA4_IRQ_STATUS & 0x0008) !=0 ) {// wait till DMA finished & FIFO empty        
    //}
   	 
	// switch buffer and re-enable DMA:
    if (g_activeBuffer2wTxIsA == true) {
        g_activeBuffer2wTxIsA= false;
        *pDMA4_START_ADDR	= &TxBuffer_8k_B[0][0];
        g_pTxBuffer2wIf	 	= &TxBuffer_8k_A[0][0];
    }
    else{
        g_activeBuffer2wTxIsA= true;
        *pDMA4_START_ADDR	= &TxBuffer_8k_A[0][0];
        g_pTxBuffer2wIf		= &TxBuffer_8k_B[0][0];
    }        
   
    *pDMA4_CONFIG	= (*pDMA4_CONFIG | DMAEN);
    ssync();    
    g_tWireDataTxSent = true;
}

/*****************************************************************************
  Function:		EX_INTERRUPT_HANDLER(dma4_sport1_rx_irq)
  Description:	Interrupt handler sport1_rx
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/
EX_INTERRUPT_HANDLER(dma3_sport1_rx_irq)
{
	 // confirm interrupt handling
	*pDMA3_IRQ_STATUS = 0x0001;   	
	//while ((*pDMA3_IRQ_STATUS & 0x0008) !=0 ) {// wait till DMA finished & FIFO empty        
    //}
 
	// switch buffer and re-enable DMA:
    if (g_activeBuffer2wRxIsA == true) {
        g_activeBuffer2wRxIsA = false;
        *pDMA3_START_ADDR	 = &RxBuffer_8k_B[0][0];
        g_pRxBuffer2wIf     = &RxBuffer_8k_A[0][0];
    }
    else{
        g_activeBuffer2wRxIsA = true;
        *pDMA3_START_ADDR	 = &RxBuffer_8k_A[0][0];	        
        g_pRxBuffer2wIf     = &RxBuffer_8k_B[0][0];    
    }        
	*pDMA3_CONFIG	= (*pDMA3_CONFIG | DMAEN);	
    g_tWireDataRxAvailable = true;
    ssync();         
    
}


//---beschreibung....

bool sport1_2wireDataAvailable(void) {
    
    if (g_tWireDataRxAvailable == true) {
        g_tWireDataRxAvailable  = false;
        return(true);
    }
    
    return(false);
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Dummy handling of data:
//		- polling function for availability of data
//		- loopback function
//
void sport1_2wireDataLoopback(void) {
    INT16 i, *pSrc, *pDest,*pSrc1, *pDest1,*pSrc2, *pDest2;
    
    // chose buffers not operated on by DMA
   if (g_activeBuffer2wRxIsA == true) {		
        pSrc	= &RxBuffer_8k_B[0][0];
        pDest	= &TxBuffer_8k_B[0][0];
    }
    else{
        pSrc	= &RxBuffer_8k_A[0][0];
        pDest	= &TxBuffer_8k_A[0][0];
    }    
    
    if (g_activeBuffer2wRxIsA == true) {		               
        for (i=0;i<(BUFFER_SIZE_2W_IF);i++) {
            TxBuffer_8k_B[4][i]= RxBuffer_8k_B[0][i];
    	    TxBuffer_8k_B[0][i]= RxBuffer_8k_B[4][i];
    	} 
    }
    else{
        for (i=0;i<(BUFFER_SIZE_2W_IF);i++) {
            TxBuffer_8k_A[4][i]= RxBuffer_8k_A[0][i];
    	    TxBuffer_8k_A[0][i]= RxBuffer_8k_A[4][i];
    	}        
    }
    
    
    //loop back, i.e. copy src to dest:    
    //for (i=0;i<(N_CHANNELS_2W*BUFFER_SIZE_2W_IF);i++) {
    //    *pDest++ = *pSrc++;
    //} 

 

           
}
