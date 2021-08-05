/*-------------------------------------------------------------------------*
 * $Workfile: EPROMHI.H $
 * Part of	: ETL600
 * Language	: C
 * Created by	: Christian Leeb
 * Remarks	:     
 * Purpose	:     Eprom High level routines
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/H/EPROMHI.H $
 * 
 * 4     16.07.05 18:45 Chmibuh
 * eprom_get_firmware_startAddress_Ptr() added
 * 
 * 3     25.05.05 17:12 Chchlee
 * Configuration check added
 * 
 * 2     16.02.05 10:27 Chchlee
 * Upgrade to Ptty2
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 5     27.09.04 7:53 Leeb02
 * 
 * 4     25.07.04 14:29 Leeb02
 * 
 * 3     25.07.04 11:13 Leeb02
 * 
 * 2     23.07.04 7:27 Leeb02
 * 
 * 1     21.07.04 12:19 Leeb02
 *-------------------------------------------------------------------------*/
 
#ifndef _EPROMHI
#define _EPROMHI
 
/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define NUMBER_OF_CONFIG_STRUCTS	4
#define PASSWORD_SIZE_IN_LONGS      4

/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef struct  
{    
	void         * address;
	unsigned long  sizeInLongs;	    
}
t_ConfigStructsList[NUMBER_OF_CONFIG_STRUCTS]; 
 

typedef struct  
{
	long  	Password[PASSWORD_SIZE_IN_LONGS];
}
t_EpromLocalConfiguration; 

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void eprom_init(t_ConfigStructsList * p_CfgList, 
				BOOL                * isTemporaryConfiguration,
                void               (*getDefaultCfg)(void));

void eprom_storeTemporaryConfiguration(void);
void eprom_markTemporaryConfigurationAsPermanent(void);
void eprom_storePermanentConfiguration(void);
void eprom_saveDefaultConfiguration(void (*getDefaultCfg)(void));
unsigned long * eprom_get_firmware_startAddress_Ptr(void);


#endif // _EPROMHI  





