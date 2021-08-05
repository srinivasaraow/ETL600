/*---------------------------------------------------------------------------*
 * $Workfile: Init_NSK_DSP.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : A. Gygax
 * Remarks      :
 * Purpose      :
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/NSK_DSP/H/Init_NSK_DSP.H $
 * 
 * 2     6.06.05 14:48 Chmibuh
 * putPiggyBackInfoToMaster() added
 * 
 * 1     19.01.05 15:35 Chchlee
 * 
 * 2     18.06.04 9:18 Rytz01
 * 
 * 1     9.06.04 13:00 Gygax02
 * 
 *---------------------------------------------------------------------------*/

#ifndef INIT_NSK_DSP_H
#define INIT_NSK_DSP_H

/* INCLUDE FILES *************************************************************/
/* PUBLIC TYPE DECLARATIONS **************************************************/
/* PUBLIC CONSTANT DECLARATIONS **********************************************/
/* PUBLIC MACRO DEFINITIONS **************************************************/
/* PUBLIC FUNCTIONS **********************************************************/

void initialize_DSP(void);
void Init_DSP_Register(void);
void SEGMENT_EXT_CODE putPiggyBackInfoToMaster(void);

#endif

