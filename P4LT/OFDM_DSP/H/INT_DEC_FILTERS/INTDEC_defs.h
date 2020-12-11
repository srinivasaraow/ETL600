/******************************************************************************
 *
 * PROJECT     : OFDM Modem for ETL600  
 * FILE        : $Workfile: INTDEC_defs.h $
 * COPYRIGHT   : 2003, 2004 TL Consulting GmbH
 * DESCRIPTION : Definitions for Interpolation/Decimation Filters 
 *
 * REVISION HISTORY:
 *-----------------------------------------------------------------------------
 * 
 * 
 * Initial version
 *       21.06.2004 PL
 * Polyphase filter unitialization
 *       12.10.2004 vgp
 *****************************************************************************/
#include "../ofdm_defs.h"

#ifndef INTDEC_FILT_DEFS_H
#define INTDEC_FILT_DEFS_H

#define INTFILT_2_FAC    		 2      // INTERPOLATION_FACTOR         2
#define INTFILT_2_LENGTH 		12      // INTERPOLATION_FILTER_LENGTH 12
#define INTFILT_2_POLYNUMBER     INTFILT_2_FAC 

#define DECFILT_2_FAC	         2
#define DECFILT_2_LENGTH 	    12
#define DECFILT_2_POLYNUMBER  	 DECFILT_2_FAC

#define INTFILT_3_FAC	         3
#define INTFILT_3_LENGTH	    24
#define INTFILT_3_POLYNUMBER     INTFILT_3_FAC



#define INTFILT_4_FAC    		 4      // INTERPOLATION_FACTOR         4
#define INTFILT_4_LENGTH 		24      // INTERPOLATION_FILTER_LENGTH 24
#define INTFILT_4_POLYNUMBER  	 INTFILT_4_FAC

#define DECFILT_4_FAC		     4
#define DECFILT_4_LENGTH 		24
#define DECFILT_4_POLYNUMBER  	 DECFILT_4_FAC

#define INTFILT_5_FAC    		 5      // INTERPOLATION_FACTOR         5
#define INTFILT_5_LENGTH 		25      // INTERPOLATION_FILTER_LENGTH 25
#define INTFILT_5_POLYNUMBER  	 INTFILT_5_FAC

#endif


