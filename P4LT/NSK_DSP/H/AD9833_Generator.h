/*---------------------------------------------------------------------------*
 * $Workfile: AD9833_Generator.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz PTUKT
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/AD9833_Generator.h $
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 4     19.10.04 17:28 Rytz01
 * 
 * 3     18.10.04 17:47 Buhl01
 * 
 * 2     20.09.04 17:00 Rytz01
 * 
 * 1     15.09.04 14:38 Rytz01
 * Initial version
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef AD9833_GENERATOR_H
#define AD9833_GENERATOR_H

/* INCLUDE FILES *************************************************************/

/* PUBLIC TYPE DECLARATIONS **************************************************/

/* PUBLIC CONSTANT DECLARATIONS **********************************************/

/* PUBLIC MACRO DEFINITIONS **************************************************/

/* PUBLIC FUNCTIONS **********************************************************/
//void calculateFrequencyForDummyload(void);
void SEGMENT_EXT_CODE initAD9833_DDS(t_P4LT_Config *g_p_Cfg);
void SEGMENT_EXT_CODE ActivateAD9833(void);
void SEGMENT_EXT_CODE DeActivateAD9833(void);


#endif

