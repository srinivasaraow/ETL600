/*-------------------------------------------------------------------------*
 * $Workfile: Piggyback.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/P4LT/NSK_DSP/H/Piggyback.h $
 * 
 * 13    11-03-11 18:07 Chhamaa
 * PIGGYBACK_HW_ID_3 = P1LY rubric 2
 * 
 * 12    11-01-11 9:21 Chhamaa
 * PIGGYBACK_HW_ID_2 = P1LY
 * 
 * 5     19.10.05 16:57 Chalgyg
 * Define for P1LXa added.
 * 
 * 4     13.07.05 8:13 Chchlee
 * PIGGYBACK_HW_ID_MASK modified
 * 
 * 3     30.06.05 17:58 Chmibuh
 * 
 * 2     6.06.05 14:44 Chmibuh
 * 
 * 1     6.06.05 14:39 Chmibuh
 *-------------------------------------------------------------------------*/

#ifndef PIGGYBACK_H
#define PIGGYBACK_H

/* INCLUDE FILES **************************************************************/
#include	"basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// It's part of one long - see P4LT_Types.h, EquipmentSWID.HwVersion
#define PIGGYBACK_HW_ID_MASK			(BIT3|BIT4|BIT5)	// We have three bits for piggyback type and 
															// HW version! No separation of type and
															// HW version. 

// defines for PIGGYBACK_HW_ID_MASK
#define PIGGYBACK_HW_ID_0				0		// P1LX
#define PIGGYBACK_HW_ID_1				1		// P1LXa (With LAN support)
#define PIGGYBACK_HW_ID_2				2		// P1LY rubric 1 (no serial ports)
#define PIGGYBACK_HW_ID_3				3		// P1LY rubric 2 (1 V.11, 2 V.24 ports)
#define PIGGYBACK_HW_ID_4				4		// tbd
#define PIGGYBACK_HW_ID_5				5		// tbd
#define PIGGYBACK_HW_ID_6				6		// tbd
#define PIGGYBACK_HW_ID_NO_PIGGYBACK	7		// No subprint plugged


/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

#endif //#ifndef PIGGYBACK_H

