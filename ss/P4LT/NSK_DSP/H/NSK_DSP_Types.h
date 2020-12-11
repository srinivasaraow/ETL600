/*-------------------------------------------------------------------------*
 * $Workfile: NSK_DSP_Types.h $
 * Part of      : ETL600 / NSK DSP
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/P4LT/Master_DSP/H/NSK_DSP_Types.h $
 * 
 * 11    8.06.10 18:54 Chhamaa
 * t_NSK_DSP_Status modified.
 * 
 * 8     23.07.07 16:52 Chtognu
 * NSK adaptive Equalizer types added: t_ModemEqualizerConfig,
 * t_NSK_EqualizerStatus, t_NSK_EqualizerCfg
 * 
 * 7     31.01.07 10:05 Chhamaa
 * O4CV status modified.
 * 
 * 2     10.02.05 9:44 Chalgyg
 * t_Mux_Status inserted.
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 6     5.01.05 13:30 Chchlee
 * OFDM DSP Cfg added to NSK DSP cfg
 * 
 * 5     17.12.04 13:14 Chhamaa
 * t_NSK_DSP_Status added
 * 
 * 4     29.11.04 15:42 Gnus01
 * NSK 600 status moved to NSN_Types.h
 * 
 * 3     25.11.04 13:36 Gnus01
 * 
 * 2     16.09.04 11:04 Gnus01
 * 
 * 1     5.03.04 15:34 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/
 
#ifndef NSK_DSP_TYPES_H
#define NSK_DSP_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include	"basetype.h"
#include	"NSK_Types.h"
#include	"ofdm_types.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/


/* GLOBAL TYPE DECLARATIONS ************************************************/


typedef struct {

	t_ModemConfig			modem[N_NSK_MODEMS];
	t_ModemEqualizerConfig	equalizer[N_NSK_MODEMS];

	t_Mux_Cfg				muxCfg;

	t_OFDM_Config			ofdmCfg;

} t_NSK_DSP_Config;


typedef struct {

	t_NSK_Status				NSK;
	t_NSK_EqualizerStatus		equalizer;

	t_MUX_Status				MUX;
	t_MUX_O4CV_Status			MUX_O4CV;
	t_MUX_VoiceTransitStatus	MUX_VoiceTransit;

} t_NSK_DSP_Status;


#endif
