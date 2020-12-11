/*-------------------------------------------------------------------------*
 * $Workfile	: configuration.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/C/configuration.c $
 * 
 * 25    12-03-28 10:55 Chalnot
 * configuration_isLecEnabled added.
 * 
 * 23    22.09.08 14:32 Chhamaa
 * Changes merged from O4CV branch.
 * 
 * 22    08-06-16 14:12 Chrobif
 * Debugging of Problem 447 showed that voiceData_receive() is not robust
 * to corrupt data rate indices in basic time slot. This modification
 * improves this aspect.
 * 
 * 21    08-01-17 8:26 Chrobif
 * Some comments added.
 * 
 * 20    07-09-13 14:54 Chrobif
 * configuration_getFxsCfg() added.
 * 
 * 19    07-09-13 8:41 Chalnot
 * Function configuration_getActiveChannels() completed.
 * Config version check implemented.
 * 
 * 18    07-09-10 9:16 Chalnot
 * Description of configuration_init() changed.
 *
 * 17    07-09-10 9:11 Chalnot
 * Default config not stored to flash during init.
 *
 * 16    07-09-06 8:37 Chalnot
 * Set warning when default config active.
 * 
 * 15    07-09-04 14:15 Chalnot
 * configuration_getBasicTimeSlotID() added.
 * 
 * 14    27.08.07 14:53 Chhamaa
 * New function added: configuration_isChannelOn()
 * 
 * 13    07-08-23 16:48 Chrobif
 * configuration_is2wireConfigured() added.
 * 
 * 12    16.08.07 18:58 Chhamaa
 * New functions added: configuration_isMWireInverted(),
 * configuration_isEWireInverted()
 * 
 * 11    25.07.07 12:06 Chhamaa
 * configuration_getDataRate() added.
 * 
 * 10    07-07-17 7:28 Chrobif
 * Functions configuration_get2WireIfType() and
 * configuration_getAfInterfaceMode() added.
 * 
 * 9     9.07.07 15:58 Chhamaa
 * New functions added: configuration_isServicePhoneActive(),
 * configuration_is2wireInterface()
 * 
 * 8     29.06.07 17:55 Chhamaa
 * Functions configuration_get2wireCfg() and configuration_get4wireCfg()
 * added.
 * 
 * 7     07-06-22 13:19 Chrobif
 * Test code added in configuration_getActiveChannels().
 * 
 * 6     07-06-11 12:53 Chrobif
 * configuration_getActiveChannels() added.
 * 
 * 5     4.05.07 10:31 Chrobif
 * Body of configuration_init implemented and config renamed to gs_config.
 * 
 * 4     2.05.07 18:21 Chhamaa
 * Function configuration_getNumberOfChannels() completed.
 * 
 * 3     23.04.07 14:04 Chhamaa
 * Function configuration_getBoardNumber() added.
 * 
 * 2     5.04.07 9:41 Chrobif
 * Bug fix in configuration_set.
 * 
 * 1     4.04.07 15:08 Chrobif
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
  
/* INCLUDE FILES *************************************************************/
// C Run-Time Library
#include <string.h>
#include <assert.h>

// O4CV firmware
#include "O4CV_Types.h"
#include "Basetype_O4CV.h"
#include "O4CV_Const.h"
#include "configuration.h"
#include "defaultO4CV.h"
#include "epromHigh.h"
#include "status.h"
#include "extTdmBus.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/

static t_O4CVConfig gs_config;


/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/

static bool isChannelEnabled(UINT16 chNr);
static bool isChannelAfInterfaceEnabled(UINT16 chNr);
static bool isServicePhoneEnabled(UINT16 chNr);
static bool isServicePhoneAfInterfaceEnabled(UINT16 chNr);
static bool is4wireInterface(UINT16 chNr);


/* IMPLEMENTATION ************************************************************/

/*****************************************************************************
  Function:		void configuration_init(void)
  Description:	configuration_init reads the configuration data from flash.
  				If the flash is virgin or corrupted or the config versions
  				do not match, default configuration	data is used and a 
  				warning is set.
				It is called once at the beginning in the main function
				before application initialisation.
  Inputs:
  Outputs:
  Return:       	
  Globals:
  Important:	The module configuration depens on the module epromHigh.
  				epromHigh_init() must be called before configuration_init()
  				is called.
*******************************************************************************/
void configuration_init(void)
{
    memset(&gs_config, 0, sizeof(gs_config));
    
    if ( !epromHigh_readConfiguration( (UINT16*) &gs_config ) )
    {
        // The flash is virgin or corrupted. Set up a default config
        // and set the warning default_configuration_active. 
        defaultO4CV_getConfig( &gs_config );
        status_setWarning(O4CV_WARNING_DEFAULT_CONFIGURATION_ACTIVE);
    }
    
    // If the config versions of the config and the fw do not match 
    // set up the default config and set the warning
    // default_configuration_active.
    if ((gs_config.configVersion) != O4CV_CONFIG_VERSION)
	{
		defaultO4CV_getConfig( &gs_config );
		status_setWarning(O4CV_WARNING_DEFAULT_CONFIGURATION_ACTIVE);
	}
}

/*****************************************************************************
  Function:		t_O4CVConfig* configuration_getPCurrentConfig(void)
  Description:	configuration_getPCurrentConfig returns a pointer to the 
  				current configuration data structure.
  Inputs:
  Outputs:
  Return:       t_O4CVConfig*: Pointer to the current configuration data structure	
  Globals:
  Important:                                      	
*******************************************************************************/
t_O4CVConfig* configuration_getPCurrentConfig(void)
{
    return &gs_config;
}

/*****************************************************************************
  Function:		void configuration_set(t_O4CVConfig* srcConfig)
  Description:	configuration_set overwrites the internal O4CV configuration 
  				with the O4CV configuration pointed to by srcConfig.
  Inputs:		t_O4CVConfig* srcConfig
  Outputs:
  Return:
  Globals:
  Important:
*******************************************************************************/
void configuration_set(t_O4CVConfig* srcConfig)
{
    memcpy(&gs_config, srcConfig, sizeof(t_O4CVConfig));
}

/*****************************************************************************
  Function:		UINT16 configuration_getNumberOfChannels(void)
  Description:	configuration_getNumberOfChannels returns the number
  				of active voice channels.
  Inputs:
  Outputs:
  Return:       UINT16: Number of active voice channels	
  Globals:
  Important:
*******************************************************************************/
UINT16 configuration_getNumberOfChannels(void)
{
    UINT16 chNr;
    UINT16 numCh = 0;
    
    
    for (chNr = 0; chNr < N_CHANNELS_ON_BOARD; chNr++)
    {
		if((isServicePhoneEnabled(chNr) && isServicePhoneAfInterfaceEnabled(chNr)) || 
			(isChannelEnabled(chNr) && isChannelAfInterfaceEnabled(chNr)))
		{
			numCh++;
		}  
    }
    
    return numCh;
}

/*****************************************************************************
  Function:		UINT16 configuration_getDataRate(void)
  Description:	Returns the data rate, which is assumed to be the same for
  				all channels.
  Inputs:
  Outputs:
  Return:       UINT16: data rate
  Globals:		gs_config
  Important:
*******************************************************************************/
UINT16 configuration_getDataRate(void)
{
    INT16 i, j;
    
    
    for (j = 0; j < N_CHANNELS_ON_BOARD; j++)
    {
        for (i = 0; i < O4CV_N_FBFF_DATA_RATES; i++)
        {
             if (gs_config.channel[j].dataRate[i] != 0)
             {
                 return(gs_config.channel[j].dataRate[i]);
             }
        }
    }
    
    return(0);
}
             
/*****************************************************************************
  Function:		bool configuration_isServicePhoneActive(void)
  Description:	Checks if service phone and its AF interface are enabled. 
  Inputs:		
  Outputs:
  Return:  		True if service phone is activated, else false.	     
  Globals:		
  Important:
*******************************************************************************/
bool configuration_isServicePhoneActive(void)
{
	bool result = false;
	
	
	if (isServicePhoneEnabled(CH1) && isServicePhoneAfInterfaceEnabled(CH1))
	{
	    result = true;
	}
	
	return result;  
}

/*****************************************************************************
  Function:		bool configuration_isChannelOn(UINT16 chNr)
  Description:	Checks if the specified channel and its AF interface are enabled. 
  Inputs:		
  Outputs:
  Return:  		True if channel and AF interface are enabled, else false.	     
  Globals:		
  Important:
*******************************************************************************/
bool configuration_isChannelOn(UINT16 chNr)
{
	if (isChannelEnabled(chNr) && isChannelAfInterfaceEnabled(chNr))
	{
	    return(true);
	}
	else
	{
	    return(false);
	}    
}

/*****************************************************************************
  Function:		bool configuration_is2wireInterface(UINT16 chNr)
  Description:	Returns true if the specified channel has a 2-wire interface. 
  Inputs:		Channel number
  Outputs:
  Return:  		True if 2-wire interface, else false	     
  Globals:		gs_config	
  Important:
*******************************************************************************/
bool configuration_is2wireInterface(UINT16 chNr)
{
	bool result = false;
    
	
    if ((gs_config.channel[chNr].afInterface.mode == AF_INTERFACE_HOTLINE) 	  ||
    	(gs_config.channel[chNr].afInterface.mode == AF_INTERFACE_REMOTE_SUB) ||
    	(gs_config.channel[chNr].afInterface.mode == AF_INTERFACE_2_WIRE_PAX))
	{
		result = true;
	}
	
	return result;
}

/*****************************************************************************
  Function:		bool configuration_isMWireInverted(UINT16 chNr)
  Description:	Returns true if the m-wire input is inverted. 
  Inputs:		Channel number
  Outputs:
  Return:  		True if m-wire inverted, else false	     
  Globals:		gs_config	
  Important:
*******************************************************************************/
bool configuration_isMWireInverted(UINT16 chNr)
{
    bool result = false;
	
	
	if(gs_config.channel[chNr].afInterface.settings & AF_INTERFACE_INV_M_WIRE)
	{
		result = true;
	}
	   
	return result;
    
}

/*****************************************************************************
  Function:		bool configuration_isEWireInverted(UINT16 chNr)
  Description:	Returns true if the e-wire output is inverted. 
  Inputs:		Channel number
  Outputs:
  Return:  		True if e-wire inverted, else false	     
  Globals:		gs_config	
  Important:
*******************************************************************************/
bool configuration_isEWireInverted(UINT16 chNr)
{
    bool result = false;
	
	
	if(gs_config.channel[chNr].afInterface.settings & AF_INTERFACE_INV_E_WIRE)
	{
		result = true;
	}
	   
	return result;
    
}

/*****************************************************************************
  Function:		bool configuration_isLecEnabled(UINT16 chNr)
  Description:	Checks if the LEC on a specified channel is enabled. 
  Inputs:		Channel number
  Outputs:
  Return:  		True if LEC is enabled, else false	     
  Globals:		gs_config	
  Important:
*******************************************************************************/
bool configuration_isLecEnabled(UINT16 chNr)
{
    bool result = false;
	
	
	if(gs_config.channel[chNr].afInterface.settings & AF_INTERFACE_LEC_EN)
	{
		result = true;
	}
	   
	return result;
    
}

/*****************************************************************************
  Function: e_2WireIfType configuration_get2WireIfType(UINT16 chNr)
  Description:  configuration_get2WireIfType() returns the type of the 2-wire
  				interface.
  Inputs: chNr: channel number
  Outputs:
  Return: e_2WireIfType 		
  Globals: gs_config
  Important: If this function is applied to a 4-wire channel e_Error is returned.
*******************************************************************************/
e_2WireIfType configuration_get2WireIfType(UINT16 chNr)
{
	if ((gs_config.channel[chNr].afInterface.mode == AF_INTERFACE_HOTLINE) 	  ||
		(gs_config.channel[chNr].afInterface.mode == AF_INTERFACE_REMOTE_SUB))
		return e_FXS;
	else if (gs_config.channel[chNr].afInterface.mode ==  AF_INTERFACE_2_WIRE_PAX)
		return e_FXO;
	else
		return e_Error;
}

/*****************************************************************************
  Function:		void configuration_get2wireCfg(t_InterfaceCfg *pIfCfg)
  Description:	Extracts the number of 2 wire interfaces and their indexes
  				from the board configuration. 
  Inputs:		Pointer to t_InterfaceCfg struct.
  Outputs:
  Return:       
  Globals:		gs_config
  Important:
*******************************************************************************/
void configuration_get2wireCfg(t_InterfaceCfg *pIfCfg)
{
	UINT16 i;
	UINT16 nIf = 0;
   
	
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    { 
    	if (isChannelEnabled(i) && isChannelAfInterfaceEnabled(i) && configuration_is2wireInterface(i))
    	{
			pIfCfg->ix[nIf] = i;
			nIf++;
    	}
    }
    
    pIfCfg->nIf = nIf; 
    	    
}

/*****************************************************************************
  Function:		void configuration_getFxsCfg(t_InterfaceCfg *pIfCfg)
  Description:	Extracts the number of 2 wire FXS interfaces and their indexes
  				from the board configuration. 
  Inputs:		Pointer to t_InterfaceCfg struct.
  Outputs:
  Return:       
  Globals:		gs_config
  Important:
*******************************************************************************/
void configuration_getFxsCfg(t_InterfaceCfg *pIfCfg)
{
	UINT16 i;
	UINT16 nIf = 0;
   
	
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    { 
    	if (
				isChannelEnabled(i) && 
				isChannelAfInterfaceEnabled(i) &&
				configuration_is2wireInterface(i) &&
				configuration_get2WireIfType(i) == e_FXS
		   )
    	{
			pIfCfg->ix[nIf] = i;
			nIf++;
    	}
    }
    
    pIfCfg->nIf = nIf; 
    	    
}

/*****************************************************************************
  Function:		bool configuration_is2wireConfigured(UINT16 chNr)
  Description:	configuration_is2wireConfigured() returns true if the specified
  				channel is enabled and 2-wire and false otherwise.
  Inputs:		UINT16 chNr: channel number
  Outputs:
  Return:       True if specified channel is enabled and 2-wire and false otherwise.
  Globals:		gs_config
  Important:
*******************************************************************************/
bool configuration_is2wireConfigured(UINT16 chNr)
{
	if (isChannelEnabled(chNr) && isChannelAfInterfaceEnabled(chNr) && configuration_is2wireInterface(chNr))
		return true;
	else
		return false;
}

/*****************************************************************************
  Function:		void configuration_get4wireCfg(t_InterfaceCfg *pIfCfg)
  Description:	Extracts the number of 4 wire interfaces and their indexes
  				from the board configuration. 
  Inputs:		Pointer to t_InterfaceCfg struct.
  Outputs:
  Return:       
  Globals:		gs_config
  Important:
*******************************************************************************/
void configuration_get4wireCfg(t_InterfaceCfg *pIfCfg)
{
	UINT16 i;
	UINT16 nIf = 0;
   
	
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    { 
    	if (isChannelEnabled(i) && isChannelAfInterfaceEnabled(i) && is4wireInterface(i))
    	{
			pIfCfg->ix[nIf] = i;
			nIf++;
    	}
    }
    
    pIfCfg->nIf = nIf; 
    	        
}


static bool is4wireInterface(UINT16 chNr)
{
	bool result = false;
    
	
    if (gs_config.channel[chNr].afInterface.mode == AF_INTERFACE_4_WIRE)
	{
		result = true;
	}
	
	return result;
}


static bool isChannelEnabled(UINT16 chNr)
{
	bool result = false;
	   
	
	if (gs_config.channel[chNr].mode == CH_MODE_VOICE)
	{
	    result = true;
	}
	
	return result; 
}


static bool isChannelAfInterfaceEnabled(UINT16 chNr)
{
	bool result = false;
	
	
	if(gs_config.channel[chNr].afInterface.settings & AF_INTERFACE_EN)
	{
		result = true;
	}
	   
	return result;    
}


static bool isServicePhoneEnabled(UINT16 chNr)
{
	bool result = false;

	
	if(chNr == CH1)
	{
		if(gs_config.servicePhone.enabled)
		{
			result = true;
		}
	}
	
	return result;   
}


static bool isServicePhoneAfInterfaceEnabled(UINT16 chNr)
{
	bool result = false;
	
	
	if(chNr == CH1)
	{
		if(gs_config.servicePhone.afInterface.settings & AF_INTERFACE_EN)
		{
			result = true;
		}
	}
 
	return result;   
}

/*****************************************************************************
  Function:		INT16 configuration_getBoardNumber(void)
  Description:	configuration_getBoardNumber returns the board number.
  Inputs:
  Outputs:
  Return:       INT16: board number
  Globals:
  Important:
*******************************************************************************/
INT16 configuration_getBoardNumber(void)
{  
   	return(gs_config.settings & BOARD_NUMBER_MASK);
}

/*****************************************************************************
  Function:		UINT8 configuration_getBasicTimeSlotID(void)
  Description:	configuration_getBasicTimeSlot returns the basic time slot id
  				which is defined in the configuration.
  Inputs:
  Outputs:
  Return:       UINT8: basic time slot id 
  Globals:
  Important:
*******************************************************************************/
UINT8 configuration_getBasicTimeSlotID(void)
{  
   	return(gs_config.basicTimeSlot);
}

/*****************************************************************************
  Function:		UINT8 configuration_getAfInterfaceMode(UINT16 chNr)
  Description:	configuration_getAfInterfaceMode() returns the audio frequency (AF)
  				interface mode of the specified channel.
  Inputs:		chNr: Channel number
  Outputs:		None
  Return:       AF interface mode
  Globals:		gs_config
  Important:	-
*******************************************************************************/
UINT8 configuration_getAfInterfaceMode(UINT16 chNr)
{
	return gs_config.channel[chNr].afInterface.mode;
}

/*****************************************************************************
  Function:		void configuration_getActiveChannels(UINT16 mod600DRIndex, bool activeChannels[N_CHANNELS_ON_BOARD))
  Description:	configuration_getActiveChannels() determines, which channels
  				are active based on the current MOD600 data rate.
  Inputs:		UINT16 mod600DRIndex: Current MOD600 data rate index.
  Outputs:		bool activeChannels[N_CHANNELS_ON_BOARD): After configuration_getActiveChannels() returns,
  									   					  the booleans are set to true if the corresponding
  									   					  channel is active. activeChannels[0] corresponds
  									   					  to channel 1.
  Return:       None
  Globals:		gs_config
  Important:	-
*******************************************************************************/
void configuration_getActiveChannels(UINT16 mod600DRIndex, bool activeChannels[N_CHANNELS_ON_BOARD])
{
    UINT32 i;

#ifdef TEST_VOICE_DATA_RX

	switch ( mod600DRIndex )
	{
	    case 0:
	    	activeChannels[0] = false;
	    	activeChannels[1] = false;
	    	activeChannels[2] = false;
	    	activeChannels[3] = false;
	    	break;
	    	
	    case 1:
	    	activeChannels[0] = true;
	    	activeChannels[1] = false;
	    	activeChannels[2] = false;
	    	activeChannels[3] = false;
	    	break;
	    	
	    case 2:
	    	activeChannels[0] = true;
	    	activeChannels[1] = true;
	    	activeChannels[2] = false;
	    	activeChannels[3] = false;
	    	break;
		    	
	    case 3:
	    	activeChannels[0] = true;
	    	activeChannels[1] = true;
	    	activeChannels[2] = true;
	    	activeChannels[3] = false;
	    	break;
		    	
	    case 4:
	    	activeChannels[0] = true;
	    	activeChannels[1] = true;
	    	activeChannels[2] = true;
	    	activeChannels[3] = true;
	    	break;
	   	
	    default:
	    	assert(false);
	}
	
#else

	// Switch off all channels if MOD600 data rate index is invalid
	if ( mod600DRIndex > BTS_MOD600_DR_INDEX_PROFILE_5 )
	{
	    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
	    {
	    	activeChannels[i] = false;
	    }
		return;
	}
	
	// Determine active channels based on MOD600 data rate index
	// and configuration.
	for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    {
	    if (gs_config.channel[i].dataRate[mod600DRIndex] != 0)
	    {
	        activeChannels[i] = true;
	    }
	    else 
	    {
	     	activeChannels[i] = false;   
	    }
    }  
      
#endif

}


