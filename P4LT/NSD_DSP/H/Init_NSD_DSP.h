/*---------------------------------------------------------------------------*
 * $Workfile: Init_NSD_DSP.h $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.P. Rytz PTUKT
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/NSD_DSP/H/Init_NSD_DSP.h $
 * 
 * 1     19.01.05 15:33 Chchlee
 * 
 * 4     9.09.04 18:30 Rytz01
 * In function initialize_DSP parameter added
 * 
 * 3     17.06.04 16:06 Rytz01
 * 
 * 2     17.06.04 15:08 Rytz01
 * 
 * 1     17.06.04 15:04 Rytz01
 * 
 *
 *---------------------------------------------------------------------------*/

#ifndef INIT_NSD_DSP_H
#define INIT_NSD_DSP_H

/* INCLUDE FILES *************************************************************/
/* PUBLIC TYPE DECLARATIONS **************************************************/
/* PUBLIC CONSTANT DECLARATIONS **********************************************/
/* PUBLIC MACRO DEFINITIONS **************************************************/
/* PUBLIC FUNCTIONS **********************************************************/

void initialize_DSP(t_NSD_DSP_Config *Config);
void Init_DSP_Register(void);

#endif

