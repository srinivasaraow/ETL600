/*-------------------------------------------------------------------------*
 * $Workfile: FE_DSP_Alarm.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : H.-J. Maag
 * Remarks      :  
 * Purpose      : Generate FE DSP Alarms
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT/FE_DSP/C/FE_DSP_Alarm.c $ 
 * 
 * 3     11.07.05 19:09 Chmibuh
 * FE_DSP_Alarm_rxRfOverflow() added
 * 
 * 2     18.02.05 15:01 Chhamaa
 * Ignore alarms if equipment is not in normal operation mode
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 3     12.01.05 18:39 Chhamaa
 * Alarm update is performed only for active APLC channels
 * 
 * 2     14.12.04 11:43 Chhamaa
 * 
 * 1     29.11.04 11:49 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/

#include "basetype.h"
#include "DIAG.H"
#include "P4LT_Types.h"
#include "FE_DSP_Alarm.h"


/* LOCAL CONSTANT DECLARATIONS ***********************************************/



/* LOCAL VARIABLES ***********************************************************/



/* IMPLEMENTATION ************************************************************/


/*****************************************************************************
  Function:		FE_DSP_Alarm_updateAplcAlarms()		
  Description:	Updates the APLC alarm bits.
  Inputs:		Pointer to FE DSP alarm bits: feAlarm 
				Pointer to FE DSP status: feStatus
  Outputs:		FE DSP alarm bits: feAlarm
  Return:		None
  Globals:		None
  Important:	-

*******************************************************************************/

void SEGMENT_EXT_CODE FE_DSP_Alarm_updateAplcAlarms(t_FE_Alarm *feAlarm, 
													t_FrontEndDSP_Status *feStatus,
													unsigned long opMode, long nAplcCh)
{
	
	if (opMode == OP_MODE_NORMAL) {

		switch(nAplcCh) {

			case 3:
				
				// update rx pilot fail alarm
				if (feStatus->APLC.pilot[APLC_CH3].general & RX_PILOT_OK_SET) {
					
					feAlarm->rxPilotFailCh3 = FALSE;
				}
				else {

					feAlarm->rxPilotFailCh3 = TRUE;
				};
				
				// update rx pilot SNR alarm
				if (feStatus->APLC.pilot[APLC_CH3].general & SNR_ALARM_SET) {

					feAlarm->pilotSNRAlarmCh3 = TRUE;
				}
				else {
					
					feAlarm->pilotSNRAlarmCh3 = FALSE;
				};

				// update rx pilot level alarm
				if (feStatus->APLC.pilot[APLC_CH3].general & PILOT_LEVEL_ALARM_SET) {
					
					feAlarm->pilotLevelAlarmCh3 = TRUE;
				}
				else {
					
					feAlarm->pilotLevelAlarmCh3 = FALSE;
				};

				// update AGC fail alarm 
				if (feStatus->APLC.pilot[APLC_CH3].general & AGC_FAIL_SET) {
					
					feAlarm->agcFailCh3 = TRUE;
				}
				else {
					
					feAlarm->agcFailCh3 = FALSE;
				};

			case 2:

				// update rx pilot fail alarm
				if (feStatus->APLC.pilot[APLC_CH2].general & RX_PILOT_OK_SET) {
					
					feAlarm->rxPilotFailCh2 = FALSE;
				}
				else {
					
					feAlarm->rxPilotFailCh2 = TRUE;
				};

				// update rx pilot SNR alarm
				if (feStatus->APLC.pilot[APLC_CH2].general & SNR_ALARM_SET) {
					
					feAlarm->pilotSNRAlarmCh2 = TRUE;
				}
				else {
					
					feAlarm->pilotSNRAlarmCh2 = FALSE;
				};

				// update rx pilot level alarm
				if (feStatus->APLC.pilot[APLC_CH2].general & PILOT_LEVEL_ALARM_SET) {
					
					feAlarm->pilotLevelAlarmCh2 = TRUE;
				}
				else {
					
					feAlarm->pilotLevelAlarmCh2 = FALSE;
				};

				// update AGC fail alarm 
				if (feStatus->APLC.pilot[APLC_CH2].general & AGC_FAIL_SET) {
					
					feAlarm->agcFailCh2 = TRUE;
				}
				else {

					feAlarm->agcFailCh2 = FALSE;
				};

			case 1:

				// update rx pilot fail alarm
				if (feStatus->APLC.pilot[APLC_CH1].general & RX_PILOT_OK_SET) {
					
					feAlarm->rxPilotFailCh1 = FALSE;
				}
				else {
					
					feAlarm->rxPilotFailCh1 = TRUE;
				};

				// update rx pilot SNR alarm
				if (feStatus->APLC.pilot[APLC_CH1].general & SNR_ALARM_SET) {
					
					feAlarm->pilotSNRAlarmCh1 = TRUE;
				}
				else {
					
					feAlarm->pilotSNRAlarmCh1 = FALSE;
				};

				// update rx pilot level alarm
				if (feStatus->APLC.pilot[APLC_CH1].general & PILOT_LEVEL_ALARM_SET) {
					
					feAlarm->pilotLevelAlarmCh1 = TRUE;
				}
				else {
					
					feAlarm->pilotLevelAlarmCh1 = FALSE;
				};

				// update AGC fail alarm 
				if (feStatus->APLC.pilot[APLC_CH1].general & AGC_FAIL_SET) {
					
					feAlarm->agcFailCh1 = TRUE;
				}
				else {
					
					feAlarm->agcFailCh1 = FALSE;
				};

				// update AFC fail alarm
				if (feStatus->APLC.statusFlags & AFC_FAIL_SET) {
					
					feAlarm->afcFail = TRUE;
				}
				else {
					
					feAlarm->afcFail = FALSE;
				};

			case 0:

				// do nothing
				break;

			default: ASSERT(FALSE);
		};
	}
	else { // ignore APLC alarms

		feAlarm->rxPilotFailCh1		= FALSE;  
		feAlarm->rxPilotFailCh2		= FALSE;  
		feAlarm->rxPilotFailCh3		= FALSE; 
		
		feAlarm->pilotSNRAlarmCh1	= FALSE;  
		feAlarm->pilotSNRAlarmCh2	= FALSE;  
		feAlarm->pilotSNRAlarmCh3	= FALSE;  

		feAlarm->pilotLevelAlarmCh1	= FALSE;  
		feAlarm->pilotLevelAlarmCh2	= FALSE; 
		feAlarm->pilotLevelAlarmCh3	= FALSE;  

		feAlarm->agcFailCh1			= FALSE;  
		feAlarm->agcFailCh2			= FALSE;  
		feAlarm->agcFailCh3			= FALSE;  

		feAlarm->afcFail			= FALSE;

	};
}

BOOL SEGMENT_EXT_CODE FE_DSP_Alarm_rxRfOverflow(void)
{
	// Flag 10 indicates overload of ADC - it's the signal OTR prolongated by FPGA
	if(((*(long*)IOFLAG) & (FLG10))) // special return for bitfields
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}

}
