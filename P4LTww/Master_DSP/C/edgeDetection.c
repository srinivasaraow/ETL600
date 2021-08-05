/*-------------------------------------------------------------------------*
 * $Workfile: edgeDetection.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Edge detection
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Master_DSP/C/edgeDetection.c $ 
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 2     9.11.04 18:18 Maag01
 * 
 * 1     9.11.04 17:53 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "edgeDetection.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		edgeDetection_init()
  Description:	Init function for detectEdge()
  Inputs:		
  Outputs:		
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/

void SEGMENT_EXT_CODE edgeDetection_init(t_EdgeDetection *param, long prolongation)
{
	if (prolongation > 0) {

		param->prolongation = prolongation;
	}
	else {	// prolongation has to be at least 1 to get TRUE in case of an edge

		param->prolongation = 1;
	};

	param->oldSample = 0;
	param->counter = 0;

}


/*****************************************************************************
  Function:		edgeDetection_activate()
  Description:	Detects edges (positive and negative) on a binary data stream
  Inputs:		sample: Sample of the data stream, *param: pointer to parameters
  Outputs:		None
  Return:		TRUE if edge detected else FALSE. TRUE is hold for a predefined
				number of sampling intervals specified at initialization (varible:
				prolongation).
  Globals:		None
  Important:	Call edgeDetection_init() before using this function.

*******************************************************************************/

BOOL edgeDetection_activate(long sample, t_EdgeDetection *param)
{

	if (sample != param->oldSample){	// edge detected

		param->counter = param->prolongation;
	}
	else if (param->counter > 0) {		// decrease counter

		param->counter--;
	};

	param->oldSample = sample;			// update oldSample

	if (param->counter >0) return(TRUE);
	else return(FALSE);

}
