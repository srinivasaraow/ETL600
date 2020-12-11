/*-------------------------------------------------------------------------*
 * $Workfile: licenceCheck.c $
 * Part of	: ETL600
 * Language	: C
 * Created by: M.Buhl 
 * Remarks	:  
 * Purpose	: 
 * Copyright: ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/C/licenceCheck.c $
 * 
 * 4     11.04.05 9:51 Chmibuh
 * rename MASK_FOR_TPE to MASK_FOR_LICENCE_PRESENT
 * 
 * 3     10.04.05 17:29 Chmibuh
 * 
 * 2     10.04.05 13:37 Chmibuh
 * licenceInsufficientAlarm() added 
 * 
 * 1     8.04.05 17:58 Chmibuh
 * initial
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES *************************************************************/
#include "default.h"
#include "licenceCheck.h" 
/* LOCAL CONSTANT DECLARATIONS ***********************************************/


/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/


/* LOCAL VARIABLES ***********************************************************/
	static BOOL g_LicenceInsufficientAlarm;


/* LOCAL FUNCTIONS ***********************************************************/


/* IMPLEMENTATION ************************************************************/


/***************************************************************************************************
Function:		licence_read_config()	
Description:	Checks the P4LT/W configuration about number of configured NSK600 Modems and if MOD600 is configured
Inputs:			Pointer to P4LT/W configuration
Outputs:		None
Return:			unsigned long licenceNeeed according the configuration
Globals:		None
Important:		suitable for HMI600
***************************************************************************************************/
unsigned long SEGMENT_EXT_CODE licence_read_config(t_P4LT_Config* pP4LTConfig)
{
	unsigned long licenceNeeed, nskIndex, nskCount;
	
	licenceNeeed = nskCount = 0;

	// Check if MOD600 is configured
	if(pP4LTConfig->OFDM.channelBW > 0)
	{
		licenceNeeed |= ONE_MOD600;
	}

	// Check how many NSK600 Modems are configured
	for(nskIndex = 0;nskIndex < N_NSK_MODEMS;nskIndex++)
	{
		if(pP4LTConfig->NSK.modem[nskIndex].mode & NSK_CFG_MODE_CH_ALLOCATION_BITS)
		{
			nskCount++;
		}
	}
	if(nskCount > 2)
	{
		licenceNeeed |= FOUR_NSK600;
	}
	else if(nskCount > 0)
	{
		licenceNeeed |= TWO_NSK600;
	}
	else
	{
		licenceNeeed |= NO_NSK600;
	}
	return(licenceNeeed);
}


/***************************************************************************************************
Function:		licence_match_config()
Description:	compares licenceCode with licenceNeeded 
Inputs:			licenceCode (got from iButton), licenceNeeded (returned from licence_read_config())
Outputs:		None
Return:			TRUE if licenceCode >= licenceNeeded 
				FALSE if licenceCode < licenceNeeded 
Globals:		None
Important:		Not suitable for HMI600, becaue HMI600 should suggest possible locence Codes
***************************************************************************************************/
BOOL SEGMENT_EXT_CODE licence_match_config(unsigned long licenceCode, unsigned long licenceNeeded)
{
	g_LicenceInsufficientAlarm = FALSE;
	
	if((licenceCode & MASK_FOR_LICENCE_PRESENT) < LICENCE_DEF_0MOD600_0NSK600)	//no valid licence code
	{
		g_LicenceInsufficientAlarm = TRUE;
		return(FALSE);
	}
	else
	{
		if(licenceCode == licenceNeeded)			//licence code matches exactly
		{
			return(TRUE);
		}
		else										//licence code is maybe good enough or to low
		{
			if((licenceCode & MASK_FOR_NSK600) >= (licenceNeeded & MASK_FOR_NSK600))//licence for NSK600 is OK
			{
				if((licenceCode & MASK_FOR_MOD600) >= (licenceNeeded & MASK_FOR_MOD600))//licence for MOD600 is OK
				{
					return(TRUE);
				}
				else
				{
					g_LicenceInsufficientAlarm = TRUE;
					return(FALSE);
				}
			}
			else
			{
				g_LicenceInsufficientAlarm = TRUE;
				return(FALSE);
			}
		}
	}
}


BOOL SEGMENT_EXT_CODE licenceInsufficientAlarm(void)
{
	return g_LicenceInsufficientAlarm;
}
