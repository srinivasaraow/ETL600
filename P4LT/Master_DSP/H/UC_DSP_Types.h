/*-------------------------------------------------------------------------*
 * $Workfile: UC_DSP_Types.h $
 * Part of      : ETL600 / Up-converter DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/UC_DSP_Types.h $
 * 
 * 2     16.02.05 14:42 Chhamaa
 * 
 * 2     9.02.05 16:46 Chhamaa
 * rfPepMax added
 * 
 * 1     5.03.04 15:35 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef UP_CONVERTER_DSP_TYPES_H
#define UP_CONVERTER_DSP_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include	"basetype.h"
#include	"RF_Types.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/


/* GLOBAL TYPE DECLARATIONS ************************************************/


typedef struct {

	t_RF_Config		RF;
	long			rfPepMax;

} t_UpConverterDSP_Config;



#endif
