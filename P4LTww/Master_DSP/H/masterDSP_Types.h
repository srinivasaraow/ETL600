/*-------------------------------------------------------------------------*
 * $Workfile: masterDSP_Types.h $
 * Part of      : ETL600 / Master DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/masterDSP_Types.h $
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 2     7.04.04 11:04 Leeb02
 * 
 * 1     5.03.04 15:33 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef MASTER_DSP_TYPES_H
#define MASTER_DSP_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include	"basetype.h"
#include	"P4LT_Types.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/


/* GLOBAL TYPE DECLARATIONS ************************************************/


typedef struct {

	t_EquipmentData			equipmentInfo;
	t_DevSystemTestSettings	systemTest;
	t_ServicesData			serviceData;
	t_TDM_BusConfig			TDM_Config[10];
	unsigned long			alarmConfiguration[N_APLC_CHANNELS];
	unsigned long			configLinkAlarmRelay;
	unsigned long			configHWAlarmRelay;
	unsigned long			configRes1Relay;
	unsigned long			configRes2Relay;
	unsigned long			configBridge;
	unsigned long			remainingActiveTestConfigTime;

} t_MasterDSP_Config;



#endif
