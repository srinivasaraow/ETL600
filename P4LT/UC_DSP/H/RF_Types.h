/*--------------------------------------------------------------------------
 * $Workfile: RF_Types.h $
 * Part of		: ETL600 / System
 * Language		: C
 * Created by	: 
 * Remarks		:  
 * Purpose		: RF global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/H/RF_Types.h $
 * 
 * 18    12.10.05 16:26 Chchlee
 * Cleaned up
 * 
 * 17    9.03.05 11:12 Chhesch5
 * 
 * 16    23.09.04 11:00 Maag01
 * 
 * 15    1.07.04 17:32 Nohl01
 * 
 * 14    3.06.04 17:31 Nohl01
 * 
 * 13    19.05.04 11:51 Nohl01
 * 
 * 12    18.05.04 17:54 Nohl01
 * 
 * 11    12.05.04 17:51 Nohl01
 * 
 * 10    28.04.04 12:08 Leeb02
 * 
 * 9     20.04.04 15:41 Maag01
 * 
 * 8     8.04.04 17:30 Nohl01
 * 
 * 7     8.04.04 10:19 Nohl01
 * 
 * 6     7.04.04 11:10 Leeb02
 * 
 * 5     30.03.04 17:34 Nohl01
 * 
 * 4     29.03.04 17:39 Nohl01
 * None-bitmask variables of type UINT8 changed to INT32.
 * 
 * 3     16.03.04 16:37 Nohl01
 * renaming of HF_Bandwidth --> RF_Bandwidth
 * 
 * 2     16.03.04 16:34 Nohl01
 * typedef  t_RF_Config completed by the following variable for porting
 * reasons:
 * UINT8   HF_Bandwidth;   // just temporary for porting reasons
 * 
 * 1     5.03.04 11:15 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/


#ifndef RF_TYPES_H
#define RF_TYPES_H


/* INCLUDE FILES ***********************************************************/

#include "basetype.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

// Defines for "long channelMode"
#define	RF_CHANNEL_MODE_ERECT		(long) 0	// ETL600 overall erect channel mode		// used by HMI600
#define RF_CHANNEL_MODE_INVERTED	(long) 1	// ETL600 overall inverted channel mode		// used by HMI600


/* GLOBAL TYPE DECLARATIONS ************************************************/

typedef	struct {

	long	txFrequency;			// Tx nominal Frequency [Hz]
	long	rxFrequency;			// Rx nominal Frequency [Hz]
	long	PEP_Value;				// Peak Envelope Power [dBm]
	float	sumSValuesInv;			// 1/(Sum of S-Values) 
	long	channelMode;			// constants RF_CHANNEL_MODE_... 
	float	txLevelAdjustment; 		// commissioning input value

} t_RF_Config;	// used by HMI600


#endif
