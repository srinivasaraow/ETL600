/*----------------------------------------------------------------------------*
 * $Workfile: SupervisionUC_Bus_UC_DSP.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : BB
 * Remarks      : 
 * Purpose      : Supervision UC bus 
 * Copyright    : ABB Switzerland Ltd., 2006
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 $Log: /branches/Source HSD600/P4LT/UC_DSP/C/SupervisionUC_Bus_UC_DSP.c $ 
 * 
 * 1     4.04.06 17:30 Chmibuh
 * 
 * 3     15.03.06 16:27 Chbrbus
 * Added UC TDM bus DMA supervision. Restarts tested.
 * 
 * 2     14.03.06 16:04 Chbrbus
 * Supervision for UC tested and OK during normal operation.
 * 
 * 1     14.03.06 10:24 Chbrbus
 * 
 * 
 *---------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include <def21161.h>
asm("#include <def21161.h>");
#include "SupervisionUC_Bus_UC_DSP.h"
#include "TDM_Init.h"

// Dirty programming - but saves a couple of calculation cycles
extern volatile unsigned long UC_Bus_DMA_ErrorCounter;

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {
    unsigned long       UpCounter_TS2_TS7_NotZero;
    unsigned long       UpCounter_NotaValidUpconverterSample;
} t_UpConverterBusSupervisionType;



/* LOCAL CONSTANT DECLARATIONS ***********************************************/
#define TRESHOLD_INVALID_TS2_TS7_SAMPLES    10	// max 400 cycles @ FE DSP, here 9 * 10 * "10" cycles
#define TRESHOLD_INVALID_DMA_CYCLES_UC_BUS	200	// max 200 cycles @ 10.666 kHz, UC bus DMA no longer working

/* LOCAL VARIABLE DECLARATIONS ***********************************************/

t_UpConverterBusSupervisionType UpConverterBusSupervisionData = {0, 0};

/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/



/*****************************************************************************
  Function:     CheckUpconverterBusOnUC_DSP()
  Description:  Checks the values received from the upconverter bus and counts up
                the errors in the corresponding error counters. This is a non real
                time function and does not check every single sample on the bus (we
                don't have time for this).
  Inputs:       Active Rx pointer upconverter TDM bus
  Outputs:      None
  Return:       None
  Globals:      None
  Important:    This is optimized for upconverter low computational cycle left.
                We can't do more with the computational cycle left (e.g. checking
                the received float IQ samples from FE DSP)

*******************************************************************************/



void CheckUpconverterBusOnUC_DSP(volatile long *rxBufferPtr)
{
    bool FoundProblem = FALSE;
    static unsigned long counter = 2;

    // Check if all other TS are zero (TS2-TS7)
    if (counter < 8)
    {
        if ((*(rxBufferPtr + counter)) != 0x00000000) FoundProblem = TRUE;
        counter ++;
    }
    else
    {
        if (FoundProblem) UpConverterBusSupervisionData.UpCounter_TS2_TS7_NotZero++;
        else UpConverterBusSupervisionData.UpCounter_TS2_TS7_NotZero = 0;
        counter = 2;
    }
	// check the value range of the upconverter samples could no be implemented due to
	// calculation time restrictions on the UC DSP. FloatCheck is at least doing a float
	// check, but there is no error counter implemented.
}




/*****************************************************************************
  Function:     RestartUC_TDM(void)
  Description:  Restart the SPORT02 if there is a problem detected
  Inputs:       None
  Outputs:      None
  Return:       None
  Globals:      
  Important:

******************************************************************************/

void RestartUC_TDM(void)
{
    if ((UpConverterBusSupervisionData.UpCounter_TS2_TS7_NotZero > TRESHOLD_INVALID_TS2_TS7_SAMPLES) ||
		(UC_Bus_DMA_ErrorCounter > TRESHOLD_INVALID_DMA_CYCLES_UC_BUS))
	{
        UpConverterBusSupervisionData.UpCounter_NotaValidUpconverterSample = 0;
        UpConverterBusSupervisionData.UpCounter_TS2_TS7_NotZero = 0;
		UC_Bus_DMA_ErrorCounter = 0;

        Restart_SPORT02();
    }
}

