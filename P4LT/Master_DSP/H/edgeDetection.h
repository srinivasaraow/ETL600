/*----------------------------------------------------------------------------*
 * $Workfile: edgeDetection.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Global type and function declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/edgeDetection.h $
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 2     12.11.04 18:26 Maag01
 * 
 * 1     9.11.04 17:52 Maag01
 * Initial version
 * 
 *
 *----------------------------------------------------------------------------*/
 
#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H


/* INCLUDE FILES **************************************************************/

#include "basetype.h"


/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct {

	long oldSample;
	long counter;
	long prolongation;

} t_EdgeDetection;


/* PUBLIC FUNCTIONS ***********************************************************/

void edgeDetection_init(t_EdgeDetection *param, long prolongation);
BOOL edgeDetection_activate(long sample, t_EdgeDetection *param);


#endif
