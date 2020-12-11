/*-------------------------------------------------------------------------*
 * $Workfile	: licenceCheck_O4CV.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/HMI600/H/licenceCheck_O4CV.h $
 * 
 * 3     20.11.07 16:47 Chsanoh
 * Addition to correction of Bug 82.
 * 
 * 2     20.11.07 9:51 Chsanoh
 * Bug 82 corrected.
 * 
 * 1     07-11-19 11:50 Chalnot
 * Initial version.
 *
 *-------------------------------------------------------------------------*/

#ifndef LICENCECHECK_O4CV_H
#define LICENCECHECK_O4CV_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define LICENCE_DEF_O4CV_2CHANNELS		0x00000210		// 2 Channel licence
#define LICENCE_DEF_O4CV_4CHANNELS		0x00000410		// 4 Channel licence
#define LICENCE_DEF_O4CV_INVALID_LICENSE	    -1		// Invalid license
#define LICENCE_DEF_O4CV_UNKNOWN_LICENSE	    -2		// Unknown license (only used by HMI600)

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

#endif

// This following macro named GENERATE_O4CV_LICENSE_PARAMETERS can only be used within the
// HMI600 poject since only there the constants IDS_CP_LICENSING_2_CHANNEL and
// IDS_CP_LICENSING_4_CHANNEL are defined!
#define GENERATE_O4CV_LICENSE_PARAMETERS(ulLicense, szLicense) \
switch(ulLicense) \
{ \
	case LICENCE_DEF_O4CV_2CHANNELS: 	   /* 2 O4CV channels licensed */ \
	{ \
		szLicense.LoadString(IDS_CP_LICENSING_2_CHANNEL); \
		break; \
	} \
\
	case LICENCE_DEF_O4CV_4CHANNELS:	   /* 4 O4CV channels licensed */ \
	{ \
		szLicense.LoadString(IDS_CP_LICENSING_4_CHANNEL); \
		break; \
	} \
\
	case LICENCE_DEF_O4CV_INVALID_LICENSE: /* Invalid license */ \
	default: \
	{ \
		szLicense.LoadString(IDS_INVALID); \
		break; \
	} \
} \
 

