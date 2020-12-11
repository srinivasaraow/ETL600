/*-------------------------------------------------------------------------*
 * $Workfile	:  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : D. Cachin/ V. Pogorilyy
 * Remarks      :  
 * Purpose      : 4 Wire interface driver
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/C/sport2.c $
 * 
 * 8     10.04.08 9:41 Chhamaa
 * Function sport2_init4wireInterface() modified. 
 * 
 * 7     26.09.07 14:54 Chhamaa
 * Global buffer pointers g_pRxBuffer4wIf, g_pTxBuffer4wIf initialised.
 * 
 * 6     4.09.07 18:02 Chhamaa
 * Initialisation, start and stop functions modified: DMAs disabled.
 * 
 * 5     07-08-08 14:00 Chalnot
 * Delay with timer module.
 * 
 * 4     07-07-03 10:01 Chvopog
 * Sync to 4 wire codec on Rx added. 
 *
 * 3     07-06-14 7:52 Chvopog
 * Interrupt priority changed, Interrupt handlers changed (execution time
 *  200ns)
 *
 * 2     11.05.07 13:01 Chhamaa
 * Defines renamed
 * 
 * 1     07-05-09 15:11 Chvopog
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/
#include <cdefBF538.h>
#include <sys\exception.h>
#include "Basetype_O4CV.h"
#include "O4CV_Const.h"
#include "sport2.h"
#include "timer.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define N_REGISTER	8		// number of config registers per channel

/* LOCAL TYPE DECLARATIONS ***************************************************/
typedef struct {			// structure for 73322L register configuration:
	UINT16	data	:8;		//  - 8bit register data
    UINT16	reg		:3;		//  - 3bit register adr. 
    UINT16	device	:3;		//  - 3bit device adr.     
    UINT16	read	:1;		//	- 1bit read or _write    
    UINT16	control	:1;		//	- 1bit control or _data        
} t_4w_RegConfig;


/* LOCAL VARIABLE DECLARATIONS ***********************************************/
#pragma align 4													// define double 
#pragma section("sdram0_bank2")							// Buffer MUST be placed in BANK2 of SDRAM. Otherwise it will be cached
  	INT16	RxBuffer_A [N_CHANNELS_ON_BOARD][BUFFER_SIZE_4W_IF];	// sample in-/output
#pragma align 4													
#pragma section("sdram0_bank2")							// allignment to 4
 	INT16	RxBuffer_B [N_CHANNELS_ON_BOARD][BUFFER_SIZE_4W_IF];	// and placing to 
#pragma align 4											// memory bank A is 
#pragma section("sdram0_bank2")							// intended for opti-
 	INT16	TxBuffer_A [N_CHANNELS_ON_BOARD][BUFFER_SIZE_4W_IF];	// mal filter algorithm 
#pragma align 4											// performance
#pragma section("sdram0_bank2")
	INT16	TxBuffer_B [N_CHANNELS_ON_BOARD][BUFFER_SIZE_4W_IF];

static	bool	ActiveBufferRxIs_A;		// flag for active set of buffers
static	bool	Fwire_data_Rxavailable;		// flag for availability of 4-wire data
static	bool	ActiveBufferTxIs_A;		// flag for active set of buffers
static	bool	Fwire_data_Txsent;		// flag for availability of 4-wire data


INT16 *g_pRxBuffer4wIf, *g_pTxBuffer4wIf; 

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
EX_INTERRUPT_HANDLER(dma8_sport2_rx_irq);
EX_INTERRUPT_HANDLER(dma9_sport2_tx_irq);

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		sport2_init4wireInterface()
  Description:	initialises and starts the 4-wire interface of the O4CV,
  				including ADC/DAC 73322L and the SPORT2
  Inputs:		- analog rx and tx gain configuration for all channels
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    when function returns, 4wire interface is activated and IRQs 
				will be generated as soon as samples are received but SPORT is switched OFF.				
*******************************************************************************/

void sport2_init4wireInterface(t_4w_Gains *p_4w_gains) {
    
	INT16			temp, i, j;
	UINT8			g_tmp;
	t_4w_RegConfig	fw_reg_config[N_REGISTER][N_CHANNELS_ON_BOARD];
	
	
	// Port E GPIO configuration for bit3, 4W-SE and bit 7, 4W-RESET. 
	// change such that temaining configuration is unchanges!!
	*pPORTEIO_FER	= (*pPORTEIO_FER | 0x0088);		// enable bits 3 & 7 
	*pPORTEIO_DIR	= (*pPORTEIO_DIR | 0x0088);		// bits 3 & 7 are outputs 
    *pPORTEIO_CLEAR	=                  0x0088 ;		// disable and reset 4W

    // configuration of SPORT2, connected to ADC/DAC 73322L. Configuration is:
    //	- NOT ENABLED YET (see manual!)		- data only on possibly irregular FS
    //	- external clock					- FS is actvie high
    //	- data out as written				- FS one clock befor data
    //	- data in: sign extension			- data and FS written on rising edge, 
    //  - MSB first							  read on falling edge
    //  - external framesync (FS)			- normal mode, no stereo, no sec. side
    //  - one FS per dataword				- wordsize 16
    *pSPORT2_TCR1	= 0x0400;
    *pSPORT2_TCR2	= 0x000F;
    *pSPORT2_RCR1	= 0x0404;
    *pSPORT2_RCR2	= 0x000F;   

    // prepare configuration structure to be written to the ADC:
    for (i=0;i<N_REGISTER;i++) {
    	for (j=0;j<N_CHANNELS_ON_BOARD;j++) {
    	    fw_reg_config[i][j].control	= 1;	// only control data ...
    	    fw_reg_config[i][j].read	= 0;	// ... to be written
    	    fw_reg_config[i][j].device	= N_CHANNELS_ON_BOARD - 1 - j;	// numbered in 
    	    													// revers order
    	    fw_reg_config[i][j].reg		= i + 1;// numbered from 1, 2, ... 0 !!!
    	    fw_reg_config[i][j].data	= 0;	// initial value   	    
    	}
    }
    
    // now, set the data identically for all channels (only gain is individuall):
    for (j=0;j<N_CHANNELS_ON_BOARD;j++) {
        
        // assemble individual data for gain config register 
        // (remember that registers are stored in reverse order!!)
        g_tmp = 0x00 | (p_4w_gains->tx_gain[N_CHANNELS_ON_BOARD - 1 - j] << 4) 
                     | (p_4w_gains->rx_gain[N_CHANNELS_ON_BOARD - 1 - j]     );
        
		// configuration data for the cfg. register A ... H: 
   	    fw_reg_config[0][j].data	= 0x0F;		// reg B: clock settings: SPORT 16.384MHz, fs 64kHz
   	    fw_reg_config[1][j].data	= 0x3D;		// reg C: power everything expect 'GainTap'. No REFOUT
   	    fw_reg_config[2][j].data	= g_tmp;	// reg D: analog in- & output gain
   	    fw_reg_config[3][j].data	= 0x00;		// reg E: no digital Gain Tap
   	    fw_reg_config[4][j].data	= 0x00;		// reg F: no analog  Gain Tap
   	    fw_reg_config[5][j].data	= 0x00;		// reg G: no config for digital Gain Tap
   	    fw_reg_config[6][j].data	= 0x00;		// reg H: no config for digital Gain Tap
   	    fw_reg_config[7][j].data	= 0x31;		// reg A: 4 devices, START (ACTIVE) DATA MODE !!
    }	   										// (A is last --> switch to data mode!)
	
	
    // set-up of the DMA, such that initialisation is written to the L73322L
    //	- enable (will start immediately)		- irq on completion of whole buffer
    //	- memory reas (transmit) operation		- irq not enabled, poll during config phase !!
    //	- 16bit transfers						- flex descriptor size: n/a (0)
    //	- linear mode							- operation mode: stop
    //	- discard FIFO befor begin
    *pDMA8_CONFIG 		= 0x0096;// DMA8 disable
    *pDMA9_CONFIG 		= 0x0024;// DMA9 disable
    *pDMA9_START_ADDR	= fw_reg_config;
    *pDMA9_X_COUNT		= N_REGISTER * N_CHANNELS_ON_BOARD;
    *pDMA9_X_MODIFY		= sizeof(t_4w_RegConfig);
    *pDMA9_CONFIG 		= 0x0025;
    
    ssync();
    
    // 100 us delay for reset pulse
	timer_init(100, e_Timer1);
	timer_wait(e_Timer1);
    
    // release 4W reset, but do not enable:
    *pPORTEIO_SET	= 0x0080; 
      
    // enable 4W interface ==> configuration will be written
    *pPORTEIO_SET	= 0x0008;					// activate 4W
    
    ssync();
    
	// 1 ms delay
	timer_init(1000, e_Timer1);
	timer_wait(e_Timer1);
    
	// enable SPORT2 TX:
    *pSPORT2_TCR1	= (*pSPORT2_TCR1 | 0x0001);	// must be done after initialisation 
    //*pSPORT2_RCR1	= (*pSPORT2_RCR1 | 0x0001);	// of SPORT and DMAs !!
  	
    ssync();
  	    	
    // wait for completion of initialisation phase:
	temp = 1;
	while (temp > 0){
    	temp = *pDMA9_IRQ_STATUS;				// poll bit #3 of IRQ_STATUS:
        temp = (temp & 0x0008) >> 3;			// bit DMA_RUN
    }    
    
    // 252 ms delay
	timer_init(252000, e_Timer1);
	timer_wait(e_Timer1);
	

    // disable SPORT2 TX and RX for reconfiguration of DMA:
    *pSPORT2_TCR1	= (*pSPORT2_TCR1 & 0xFFFE);	// highly recommended for re-init of 
    *pSPORT2_RCR1	= (*pSPORT2_RCR1 & 0xFFFE);	// DMA channels
    *pDMA9_CONFIG 	= 0x0024;//stop DMA

    // 84ms delay
	timer_init(84000, e_Timer1);
	timer_wait(e_Timer1); 
	   
	register_handler(ik_ivg9, dma9_sport2_tx_irq);	// assign function to irq 
	register_handler(ik_ivg8,dma8_sport2_rx_irq);	// assign function to irq 
	
	*pSIC_IAR4	 = (*pSIC_IAR4 & 0xFFFFF00F) | 0x00000210;	// assign DMA8 Rx  & DMA9 Tx IRQ to irq #9,#8	
	*pSIC_IMASK1 = *pSIC_IMASK1 | 0x00000006;		// enable  IRX, DMA8, DMA9
		
	// on exit of the init function:
    //	- 4-wire interface is configured and enabled
    //	- IRQ (dma rx irq) is configured and not enabled 
    //	- Sport not Enabled, No samples have been received                 	       
}

/*****************************************************************************
  Function:		sport2_startRx4wireInterface
  Description:	enables DMA8
  				enables Sport 2 to get SPORT data flow  				
  				enables Interrupt from DMA 8
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    void sport2_init4wireInterface(t_4w_Gains *p_4w_gains)
  				must be called before this function
*******************************************************************************/

void sport2_startRx4wireInterface(void) {
      // disable 4W interface ==> deactivate 4W
    *pPORTEIO =(*pPORTEIO & 0xFFF7);    
    
   	// reconfigure DMA8 for in samples:
	//  - set-up DMA in 2D mode, such that buffers are written in the order:
	//	  buf[0][0], buf[0][1], ... buf[0][3], buf[1][0], buf[1][0], buf[1][1], ... buf[3][7]
	//  - ==> inner count = N_CHANNELS_ON_BOARD, adr. update is N_CHANNELS_ON_BOARD * 2Bytes	:   8
	//  - ==> outer count = #SAMPLES  , adr. update is ... 					: -46
	//  - DMA enabled, wordsize 16, retain FIFO mode
	//	- irq after completition of inner AND outer loop. IRQ enabled for DMA8 (Rx)!
	//	- DMA in STOP mode ==> needs reconfiguration in IRQ !!
	*pDMA8_PERIPHERAL_MAP = 0x0000;	
    *pDMA8_START_ADDR	= &RxBuffer_A[0][0];	// init to read from buffer A
    *pDMA8_X_COUNT		= N_CHANNELS_ON_BOARD;
    *pDMA8_X_MODIFY		= BUFFER_SIZE_4W_IF * sizeof(INT16);
    *pDMA8_Y_COUNT		= BUFFER_SIZE_4W_IF;
    *pDMA8_Y_MODIFY		= ( -1 * (N_CHANNELS_ON_BOARD - 1) * BUFFER_SIZE_4W_IF + 1) * 2;
    *pDMA8_CONFIG 		= 0x0096;
    
    ActiveBufferRxIs_A	= true;					// designate active buffer
    Fwire_data_Rxavailable	= false;			// state no data available yet    
    g_pRxBuffer4wIf = &RxBuffer_B[0][0];		// init global buffer pointer		
    
    // Enable Int on DMA 8;
 	*pSIC_IMASK1 = *pSIC_IMASK1 | 0x00000002;		// enable DMA8 RX IRX
	ssync();
    // enable DMA 8:
    *pDMA8_CONFIG = 0x0097;
    // enable SPORT2 RX:
    *pSPORT2_RCR1 = (*pSPORT2_RCR1 | 0x0001);	
    ssync();  
	
    // on exit of the start function:    
    //	- Int on DMA 8 is enabled
    //	- DMA 8 is configured & enabled to receive buffers from SPROT    
    //	- Sport enabled
    //	- IRQ (dma rx irq) is configured and enabled 
    
}

/*****************************************************************************
  Function:		sport2_startTx4wireInterface
  Description:	enables DMA9
  				enables Sport 2 to get SPORT data flow  				
  				enables Interrupt from DMA 9
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    void sport2_init4wireInterface(t_4w_Gains *p_4w_gains)
  				must be called before this function
*******************************************************************************/

void sport2_startTx4wireInterface(void) {  
    // reconfigure DMA9 for output of samples:
	//  - set-up DMA in 2D mode, such that buffers are written in the order:
	//	  buf[0][0], buf[0][1], ... buf[0][3], buf[1][0], buf[1][0], buf[1][1], ... buf[3][7]
	//  - ==> inner count = N_CHANNELS_ON_BOARD, adr. update is N_CHANNELS_ON_BOARD * 2Bytes	:   8
	//  - ==> outer count = #SAMPLES  , adr. update is ... 					: -46
	//  - DMA enabled, wordsize 16, retain FIFO mode
	//	- irq after completition of inner AND outer loop. IRQ enabled for DMA9 (Tx)
	//	- DMA in STOP mode ==> needs reconfiguration in IRQ !!
	
    *pDMA9_PERIPHERAL_MAP = 0x1000;
    *pDMA9_START_ADDR	= &TxBuffer_A[0][0];	// init to write to buffer A
    *pDMA9_X_COUNT		= N_CHANNELS_ON_BOARD;
    *pDMA9_X_MODIFY		= BUFFER_SIZE_4W_IF * sizeof(INT16);
    *pDMA9_Y_COUNT		= BUFFER_SIZE_4W_IF;
    *pDMA9_Y_MODIFY		= ( -1 * (N_CHANNELS_ON_BOARD - 1) * BUFFER_SIZE_4W_IF + 1) * 2;
    *pDMA9_CONFIG 		= 0x0094;
    
    ActiveBufferTxIs_A	= true;					// designate active buffer
    Fwire_data_Txsent	= false;				// state no data available yet
    g_pTxBuffer4wIf		= &TxBuffer_B[0][0];	// init global buffer pointer
      
    // Enable Int on DMA 9;
    *pSIC_IMASK1 = *pSIC_IMASK1 | 0x00000004;		// enable DMA9 TX IRX
	ssync();
    // enable DMA 9:
	*pDMA9_CONFIG = 0x0095;
    // enable SPORT2 TX:
    *pSPORT2_TCR1 = (*pSPORT2_TCR1 | 0x0001);  
    ssync();   
    
    // enable 4W interface ==> 
    *pPORTEIO_SET = 0x0008;		// activate 4W
    
    // on exit of the start function:  
    //	- Int DMA 9 is enabled   
    //	- DMA 9 is configured & enabled, transfer buffers to SPORT    
    //	- Sport enabled 
    //	- IRQ (dma tx irq) is configured and not enabled 
}


/*****************************************************************************
  Function:		sport2_stopRx4wireInterface
  Description:	disables Int DMA8  				
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/

void sport2_stopRx4wireInterface(void) {
    
    // Disable Int on DMA 8;    
 	*pSIC_IMASK1 = (*pSIC_IMASK1 & 0xFFFFFFFD);
 	// DMA8 disable
 	*pDMA8_CONFIG = 0x0096;
 	// Disable Sport2 rx    
 	*pSPORT2_RCR1 = (*pSPORT2_RCR1 & 0xFFFE);	// DMA channels 	
	ssync();
 
    // on exit of the start function:    
    //	- Int on DMA 8 is disabled        
}

/*****************************************************************************
  Function:		sport2_stopTx4wireInterface
  Description:	Disables Int DMA9  				
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/

void sport2_stopTx4wireInterface (void) {
       
    // Disable Int on DMA 9
    *pSIC_IMASK1 = (*pSIC_IMASK1 & 0xFFFFFFFB);
    // DMA9 disable
    *pDMA9_CONFIG = 0x0024;
    // Disable Sport2 tx
	*pSPORT2_TCR1 = (*pSPORT2_TCR1 & 0xFFFE);	    
	ssync();
	// on exit of the start function:  
    //	- Int DMA 9 is Disabled       
}

/*****************************************************************************
  Function:		dma8_sport2_rx_irq
  Description:	interrupt on completion of dma8 (sport2_rx_irq) 
				- buffers to wrtie/read are swapped
				- dma is reinitialised
				- availability of data is signalled to main()
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    --
*******************************************************************************/
EX_INTERRUPT_HANDLER(dma8_sport2_rx_irq){
    unsigned short temp=0;
    
        
    	
   	 	// confirm interrupt handling
		*pDMA8_IRQ_STATUS = 0x0001;   	
    	//while ((*pDMA8_IRQ_STATUS & 0x0008) !=0 ) {// wait till DMA finished & FIFO empty        
	    //}
		
		
		// switch buffer and re-enable DMA:
	    if (ActiveBufferRxIs_A == true) {
	        ActiveBufferRxIs_A  	= false;
	        *pDMA8_START_ADDR	= &RxBuffer_B[0][0];
	        g_pRxBuffer4wIf	        = &RxBuffer_A[0][0];
	    }
	    else{
	        ActiveBufferRxIs_A  	= true;
	        *pDMA8_START_ADDR	= &RxBuffer_A[0][0];	        
	        g_pRxBuffer4wIf	        = &RxBuffer_B[0][0]; 
	    }        
    	       
       *pDMA8_CONFIG = 0x0097;
       ssync();
       Fwire_data_Rxavailable = true;
        
}



/*****************************************************************************
  Function:		dma9_sport2_tx_irq
  Description:	interrupt on completion of dma9 (sport2_tx_irq) 				
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    --
*******************************************************************************/
EX_INTERRUPT_HANDLER(dma9_sport2_tx_irq){
    unsigned short temp=0;
    
    
      	// confirm interrupt handling
		*pDMA9_IRQ_STATUS = 0x0001;   	
    	//while ((*pDMA9_IRQ_STATUS & 0x0008) !=0 ) {// wait till DMA finished & FIFO empty        
	    //}
	   	    
		// switch buffer and re-enable DMA:
	    if (ActiveBufferTxIs_A == true) {
	        ActiveBufferTxIs_A  	= false;
	        *pDMA9_START_ADDR	= &TxBuffer_B[0][0];
	        g_pTxBuffer4wIf	 		= &TxBuffer_A[0][0];
	    }
	    else{
	        ActiveBufferTxIs_A  	= true;
	        *pDMA9_START_ADDR	= &TxBuffer_A[0][0];
	        g_pTxBuffer4wIf 			= &TxBuffer_B[0][0];
	    }        
       
	    *pDMA9_CONFIG = 0x0095;
	    ssync();    
	    Fwire_data_Txsent = true;
}

/*****************************************************************************
  Function:		sport2_start4wireInterface
  Description:	Start 4 Wire  				
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/

void sport2_start4wireInterface(void) {
    
    // first start rx, second tx (do not modify sequence)
 	sport2_startRx4wireInterface();
    sport2_startTx4wireInterface();      
}

/*****************************************************************************
  Function:		sport2_stop4wireInterface
  Description:	Stops 4 Wire  				
  Inputs:		none
  Outputs:		none
  Return:		none
  Globals:      none
  Important:    ----
*******************************************************************************/

void sport2_stop4wireInterface(void) {
    
 	sport2_stopRx4wireInterface();
    sport2_stopTx4wireInterface();      
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Dummy handling of data:
//		- polling function for availability of data
//		- loopback function
//
bool sport2_4wireDataAvailable(void) {
    
    if (Fwire_data_Rxavailable == true) {
        Fwire_data_Rxavailable  = false;
        return(true);
    }
    
    return(false);
}


void sport2_4wireDataLoopback(void) {
    INT16 i, *pSrc, *pDest,j;
    
    // chose buffers not operated on by DMA
    if (ActiveBufferRxIs_A == true) {		
        pSrc	= &RxBuffer_B[0][0];
        pDest	= &TxBuffer_B[0][0];
    }
    else{
        pSrc	= &RxBuffer_A[0][0];
        pDest	= &TxBuffer_A[0][0];
    }
    // loop back, i.e. copy src to dest:    
    for (i=0;i<(N_CHANNELS_ON_BOARD*BUFFER_SIZE_4W_IF);i++) {
        *pDest++ = *pSrc++;
    } 

}



