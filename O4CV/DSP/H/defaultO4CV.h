/*-------------------------------------------------------------------------*
 * $Workfile	: defaultO4CV.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The module defaultO4CV provides a partial or complete
 *				  O4CV configuration.
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/defaultO4CV.h $
 * 
 * 5     23.07.07 15:09 Chhamaa
 * Function defaultO4CV_getCpTones() removed.
 * 
 * 4     07-07-06 13:58 Chmaflu
 * Add to existing C functions the same but as extern "C" declarations 
 * 
 * 3     23.04.07 8:59 Chrobif
 * Body implemented.
 * 
 * 2     5.04.07 11:28 Chrobif
 * Bug fix in VSS log area.
 *
 * 1     5.04.07 11:20 Chrobif
 * Initial version.
 *
 *-------------------------------------------------------------------------*/
 
#ifndef DEFAULT_O4CV_H
#define DEFAULT_O4CV_H

/* INCLUDE FILES **************************************************************/

// O4CV firmware
#include "O4CV_Types.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/




/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/


#ifdef __cplusplus
	extern "C" void defaultO4CV_getAfInterfaceCfg2wire(t_AfInterfaceCfg *pAfInterfaceCfg);
	extern "C" void defaultO4CV_getAfInterfaceCfg4wire(t_AfInterfaceCfg *pAfInterfaceCfg);
	extern "C" void defaultO4CV_getSevicePhone(t_ServicePhoneCfg *pServicePhoneCfg);
	extern "C" void defaultO4CV_getChannel(t_ChannelCfg *pChannelCfg);
	extern "C" void defaultO4CV_getConfig(t_O4CVConfig *pConfig);
#else
	void defaultO4CV_getAfInterfaceCfg2wire(t_AfInterfaceCfg *pAfInterfaceCfg);
	void defaultO4CV_getAfInterfaceCfg4wire(t_AfInterfaceCfg *pAfInterfaceCfg);
	void defaultO4CV_getSevicePhone(t_ServicePhoneCfg *pServicePhoneCfg);
	void defaultO4CV_getChannel(t_ChannelCfg *pChannelCfg);
	void defaultO4CV_getConfig(t_O4CVConfig *pConfig);
#endif 

#endif 
