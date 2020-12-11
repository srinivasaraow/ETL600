/*-------------------------------------------------------------------------*
 * $Workfile	: licenceCheck.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/C/licenceCheck.c $
 * 
 * 1     07-04-30 14:19 Chalnot
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
  
/* INCLUDE FILES *************************************************************/

#include "BASETYPE.H"
#include "Basetype_O4CV.h"
#include "O4CV_Const.h"
#include "O4CV_Types.h"
#include "licenceCheck.h" 
#include "configuration.h" 
#include "iButHigh.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/

/***************************************************************************************************
Function:		licenceCheck_verifyLicence()
Description:	Compares the licenceCode of the ibutton with the licenceNeeded which depends
				on the number of activated channels in the configuration  
Inputs:			None
Outputs:		None
Return:			TRUE if licenceCode of the ibutton is sufficient
Globals:		None
Important:		Only call this function if iButH_isIButtonValid was successful
***************************************************************************************************/
bool licenceCheck_verifyLicence()
{
	UINT32 licenceNeeded;
	UINT32 licenceCode;
	UINT16 numberOfChannels = 0;
	
    // Get the licence code from the ibutton
	licenceCode = iButH_getLicenceCode();
	
    // Get the number of activated channels defined in the configuration
	numberOfChannels = configuration_getNumberOfChannels();
	
	// Define the needed licence
	licenceNeeded = LICENCE_DEF_O4CV_4CHANNELS;
	
	if(numberOfChannels <= 2)
	{
	  	licenceNeeded = LICENCE_DEF_O4CV_2CHANNELS;  
	}
	
	// Check if the licence of the ibutton is sufficient
	if(licenceCode == LICENCE_DEF_O4CV_4CHANNELS)			
	{
		return(TRUE);
	}
	else										
	{  
    	if(licenceCode == licenceNeeded)		
		{
			return(TRUE);	
		}
		else
		{
			return(FALSE);
		}
	}   
}

