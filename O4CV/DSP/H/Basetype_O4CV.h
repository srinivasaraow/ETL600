/*-------------------------------------------------------------------------*
 * $Workfile	: Basetype_O4CV.h  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2006
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/Basetype_O4CV.h $
 * 
 * 6     18-10-11 16:35 Chthbae
 * Changes for easy update to new version of visual studio
 * 
 * 3     07-11-27 13:30 Chrobif
 * Debug telephony infrastructure added.
 * 
 * 2     07-08-23 16:50 Chrobif
 * Limits of basic data types added.
 * 
 * 1     9.01.07 15:57 Chhamaa
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef BASETYPE_O4CV_H
#define BASETYPE_O4CV_H

/* COMPILER SWITCHES *********************************************************/

// Switch on/off telephony debug infrastructure
// If DEBUG_TELEPHONY is defined the following output pins carry the following signals of channel 1:
// 	(1) e_Buf_EW1 carries the received M-wire signal.
// 	(2) e_Buf_EW2 toggles each time a signalling frame was send.
// 	(3) e_Buf_EW3 carries the received E-wire signal.
// 	(4) e_Buf_EW4 toggles each time a signalling frame was received.
//#define DEBUG_TELEPHONY

/* INCLUDE FILES **************************************************************/
#include <limits.h>

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// basic data types
#define INT8	char			//  8 bits signed
#define INT16	short			// 16 bits signed
#define INT32	int				// 32 bits signed

#define UINT8	unsigned char	//  8 bits unsigned	
#define UINT16	unsigned short	// 16 bits unsigned
#define UINT32	unsigned int	// 32 bits unsigned

#ifndef DONOTUSEO4CVTYPES
// limits of basic data types
#define INT8_MIN	SCHAR_MIN
#define INT8_MAX	SCHAR_MAX
#define INT16_MIN	SHRT_MIN
#define INT16_MAX	SHRT_MAX
#define INT32_MIN	INT_MIN
#define INT32_MAX	INT_MAX

#define UINT8_MAX	UCHAR_MAX
#define UINT16_MAX	USHRT_MAX
#define UINT32_MAX	UINT_MAX
#endif

#ifndef BOOL

	#ifndef _WINDEF_
		
		#define BOOL unsigned char
		
	#endif

#endif

#define ON		1
#define OFF		0

#endif

