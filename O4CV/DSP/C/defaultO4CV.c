/*-------------------------------------------------------------------------*
 * $Workfile	: defaultO4CV.c  $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The module defaultO4CV provides a partial or complete
 *				  O4CV configuration.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/O4CV/DSP/C/defaultO4CV.c $
 * 
 * 10    12-03-27 16:02 Chalnot
 * Disable LEC in default config.
 * 
 * 7     07-09-12 14:17 Chrobif
 * Configuration struct version added.
 * 
 * 6     07-09-05 15:09 Chalnot
 * Changed default basic time slot from 0 to 3 (Slot N34).
 * 
 * 5     23.07.07 15:08 Chhamaa
 * Function  defaultO4CV_getCpTones() removed.
 * 
 * 4     9.07.07 14:21 Chmaflu
 * Change defaults from channel settings (Bug 52)
 * 
 * 3     07-07-06 13:57 Chmaflu
 * Change some default values 
 * 
 * 2     23.04.07 8:59 Chrobif
 * Body implemented.
 * 
 * 1     5.04.07 11:20 Chrobif
 * Initial version.
 * 
 *
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

// C Run-Time Library
#include <string.h>

// O4CV firmware
#include "defaultO4CV.h"
#include "O4CV_Types.h"
#include "O4CV_Const.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
/* LOCAL TYPE DECLARATIONS ***************************************************/
/* LOCAL VARIABLE DECLARATIONS ***********************************************/
/* LOCAL MACRO DEFINITIONS ***************************************************/
/* LOCAL FUNCTION DEFINITIONS ************************************************/
/* IMPLEMENTATION ************************************************************/


/*****************************************************************************
  Function:		void defaultO4CV_getAfInterfaceCfg2wire(t_AfInterfaceCfg *pAfInterfaceCfg)
  Description:	defaultO4CV_getAfInterfaceCfg2wire provides a default 2 wire
  				interface configuration. The configuration is copied to the memory
  				allocated by the caller and specified by the i/p argument.
  Inputs:
  Outputs:		t_AfInterfaceCfg *pAfInterfaceCfg: Pointer to memoery to which a default
  				configuration is copied to.
  Return:       	
  Globals:
  Important:
*******************************************************************************/
void defaultO4CV_getAfInterfaceCfg2wire(t_AfInterfaceCfg *pAfInterfaceCfg)
{
    t_AfInterfaceCfg afInterfaceCfg;
    
    memset(&afInterfaceCfg, 0, sizeof(afInterfaceCfg));
    
    afInterfaceCfg.mode = AF_INTERFACE_2_WIRE_PAX;
    afInterfaceCfg.ringerFreq = 25;
    afInterfaceCfg.impedanceInd = 1;
    afInterfaceCfg.g168ReturnLoss = 0;
    afInterfaceCfg.filterInd = 0;
    afInterfaceCfg.settings &= ~AF_INTERFACE_EN;
    afInterfaceCfg.settings &= ~AF_INTERFACE_INV_M_WIRE;
    afInterfaceCfg.settings &= ~AF_INTERFACE_INV_E_WIRE;
    afInterfaceCfg.settings &= ~AF_INTERFACE_G168_EN;
    afInterfaceCfg.settings &= ~AF_INTERFACE_FILTER_EN;
    afInterfaceCfg.settings &= ~AF_INTERFACE_LEC_EN;
    afInterfaceCfg.inputGain = 0;
    afInterfaceCfg.outputGain = -700;
     
    memcpy(pAfInterfaceCfg, &afInterfaceCfg, sizeof(t_AfInterfaceCfg));
}

/*****************************************************************************
  Function:		void defaultO4CV_getAfInterfaceCfg4wire(t_AfInterfaceCfg *pAfInterfaceCfg)
  Description:	defaultO4CV_getAfInterfaceCfg4wire provides a default 4 wire
  				interface configuration. The configuration is copied to the memory
  				allocated by the caller and specified by the i/p argument.
  Inputs:
  Outputs:		t_AfInterfaceCfg *pAfInterfaceCfg: Pointer to memoery to which a default
  				configuration is copied to.
  Return:       	
  Globals:
  Important:
*******************************************************************************/
void defaultO4CV_getAfInterfaceCfg4wire(t_AfInterfaceCfg *pAfInterfaceCfg)
{
    t_AfInterfaceCfg afInterfaceCfg;
    
    memset(&afInterfaceCfg, 0, sizeof(afInterfaceCfg));
    
    afInterfaceCfg.mode = AF_INTERFACE_4_WIRE;
    afInterfaceCfg.ringerFreq = 0;
    afInterfaceCfg.impedanceInd = 0;
    afInterfaceCfg.g168ReturnLoss = 0;
    afInterfaceCfg.filterInd = 7;
    afInterfaceCfg.settings &= ~AF_INTERFACE_EN;
    afInterfaceCfg.settings &= ~AF_INTERFACE_INV_M_WIRE;
    afInterfaceCfg.settings &= ~AF_INTERFACE_INV_E_WIRE;
    afInterfaceCfg.settings &= ~AF_INTERFACE_G168_EN;
    afInterfaceCfg.settings &= ~AF_INTERFACE_FILTER_EN;
    afInterfaceCfg.settings &= ~AF_INTERFACE_LEC_EN;
    afInterfaceCfg.inputGain = -350;
    afInterfaceCfg.outputGain = -350;
     
    memcpy(pAfInterfaceCfg, &afInterfaceCfg, sizeof(t_AfInterfaceCfg));
}

/*****************************************************************************
  Function:		void defaultO4CV_getSevicePhone(t_ServicePhoneCfg *pServicePhoneCfg)
  Description:	defaultO4CV_getSevicePhone provides a default service phone
  				configuration. The configuration is copied to the memory
  				allocated by the caller and specified by the i/p argument.
  Inputs:
  Outputs:		t_ServicePhoneCfg *pServicePhoneCfg: Pointer to memoery
  				to which a default configuration is copied to.
  Return:       	
  Globals:
  Important:
*******************************************************************************/
void defaultO4CV_getSevicePhone(t_ServicePhoneCfg *pServicePhoneCfg)
{
    t_ServicePhoneCfg servicePhoneCfg;
    
    memset(&servicePhoneCfg, 0, sizeof(servicePhoneCfg));
    
    defaultO4CV_getAfInterfaceCfg2wire(&servicePhoneCfg.afInterface);
    servicePhoneCfg.enabled = FALSE;
    
    memcpy(pServicePhoneCfg, &servicePhoneCfg, sizeof(t_ServicePhoneCfg));
}

/*****************************************************************************
  Function:		void defaultO4CV_getChannel(t_ChannelCfg *pChannelCfg)
  Description:	defaultO4CV_getChannel provides a default channel configuration.
  				The configuration is copied to the memory allocated by the caller
  				and specified by the i/p argument.
  Inputs:
  Outputs:		t_ChannelCfg *pChannelCfg: Pointer to memoery to which a default
  				configuration is copied to.
  Return:       	
  Globals:
  Important:
*******************************************************************************/
void defaultO4CV_getChannel(t_ChannelCfg *pChannelCfg)
{
    t_ChannelCfg channelCfg;
    UINT16 i;
    
    memset(&channelCfg, 0, sizeof(channelCfg));
    
    defaultO4CV_getAfInterfaceCfg2wire(&channelCfg.afInterface);
    channelCfg.mode = CH_MODE_OFF;
    for (i = 0; i < O4CV_N_FBFF_DATA_RATES; i++)
    	channelCfg.dataRate[i] = CH_VOICE_DR_5300;
	channelCfg.settings |= CH_INPUT_OUTPUT_FILTER_EN;
	channelCfg.settings &= ~CH_DELAY_BUFFER_EN;
    
    memcpy(pChannelCfg, &channelCfg, sizeof(t_ChannelCfg));
}

/*****************************************************************************
  Function:		void defaultO4CV_getConfig(t_O4CVConfig *pConfig)
  Description:	defaultO4CV_getConfig provides a default O4CV configuration.
  				The configuration is copied to the memory allocated by the caller
  				and specified by the i/p argument.
  Inputs:
  Outputs:		t_O4CVConfig *pConfig: Pointer to memoery to which a default
  				configuration is copied to.
  Return:       	
  Globals:
  Important:
*******************************************************************************/
void defaultO4CV_getConfig(t_O4CVConfig *pConfig)
{
    t_O4CVConfig config;
    UINT16 i;
    
    memset(&config, 0, sizeof(t_O4CVConfig));
    
    for (i = 0; i < N_CHANNELS_ON_BOARD; i++)
    	defaultO4CV_getChannel(&config.channel[i]);

	defaultO4CV_getSevicePhone(&config.servicePhone);
	config.settings |= 0 & BOARD_NUMBER_MASK;
	config.settings |= OP_MODE_BIT;
	config.basicTimeSlot = 3;
	config.configVersion = O4CV_CONFIG_VERSION;
	
    memcpy(pConfig, &config, sizeof(t_O4CVConfig));
}
