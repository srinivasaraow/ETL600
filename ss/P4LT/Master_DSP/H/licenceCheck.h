/*-------------------------------------------------------------------------*
 * $Workfile: licenceCheck.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : M. Buhl
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/HMI600/H/licenceCheck.h $
 * 
 * 7     9.05.05 8:41 Chsanoh
 * 
 * 6     3.05.05 14:09 Chsanoh
 * 
 * 5     29.04.05 17:59 Chsanoh
 * 
 * 4     11.04.05 9:51 Chmibuh
 * rename MASK_FOR_TPE to MASK_FOR_LICENCE_PRESENT
 * 
 * 3     10.04.05 17:29 Chmibuh
 * 
 * 2     10.04.05 13:39 Chmibuh
 * 
 * 1     8.04.05 18:01 Chmibuh
 * initial
 *-------------------------------------------------------------------------*/

#ifndef LICENCECHECK_H
#define LICENCECHECK_H

/* INCLUDE FILES **************************************************************/

#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

//										   |--------free for future use
//										   | |------number of MOD600
//										   | | |----number of NSK Modems
//										   | | | |--licence present? 	
//										  fffmnnfp - f=free, m=MOD, n=NSK, p=present   
#define LICENCE_DEF_NO_LICENCE			0x00000000		
#define LICENCE_DEF_0MOD600_0NSK600		0x00000001		//white label  
#define LICENCE_DEF_0MOD600_2NSK600		0x00000201		//blue label
#define LICENCE_DEF_0MOD600_4NSK600		0x00000401		//green label
#define LICENCE_DEF_1MOD600_0NSK600		0x00010001		//yellow label
#define LICENCE_DEF_1MOD600_2NSK600		0x00010201		//orange label
#define LICENCE_DEF_1MOD600_4NSK600		0x00010401		//red label
 
/// Label colours are not definitely defined yet - depends at the available labels!!!!!
#define LICENCE_LABEL_WHITE				LICENCE_DEF_0MOD600_0NSK600
#define LICENCE_LABEL_BLUE				LICENCE_DEF_0MOD600_2NSK600
#define LICENCE_LABEL_GREEN				LICENCE_DEF_0MOD600_4NSK600
#define LICENCE_LABEL_YELLOW			LICENCE_DEF_1MOD600_0NSK600
#define LICENCE_LABEL_ORANGE			LICENCE_DEF_1MOD600_2NSK600
#define LICENCE_LABEL_RED				LICENCE_DEF_1MOD600_4NSK600

#define NO_MOD600						0x00000001
#define ONE_MOD600						0x00010001
#define NO_NSK600						0x00000001
#define TWO_NSK600						0x00000201
#define FOUR_NSK600						0x00000401

#define MASK_FOR_LICENCE_PRESENT		0x0000000F
#define MASK_FOR_NSK600					0x0000FF00
#define MASK_FOR_MOD600					0x000F0000

// Defines for licence ranges
#define LICENCES_FOR_0MOD600_0NSK600	{LICENCE_DEF_0MOD600_0NSK600, LICENCE_DEF_0MOD600_2NSK600, \
										 LICENCE_DEF_0MOD600_4NSK600, LICENCE_DEF_1MOD600_0NSK600, \
										 LICENCE_DEF_1MOD600_2NSK600, LICENCE_DEF_1MOD600_4NSK600}
#define LICENCES_FOR_0MOD600_2NSK600	{LICENCE_DEF_0MOD600_2NSK600, \
										 LICENCE_DEF_0MOD600_4NSK600, \
										 LICENCE_DEF_1MOD600_2NSK600, LICENCE_DEF_1MOD600_4NSK600}
#define LICENCES_FOR_0MOD600_4NSK600	{LICENCE_DEF_0MOD600_4NSK600, \
										 LICENCE_DEF_1MOD600_4NSK600}
#define LICENCES_FOR_1MOD600_0NSK600	{LICENCE_DEF_1MOD600_0NSK600, \
										 LICENCE_DEF_1MOD600_2NSK600, LICENCE_DEF_1MOD600_4NSK600}
#define LICENCES_FOR_1MOD600_2NSK600	{LICENCE_DEF_1MOD600_2NSK600, LICENCE_DEF_1MOD600_4NSK600}
#define LICENCES_FOR_1MOD600_4NSK600	{LICENCE_DEF_1MOD600_4NSK600}	


/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

// This following macro named GENERATE_LICENSE_PARAMETERS can only be used within the
// HMI600 poject since only there the constants IDS_WHITE, IDS_BLUE etc. are defined!
#define GENERATE_LICENSE_PARAMETERS(ulLicense, szColor, iNoMOD600, iNoNSK600) \
switch(ulLicense) \
{ \
	case LICENCE_DEF_0MOD600_0NSK600: 	/* white label */ \
	{ \
		szColor.LoadString(IDS_WHITE); \
		iNoMOD600 = 0; \
		iNoNSK600 = 0; \
		break; \
	} \
\
	case LICENCE_DEF_0MOD600_2NSK600:	/* blue label */ \
	{ \
		szColor.LoadString(IDS_BLUE); \
		iNoMOD600 = 0; \
		iNoNSK600 = 2; \
		break; \
	} \
\
	case LICENCE_DEF_0MOD600_4NSK600:	/* green label */ \
	{ \
		szColor.LoadString(IDS_GREEN); \
		iNoMOD600 = 0; \
		iNoNSK600 = 4; \
		break; \
	} \
\
	case LICENCE_DEF_1MOD600_0NSK600:	/* yellow label */ \
	{ \
		szColor.LoadString(IDS_YELLOW); \
		iNoMOD600 = 1; \
		iNoNSK600 = 0; \
		break; \
	} \
\
	case LICENCE_DEF_1MOD600_2NSK600:	/* orange label */ \
	{ \
		szColor.LoadString(IDS_ORANGE); \
		iNoMOD600 = 1; \
		iNoNSK600 = 2; \
		break; \
	} \
\
	case LICENCE_DEF_1MOD600_4NSK600:	/* red label */ \
	{ \
		szColor.LoadString(IDS_RED); \
		iNoMOD600 = 1; \
		iNoNSK600 = 4; \
		break; \
	} \
\
	default: \
	{ \
		szColor.LoadString(IDS_INVALID); \
		iNoMOD600 = 0; \
		iNoNSK600 = 0; \
		break; \
	} \
} \

/* PUBLIC FUNCTIONS ***********************************************************/
unsigned long licence_read_config(t_P4LT_Config* pP4LTConfig);
BOOL licence_match_config(unsigned long licenceCode, unsigned long licenceNeeded);
BOOL SEGMENT_EXT_CODE licenceInsufficientAlarm(void);


#endif //#ifndef LICENCECHECK_H

