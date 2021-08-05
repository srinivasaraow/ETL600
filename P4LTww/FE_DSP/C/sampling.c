/*-------------------------------------------------------------------------*
 * $Workfile: sampling.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Functions to sample AF signals on FE DSP
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/C/sampling.c $ 
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 2     18.11.04 13:21 Maag01
 * 
 * 1     15.11.04 15:52 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "sampling.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/


/* LOCAL VARIABLES ***********************************************************/

static float SEGMENT_EXT_DATA g_sampleBuffer[SAMPLING_SIGNAL_BUF_SIZE];


/* IMPLEMENTATION ************************************************************/

#pragma optimize_off

/*****************************************************************************
  Function:		sampling_init()		
  Description:	Initializes sampling parameters	
  Inputs:		sampling: pointer to sampling parameters			
  Outputs:		-	
  Return:		-
  Globals:		-
  Important:	Has to be called once before using sampling_activate()

*******************************************************************************/

void SEGMENT_EXT_CODE sampling_init(t_Sampling *sampling)
{
	sampling->enabled = FALSE;
	sampling->ptr = NULL;
	sampling->index = 0;

}


/*****************************************************************************
  Function:		sampling_enable()		
  Description:	Enables the sampling process.	
  Inputs:		sourcePtr: pointer to signal sourec; sampling: Pointer to 
				sampling parameters			
  Outputs:		-	
  Return:		-
  Globals:		-
  Important:	-

*******************************************************************************/

void sampling_enable(float *sourcePtr, t_Sampling *sampling)
{
	// set sampling pointer
	sampling->ptr = sourcePtr;

	// enable sampling
	sampling->enabled = TRUE;
}


/*****************************************************************************
  Function:		sampling_activate()		
  Description:	If sampling is enabled tow samples are copied to the sample
				buffer.
  Inputs:		sampling: pointer to sampling parameters
  Outputs:		-	
  Return:		-
  Globals:		-
  Important:	-

*******************************************************************************/

void sampling_activate(t_Sampling *sampling)
{
	if (sampling->enabled) {

		g_sampleBuffer[sampling->index++] = *sampling->ptr;
		g_sampleBuffer[sampling->index++] = *(sampling->ptr + 1);

		if (sampling->index >= SAMPLING_SIGNAL_BUF_SIZE) {
			
			// disable sampling
			sampling->enabled = FALSE;
			sampling->index = 0;
		};
	};
}


/*****************************************************************************
  Function:		sampling_getBufferPtr()		
  Description:	Returns the pointer to the sampling buffer.
  Inputs:		sampling: pointer to sampling parameters
  Outputs:		-	
  Return:		-
  Globals:		-
  Important:	-

*******************************************************************************/

float *sampling_getBufferPtr(void)
{
	return g_sampleBuffer;
}

