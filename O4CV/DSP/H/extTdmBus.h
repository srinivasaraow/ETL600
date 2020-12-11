/*-------------------------------------------------------------------------*
 * $Workfile	: extTdmBus.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : This module implements the external TDM bus services.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/extTdmBus.h $
 * 
 * 6     08-02-15 10:50 Chrobif
 * + extTdmBus_getReadDuSlicIntFlag() added.
 * 
 * 5     07-09-04 16:37 Chalnot
 * extTdmBus_setRdyLedBlink() added.
 * 
 * 4     07-08-31 12:45 Chrobif
 * extTdmBus_setReadDuSlicIntFlag() implemented.
 * 
 * 3     16.08.07 19:51 Chhamaa
 * New function added:  extTdmBus_getRestartFlag()
 * 
 * 2     07-07-20 16:16 Chrobif
 * Various BTS_X defines added.
 * 
 * 1     22.05.07 17:07 Chrobif
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
#ifndef EXT_TDM_BUS_H
#define EXT_TDM_BUS_H

/* INCLUDE FILES **************************************************************/
#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
#define BTS_MOD600_DR_INDEX_MASK 			(BIT6 | BIT7 | BIT8) // mask for MOD600 data rate index in basic timeslot
#define BTS_MOD600_DR_INDEX_LSB_POSITION	6 // position of LSB of MOD600 data rate index
#define BTS_MOD600_DR_INDEX_OUT_OF_SYNC		7
#define BTS_MOD600_DR_INDEX_PROFILE_1		0
#define BTS_MOD600_DR_INDEX_PROFILE_2		1
#define BTS_MOD600_DR_INDEX_PROFILE_3		2
#define BTS_MOD600_DR_INDEX_PROFILE_4		3
#define BTS_MOD600_DR_INDEX_PROFILE_5		4

/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/
extern void extTdmBus_init(void);
extern void extTdmBus_activate(UINT16 *pExtTdmRxBuffer, UINT16 *pExtTdmTxBuffer);
extern bool extTdmBus_getRestartFlag(void);
extern void extTdmBus_setReadDuSlicIntFlag(bool flag);
extern bool extTdmBus_getReadDuSlicIntFlag(void);
extern void extTdmBus_setRdyLedBlinking(bool state);

#endif
