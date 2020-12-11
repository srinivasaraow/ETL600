/*----------------------------------------------------------------------------*
 * $Workfile: SupervisionUC_Bus_FE_DSP.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : BB
 * Remarks      : 
 * Purpose      : Supervision UC bus 
 * Copyright    : ABB Switzerland Ltd., 2006
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /branches/Source HSD600/P4LT/FE_DSP/C/SupervisionUC_Bus_FE_DSP.c $ 
 * 
 * 1     4.04.06 18:01 Chmibuh
 * 
 * 3     15.03.06 11:51 Chbrbus
 * Added DMA supervision UC TDM bus: New functions
 * IncrementDMA_ErrorCounter() and SetDMA_ErrorCounterZero(), tested
 * 
 * 2     14.03.06 16:09 Chbrbus
 * Functions tested - still DMA supervision missing.
 * 
 * 1     10.03.06 15:57 Chbrbus
 * 
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "SupervisionUC_Bus_FE_DSP.h"
#include "TDM_Init.h"


extern volatile long IR_TimerCounter;
// extremely bad code practice, in thil limited time no other solution
extern SEGMENT_EXT_CODE resetDataConverters(void);

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {
	unsigned long		UpCounter_NotaValidAD6620Sample;
	unsigned long		UpCounter_TS2_NotZero;
	unsigned long		UpCounter_TS3_TS7_NotZero;
	unsigned long		UpCounter_DMA_Error;
} t_UpConverterBusSupervisionType;


#define MASK_BIT0_BIT8			0x000000FF
#define MASK_ZERO_TS2			0x00FFFFFF


/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define TRESHOLD_INVALID_AD6620_SAMPLES		10	// after 200 cycles, is called with 1006.6 Hz
#define TRESHOLD_INVALID_TS2_SAMPLES		20	// after 400 cycles, is called with 1006.6 Hz
#define TRESHOLD_INVALID_TS2_TS7_SAMPLES	10	// after 200 cycles, is called with 1006.6 Hz
#define TRESHOLD_NO_DMA_INTERRUPTS			100	// after 100 cycles, is called with 10.666 kHz

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

t_UpConverterBusSupervisionType UpConverterBusSupervisionData = {0, 0, 0, 0};

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/



/*****************************************************************************
  Function:		CheckUpconverterBusOnFE_DSP()
  Description:	Checks the values received from the upconverter bus and counts up
				the errors in the corresponding error counters. This is a non real
				time function and does not check every single sample on the bus (we
				don't have time for this).
  Inputs:		Active Rx pointer upconverter TDM bus
  Outputs:		None
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/



void CheckUpconverterBusOnFE_DSP(volatile long *rxBufferPtr)
{
	bool FoundProblem = FALSE;
	long counter;

	// Check TS0 real and TS1 imaginary sample from AD6620
	for (counter = 0; counter < 2; counter++)
	{
		if (((*(rxBufferPtr + counter)) & MASK_BIT0_BIT8) != 0x00000000) FoundProblem = TRUE;
	}

	if (FoundProblem) UpConverterBusSupervisionData.UpCounter_NotaValidAD6620Sample++;
	else UpConverterBusSupervisionData.UpCounter_NotaValidAD6620Sample = 0;

	// Check TS2 if 24 first  bit are zero
	if (((*(rxBufferPtr + 2)) & MASK_ZERO_TS2) != 0x00000000) UpConverterBusSupervisionData.UpCounter_TS2_NotZero++;
	else UpConverterBusSupervisionData.UpCounter_TS2_NotZero = 0;

	// Check if all other TS are zero
	FoundProblem = FALSE;
	for (counter = 3; counter < 8; counter++)
	{
		if ((*(rxBufferPtr + 3)) != 0x00000000) FoundProblem = TRUE;
	}
	if (FoundProblem) UpConverterBusSupervisionData.UpCounter_TS3_TS7_NotZero++;
	else UpConverterBusSupervisionData.UpCounter_TS3_TS7_NotZero = 0;
}




/*****************************************************************************
  Function:		RestartUC_TDM(void)
  Description:	Restart the SPORT02 if there is a problem detected
  Inputs:		None
  Outputs:		None
  Return:		True if SPORT02 is restarted
  Globals:		
  Important:

******************************************************************************/

bool RestartUC_TDM(void)
{

	if ((UpConverterBusSupervisionData.UpCounter_NotaValidAD6620Sample > TRESHOLD_INVALID_AD6620_SAMPLES) ||
		(UpConverterBusSupervisionData.UpCounter_TS2_NotZero > TRESHOLD_INVALID_TS2_SAMPLES) ||
		(UpConverterBusSupervisionData.UpCounter_TS3_TS7_NotZero > TRESHOLD_INVALID_TS2_TS7_SAMPLES) ||
		(UpConverterBusSupervisionData.UpCounter_DMA_Error > TRESHOLD_NO_DMA_INTERRUPTS) )
	{
		
		UpConverterBusSupervisionData.UpCounter_NotaValidAD6620Sample = 0;
		UpConverterBusSupervisionData.UpCounter_TS2_NotZero = 0;
		UpConverterBusSupervisionData.UpCounter_TS3_TS7_NotZero = 0;
		UpConverterBusSupervisionData.UpCounter_DMA_Error = 0;

		resetDataConverters();		// do a reset of AD6620 and ADS1605
		Restart_SPORT02();

		return (TRUE);				// gives the main() the signal to reload the AD6620 registers
	}
	else return (FALSE);				// Nothing to do
}


// Upconverter bus DMA Supervision
/*****************************************************************************
  Function:		IncrementDMA_ErrorCounter(void)
  Description:	Increments the counter variable for supervision UC TDM bus
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:

******************************************************************************/

void IncrementDMA_ErrorCounter(void)
{
	UpConverterBusSupervisionData.UpCounter_DMA_Error++;
}


/*****************************************************************************
  Function:		SetDMA_ErrorCounterZero(void)
  Description:	Increments the counter variable for supervision UC TDM bus
  Inputs:		None
  Outputs:		None
  Return:		None
  Globals:		
  Important:

******************************************************************************/

void SetDMA_ErrorCounterZero(void)
{
	UpConverterBusSupervisionData.UpCounter_DMA_Error = 0;
}


