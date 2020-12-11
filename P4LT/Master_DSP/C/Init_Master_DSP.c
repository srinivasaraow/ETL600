/*---------------------------------------------------------------------------*
 * $Workfile: Init_Master_DSP.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz
 * Remarks      : 
 * Purpose      : Timer 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /branches/Source HSD600/P4LT/Master_DSP/C/Init_Master_DSP.c $ 
 * 
 * 10    29.03.06 11:01 Chmibuh
 * init MSGR4
 * 
 * 8     3.10.05 11:23 Chmibuh
 * comments added
 * 
 * 7     27.09.05 16:11 Chchlee
 * 
 * 6     17.05.05 18:10 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 5     23.02.05 15:53 Chmibuh
 * 
 * 4     15.02.05 12:06 Chmibuh
 * 
 * 3     2.02.05 17:34 Chmibuh
 * Prepared for Prototype 2 HW & Release HW (SPI Problem)
 * 
 * 2     20.01.05 16:00 Chmibuh
 * Adapt Flags for Prototype 2 HW and Set_DCD7005 removed
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 26    9.12.04 19:41 Chmibuh
 * IO_BUFFER_READ added
 * 
 * 25    6.12.04 18:36 Chmibuh
 * Prepared for Emergency FW
 * 
 * 24    18.10.04 17:46 Buhl01
 * 
 * 23    20.09.04 14:24 Rytz01
 * Init the frequency register for the AD9833 Generator
 * 
 * 22    6.09.04 11:54 Leeb02
 * 
 * 21    26.08.04 8:53 Leeb02
 * tx channel mask override implemented
 * 
 * 20    24.08.04 11:58 Leeb02
 * temporary version for tx_channel_mask_0
 * 
 * 19    24.08.04 10:57 Buhl01
 * Set Flag2 as output
 * 
 * 18    3.08.04 14:46 Buhl01
 * disable write protection of FLASH
 * 
 *---------------------------------------------------------------------------*/

// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION
//
// If you add something, either if you write new code or comment-in previously commented-out 
// code fragments than bracket it with 
// #ifndef EMERGENCY_FW
//		"your code"
// #endif
//
// This prevents the emergency FW from needless code. The size of emergency FW is limited!!!
// This belongs to msgHandler.c, bridge.c, SPI_functions.c and Init_Master_DSP only.
//
// ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION - ATTENTION

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "idleCounter.h"
#include "Timer.h"
#include "Constant.h"
#include "IO_HW_Buffer.h"
#include "ExternalTDM_BusBridge.h"
#include "P4LT_Types.h"

/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/

// must be moved later
//t_P4LT_Config ActualP4LT_Config;


extern volatile long IR_BlockCounter;

/*****************************************************************************
  Function:		void Init_DSP_Register(void)
  Description:	Initialize SDRAM's and Registers.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	incl. setting of all waitstates
******************************************************************************/

void Init_DSP_Register(void)
{
    // set (WAIT) waitstate register
	asm("ustat1 = 0;");      
	asm("bit set ustat1 RBWST7|RBWA0|EB2WS4|EB2S2|EB1WS7|EB1A0|EB0WS0|EB0A0;");
    asm("dm(WAIT) = ustat1;");
    // set (SDRDIV) SDRAM refresh rate register
	asm("ustat1 = 0x05F7;");			// refresh rate P4LT Settings
    asm("dm(SDRDIV) = ustat1;");
	// set (SDCTL) SDRAM control register
	asm("ustat1 = 0;");
	asm("bit set ustat1 SDTRCD2|SDCKRx1|SDBN4|SDEM0|SDSRF|SDPSS|SDPGS256|SDTRP2|SDTRAS5|DSDCK1|SDCL2;");
	asm("bit clr ustat1 SDBUF|SDEM3|SDEM2|SDEM1|SDPM|DSDCTL;");
//	asm("ustat1=0x0231C25A;");			// hex value of settings above - use to set SDRAM registers for emulator use 
    asm("dm(SDCTL)=ustat1;");	
	// configure External port for 48-bit writes (disable linkports)
	asm("ustat1 = dm(SYSCON);");
	asm("bit set ustat1 IPACK0 | EBPR10;");
	asm("dm(SYSCON) = ustat1;");
	
	//Disable nested Interupts
	asm("bit clr mode1 NESTM;");    
	// ****************** configure IO FLAGS *********************************** 
	// for Prototype 2 HW use #define P4LT_PROTOTYPE2HW - same as in spi_dsp.h, SPI_access.h & IrigB.c

#ifdef P4LW_PTTY2													// set FLAG 0 & 3 as input, FLAG0 --> IRIG (in)					
	asm("bit clr mode2  FLG0O|FLG3O;");								// FLAG1 --> N-SPI-CS-EN (out)
																	// FLAG2 --> uP Flag  (out) 
																	// FLAG3 --> Master DSP detect busy FLASH (in)
	asm("nop;");
	asm("bit set mode2  FLG1O|FLG2O;");								// Set FLAG 1 & 2 as output 
	asm("nop;");												
	asm("bit set FLAGS  FLG1|FLG2;");								// Set FLAG 1 & 2 high

																	// FLAG4 -->  SPI-CS-A0#1 (out)					
																	// FLAG5 -->  SPI-CS-A1#1 (out)
																	// FLAG6 -->  SPI-CS-A2#1 (out) 
																	// FLAG7 -->  DEBUG_FLAG     (in)
																	// FLAG8 -->  N-SPI-CS_DDS#1 (out)					
																	// FLAG9 -->  EMERGENCY_FLAG (in)
																	// FLAG10 --> N-WP-FLASH#1   (out) 
																	// FLAG11 --> N-OUT_BUF_EN#1 (out)

	*(long*)(IOFLAG) |= FLG11O|FLG10O|FLG8O|FLG6O|FLG5O|FLG4O;		// set Flags (4-6,8,10-11) as output
	*(long*)(IOFLAG) |= FLG11|FLG10|FLG8|FLG6|FLG5|FLG4;			// set all output Flags  High
#else
																	// FLAG0 --> SPI-CS-A0#1 (out)					
																	// FLAG1 --> SPI-CS-A1#1 (out)
																	// FLAG2 --> SPI-CS-A2#1 (out) 
																	// FLAG3 --> N-SPI-CS-EN (out)
	asm("bit set mode2  FLG0O|FLG1O|FLG2O|FLG3O;");					// Set FLAG 0,1,2 & 3 as output 
	asm("nop;");												
	asm("bit set FLAGS  FLG0|FLG1|FLG2|FLG3;");						// Set FLAG 0,1,2 & 3 high

																	// FLAG4 -->  FLAG_UC-DSP#1  (out)					
																	// FLAG5 -->  IRIG-B#1       (in)
																	// FLAG6 -->  READY-FLASH#1  (in) 
																	// FLAG7 -->  DEBUG_FLAG     (in)
																	// FLAG8 -->  ??? neu Adressdecoder N-SPI-CS_DDS#1 (out)					
																	// FLAG9 -->  EMERGENCY_FLAG (in)
																	// FLAG10 --> N-WP-FLASH#1   (out) 
																	// FLAG11 --> N-OUT_BUF_EN#1 (out)
	*(long*)(IOFLAG) |= FLG11O|FLG10O|FLG4O;						// set Flags (4,10 & 11) as output
	*(long*)(IOFLAG) |= FLG11|FLG10|FLG4;							// set all output Flags  High
#endif

// D23 = WR iButton must be 0
	IO_BUFFER_WRITE = 0x0000FF7F;									// Init Outbuffer, MSGR0
	IO_BUFFER_READ = 0x00000000;									// Init Inbuffer, MSGR1

	*(long*)(MSGR4) = 0;											// Init reg for R1LA reset, MSGR4

	IO_BUFFER_WRITE &= ~(P4LT_AL_LED);
	*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE;						// Write to OutBuffer
	*(long*)(IOFLAG) &= ~(FLG11);									// clear Flag 11 -> enable IO_BUFFER
}


/*****************************************************************************
  Function:		void initialize_DSP(void)
  Description:	Initialize DSP's.
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:	
******************************************************************************/
void initialize_DSP(void)
{
#ifndef EMERGENCY_FW	
	long DelayCounter;

	unsigned long externalTDMBusTxChannelMask_TimeSlot_0_31;

	// Add HW-ID, Memory Check, ect.
	IO_BUFFER_READ = *(long*)IO_BUFFER_DSP;									// As from now read/mask HW-ID from IO_BUFFER_READ

	// Init external TDM bus before SPORT02 - depending from config
	intExtTDMBusBridge_initMapping(&externalTDMBusTxChannelMask_TimeSlot_0_31);

	tdmInit_OverrideTxChannelMask02_0(externalTDMBusTxChannelMask_TimeSlot_0_31);
#endif

	asm("bit set IMASK IRQ0I;");											// enable Interrupt 0 for Atmel(UART) communication

#ifndef EMERGENCY_FW	
	InitSPORT02();
#endif

	InitSPORT13();

#ifndef EMERGENCY_FW	
	Start_SPORT02();
#endif

	Start_SPORT13();

	///// extract global interrupt enable from Start_SPORT02()
	//////////////////////////////////////////////////////////
	

	IO_BUFFER_WRITE &= ~(N_EN_CLK_INT_TDM | N_EN_CLK_EXT_TDM);				// Enable the N_EN_CLK_INT_TDM and N_EN_CLK_EXT_TDM and the READY_LED
	*(long*)IO_BUFFER_DSP = IO_BUFFER_WRITE;								// Write to OutBuffer
		
#ifndef EMERGENCY_FW	
	// After a delay, start the Tx DMAs (keep order in DMA buffer)
	for (DelayCounter = 0; DelayCounter < 900; DelayCounter++)
	{
	}
	StartChainedDMA_SPORT13Tx();
	StartChainedDMA_SPORT02Tx();


	// this DSP has 10 maincases and 10 subcases. The mainloop runs with 10.667kHz
	idleCounter_init(DSP_CYCLES_PER_MAINLOOP, N_MAINCASES_10K, N_SUBCASES_10K);
#endif	

	init_timer();
}

