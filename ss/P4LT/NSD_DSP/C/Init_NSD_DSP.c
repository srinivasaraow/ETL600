/*---------------------------------------------------------------------------*
 * $Workfile: Init_NSD_DSP.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz
 * Remarks      : 
 * Purpose      : Initialize DSP 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/NSD_DSP/C/Init_NSD_DSP.c $ 
 * 
 * 4     19.05.05 11:15 Chmibuh
 * dummy memoryCheck removed
 * 
 * 3     11.05.05 20:14 Chmibuh
 * 
 * 2     23.02.05 16:35 Chmibuh
 * unchanged FLAGS 4-6 for no iButton
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 16    29.11.04 19:08 Buhl01
 * changed FLAGS 4-6 for iButton
 * 
 * 15    10.09.04 9:09 Plueer01
 * global variable (t_NSD_DSP_Config      Config;) removed
 * 
 * 14    9.09.04 17:16 Rytz01
 * In function MapExternalTDMBusConfigToTimeslotsG4AI parameter added
 * 
 * 13    30.08.04 10:12 Rytz01
 * TDM Bus off-set problem solved
 * 
 * 12    26.08.04 8:53 Leeb02
 * tx channel mask override implemented
 * 
 * 11    24.08.04 13:00 Leeb02
 * MapExternalTDMBusConfigToTimeslotsG4AI: parameter added
 * 
 * 10    20.08.04 15:49 Buhl01
 * shortened dummy mem check again
 * 
 * 9     19.08.04 17:04 Buhl01
 * shortened dummy mem check
 * 
 * 8     11.08.04 11:36 Buhl01
 * dummy memory check added for boot control
 * 
 * 7     26.07.04 16:07 Maag01
 * 
 * 6     25.07.04 13:40 Leeb02
 * adjusted for new idleCounter_init
 * 
 * 5     8.07.04 16:35 Bussli02
 * G4AI static config added
 * 
 * 4     8.07.04 14:07 Plueer01
 * function void initialize_DSP(void) changed -> it is now the same as it
 * is on the FE_DSP
 * 
 * 3     23.06.04 16:10 Rytz01
 * 
 * 2     17.06.04 16:06 Rytz01
 * 
 * 1     17.06.04 15:04 Rytz01
 * Initial version
 *---------------------------------------------------------------------------*/


/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "idleCounter.h"
#include "Timer.h"
#include "Constant.h"
#include "ExternalTDM_BusG4AI_HL.h"
#include "NSD_DSP_Types.h"

/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/

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
    asm("ustat1 = 0;");      
	asm("bit set ustat1 EB0WS0|EB0A0;");
    asm("dm(WAIT) = ustat1;");
    asm("ustat1 = 0x05F7;");          // refresh rate P4LT Settings
    asm("dm(SDRDIV) = ustat1;");
	asm("ustat1 = 0;");
	asm("bit set ustat1 SDTRCD2|SDCKRx1|SDBN4|SDEM0|SDSRF|SDPSS|SDPGS256|SDTRP2|SDTRAS5|DSDCK1|SDCL2;");
	asm("bit clr ustat1 SDBUF|SDEM3|SDEM2|SDEM1|SDPM|DSDCTL;");
//	asm("ustat1=0x0231C25A;");		// Settings for P4LT 
    asm("dm(SDCTL)=ustat1;");	
	// configure External port for 48-bit writes (disable linkports)
	asm("ustat1 = dm(SYSCON);");
	asm("bit set ustat1 IPACK0 | EBPR10;");
	asm("dm(SYSCON) = ustat1;");
	
	//Disable nested Interupts
	asm("bit clr mode1 NESTM;");    


	// configure IOFLAGS 
	asm("bit clr mode2  FLG0O|FLG1O|FLG2O|FLG3O;");						// set FLAG 0-3 as input, FLAG0 --> not used (TP)
																		// FLAG1 --> ID0 (pulled down)
																		// FLAG2 --> ID1 (pulled up)
																		// FLAG3 --> ID2 (pulled up)

*(long*)(IOFLAG) |= FLG11O|FLG10O|FLG9O|FLG8O|FLG7O;          // iButton will be removed from NSD DSP Flag 4,5,6 must be input to avoid conflict with Master DSP
//	*(long*)(IOFLAG) |= FLG11O|FLG10O|FLG9O|FLG8O|FLG7O|FLG6O|FLG5O;	// FLAG4 = Input = RD_1-Wire
																		// FLAG5 = Output= WR_1-Wire
																		// FLAG6 = Output= PU_1-Wire
																		// all unused FLAGS set as Output 	
	*(long*)(IOFLAG) &= ~(FLG5);										// Clear FLAG5
//	*(long*)(IOFLAG) |= FLG11|FLG10|FLG9|FLG8|FLG7|FLG6;				// set all Flags (6-11) High
*(long*)(IOFLAG) |= FLG11|FLG10|FLG9|FLG8|FLG7;
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
void initialize_DSP(t_NSD_DSP_Config *Config)
{
	unsigned long externalTDMBusTxChannelMask_TimeSlot_0_31;

	
	// Add Mem Check, ect.

	MapExternalTDMBusConfigToTimeslotsG4AI(&externalTDMBusTxChannelMask_TimeSlot_0_31, Config);

	tdmInit_OverrideTxChannelMask02_0(externalTDMBusTxChannelMask_TimeSlot_0_31);

	asm("bit set mode1 IRPTEN;");

// Do not change anything below this line *********************************************************************
//
// With this starting procedure (without waiting and without trick!!!) I had no lateral off-set in the internal 
// TDM Bus on NSD!!! (30-08-2004 HPR)
	InitSPORT02();
	InitSPORT13();
	Start_SPORT02();
	Start_SPORT13();

	IR_BlockCounter = 1;
	while (IR_BlockCounter)
	{
	}
	StartChainedDMA_SPORT13Tx_Double();
	StartChainedDMA_SPORT02Tx();
// Do not change anything above this Line *********************************************************************

	// this DSP has 5 maincases and 10 subcases. The mainloop runs with 5.3kHz
	idleCounter_init(DSP_CYCLES_PER_MAINLOOP_5K, N_MAINCASES_5K, N_SUBCASES_5K);

	init_timer();
	// Wait at least 2 cycles to ensure there are no TDM null pointer
	IR_BlockCounter = 3;
	while (IR_BlockCounter)
	{

	}
}



