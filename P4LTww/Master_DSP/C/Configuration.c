/*-------------------------------------------------------------------------*
 * $Workfile: Configuration.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/Master_DSP/C/Configuration.c $ 
 * 
 * 11    3.10.05 18:38 Chmibuh
 * #ifndef EMERGENCY_FW added
 * 
 * 10    10.08.05 9:28 Chchlee
 * configuration version now reduced to 8 bits
 * 
 * 9     25.05.05 17:12 Chchlee
 * Configuration check added
 * 
 * 8     12.04.05 19:52 Chmibuh
 * #ifndef EMERGENCY_FW added - thanks CL for removing it!
 * 
 * 7     12.04.05 8:12 Chchlee
 * String changed again
 * 
 * 6     11.04.05 19:46 Chchlee
 * New default configuration text: ""P4LW DEFAULT CONFIGURATION""
 * 
 * 5     22.02.05 14:52 Chchlee
 * default constructor now common with HMI
 * 
 * 4     21.02.05 19:41 Chchlee
 * temporary configuration warning cleaned up
 * 
 * 3     16.02.05 10:34 Chchlee
 * Upgrade to Ptty2
 * 
 * 2     20.01.05 16:01 Chmibuh
 * #ifndef EMERGENCY_FW added
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 16    3.01.05 9:28 Chchlee
 * 
 * 15    30.12.04 15:13 Chchlee
 * adjusted for new field "systemSettings" in system configuration
 * 
 * 14    16.12.04 14:17 Chchlee
 * transition to new status structure
 * 
 * 13    15.11.04 7:54 Leeb02
 * Status getter removed
 * 
 * 12    4.10.04 9:43 Leeb02
 * Temporary configuration enabled
 * 
 * 11    27.09.04 7:53 Leeb02
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "diag.h"
#include "configuration.h"
#include "epromhi.h"
#include "default.h"
#include "Monitor.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static SEGMENT_EXT_DATA t_P4LT_Config g_CurrentP4LTConfig;
static SEGMENT_EXT_DATA t_P4LT_Config g_NewP4LTConfig;

static SEGMENT_EXT_DATA t_EpromLocalConfiguration g_EpromLocalConfiguration;

static SEGMENT_EXT_DATA t_EqualizerStatus g_equalizerStatus[N_APLC_CHANNELS];

static BOOL bInitCalled;

static BOOL g_configurationIsTemporary;

t_ConfigStructsList g_ConfigStructsList =
						 {  {&g_CurrentP4LTConfig,	      sizeof(g_CurrentP4LTConfig)},
							{&g_EpromLocalConfiguration,  sizeof(g_EpromLocalConfiguration)},
							{&g_equalizerStatus,		  sizeof(g_equalizerStatus)},
							{NULL, 0}};

/* LOCAL FUNCTIONS ***********************************************************/

static void setDefaultCfg(void);

/* IMPLEMENTATION ************************************************************/

void SEGMENT_EXT_CODE configuration_init(void)
{
	bInitCalled = TRUE; // allow getter function to access cfg & status

    // read configuration from flash:
	// reads g_ConfigStructsList from flash if available. Else,
	// if the flash was virgin or corrupted,
	// the default configuration is requested by calling "setDefaultCfg"
	// and stored to flash.

	eprom_init(&g_ConfigStructsList, 
		       &g_configurationIsTemporary,
		       &setDefaultCfg);

#ifndef EMERGENCY_FW	// do not check the version of the configuration, because g_CurrentP4LTConfig is not initialized.
						// Otherwise the startup of the emergency firmware would save the default configuration
	// check the version of the configuration. Use default configuration if not compatible
	if ((g_CurrentP4LTConfig.configVersion & MASK_8LSB) != P4LT_CONFIGURATION_VERSION)
	{
		eprom_saveDefaultConfiguration(&setDefaultCfg);
		g_configurationIsTemporary = FALSE;
	}
#endif

	if (!g_configurationIsTemporary)
	{
		g_CurrentP4LTConfig.system.remainingActiveTestConfigTime = 0; // de-activates time-bomb
	}

	// from now on we save into new configuration
	MEMCPY(&g_NewP4LTConfig, &g_CurrentP4LTConfig, sizeof(g_NewP4LTConfig));

	g_ConfigStructsList[0].address = &g_NewP4LTConfig; // size remains the same

}

static void SEGMENT_EXT_CODE setDefaultCfg(void)
{
	// init structures with default values
	// just a minimum to allow HMI communication

#ifndef EMERGENCY_FW

	InitDefaultConfigP4LT (&g_CurrentP4LTConfig);

	// overwrite equipmentInfo to show that default configuration was produced by P4LW
	// "P4LW DEFAULT CONFIGURATION"
	g_CurrentP4LTConfig.system.equipmentInfo.identification[0] = 'P' | '4'<<SB8 | 'L'<<SB16 | 'W'<<SB24;
	g_CurrentP4LTConfig.system.equipmentInfo.identification[1] = 'D' | 'E'<<SB8 | 'F'<<SB16 | 'A'<<SB24;
	g_CurrentP4LTConfig.system.equipmentInfo.identification[2] = 'U' | 'L'<<SB8 | 'T'<<SB16 | ' '<<SB24;
	g_CurrentP4LTConfig.system.equipmentInfo.identification[3] = 'C' | 'O'<<SB8 | 'N'<<SB16 | 'F'<<SB24;
	g_CurrentP4LTConfig.system.equipmentInfo.identification[4] = 'I' | 'G'<<SB8 | 'U'<<SB16 | 'R'<<SB24;
	g_CurrentP4LTConfig.system.equipmentInfo.identification[5] = 'A' | 'T'<<SB8 | 'I'<<SB16 | 'O'<<SB24;
	g_CurrentP4LTConfig.system.equipmentInfo.identification[6] = 'N' | ' '<<SB8 | ' '<<SB16 | ' '<<SB24;


	//set blank P4LT password (encrypted)
	g_EpromLocalConfiguration.Password[0] = 1748265325;
	g_EpromLocalConfiguration.Password[1] = 1818715956;
	g_EpromLocalConfiguration.Password[2] = 2051362647;
	g_EpromLocalConfiguration.Password[3] = -191;
#endif
}

t_P4LT_Config SEGMENT_EXT_CODE * configuration_getPCurrentConfig(void)
{
	ASSERT(bInitCalled);
	return &g_CurrentP4LTConfig;
}

t_P4LT_Config SEGMENT_EXT_CODE * configuration_getPNewConfig(void)
{
	ASSERT(bInitCalled);
	return &g_NewP4LTConfig;
}

t_EpromLocalConfiguration SEGMENT_EXT_CODE * configuration_getPEpromLocalConfiguration(void)
{
	ASSERT(bInitCalled);
	return &g_EpromLocalConfiguration;
}

t_EqualizerStatus SEGMENT_EXT_CODE * configuration_getPEqualizerStatus(void)
{
	ASSERT(bInitCalled);
	return g_equalizerStatus;
}

BOOL configuration_isTemporary(void)
{
	return g_configurationIsTemporary;
}

void configuration_setConfigurationToPermanent(void)
{
	g_configurationIsTemporary = FALSE;
}
