/*-------------------------------------------------------------------------*
 * $Workfile	: binInOut.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Roger Biffiger (RBi)
 * Remarks      :  
 * Purpose      : The module binInOut handles the binary input/output signals 
 * 				  connected to general purpose pins of the DSP 
 * 				  (LEDs, M-wire, E-wire, local/transit, on/off hook, buzzer, ...). 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/binInOut.h $
 * 
 * 15    08-01-29 18:36 Chrobif
 * + binInOut init source added
 * 
 * 14    07-12-05 8:02 Chrobif
 * + binInOut_clearRingDetector() added.
 * 
 * 13    07-11-28 10:47 Chrobif
 * + binInOut_isMWireRinging() and binInOut_clearRingingMWireDetector()
 * added.
 * 
 * 12    07-11-27 16:22 Chrobif
 * + Ringing E-wire detection added
 * + bin_InOut_toggleTestOutPin() made "public"
 * + bin_InOut_toggleOutPin() added
 * + Last GPIO configured as output in series HW case
 * 
 * 11    07-09-13 14:59 Chrobif
 * + M-wire sampling commands introduced.
 * + binInOut_insertMWireFrame() added.
 * 
 * 10    12.09.07 14:48 Chhamaa
 * Output pin e_Tdm_Input_En added.
 * 
 * 9     07-09-07 15:03 Chalnot
 * Buzzer one freq functions added.
 * 
 * 8     07-08-22 15:04 Chalnot
 * Led defines added.
 * 
 * 7     21.08.07 17:59 Chhamaa
 * New function prototypes added: binInOut_getEWireDelayed(),
 * binInOut_getMWireDelayed()
 * 
 * 6     17.08.07 17:30 Chhamaa
 * Global variable declarations added.
 * 
 * 5     16.08.07 19:13 Chhamaa
 * Ring current detection functions added.
 * 
 * 4     07-08-13 9:53 Chalnot
 * Buzzer functions added.
 * 
 * 3     8.08.07 14:40 Chhamaa
 * Function prototypes modified.
 * 
 * 2     07-07-20 16:34 Chrobif
 * + binInOut_enableMWireSampling() replaced by
 * binInOut_enableMWireForwarding()
 * + binInOut_disableMWireSampling() replaced by
 * binInOut_disableMWireForwarding()
 * 
 * 1     07-07-13 16:56 Chrobif
 * Initial version
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef BIN_IN_OUT_H
#define BIN_IN_OUT_H

/* INCLUDE FILES **************************************************************/

#include "Basetype_O4CV.h"
#include "configuration.h"
#include "subframe.h"


/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define LED_ON		0
#define LED_OFF		1

// Buzzer frequencies
#define BUZZER_FREQ_5		2 	// f=2666Hz
#define BUZZER_FREQ_4		3	// f=1777Hz 	
#define BUZZER_FREQ_3		4 	// f=1333Hz
#define BUZZER_FREQ_2		5	// f=1066Hz
#define BUZZER_FREQ_1		6	// f=888Hz	

// For testing
#define TOGGLE_OUT_PIN_INSTANCE_1	0
#define TOGGLE_OUT_PIN_INSTANCE_2	1
#define TOGGLE_OUT_PIN_INSTANCE_3	2
#define TOGGLE_OUT_PIN_INSTANCE_4	3

/* PUBLIC TYPE DECLARATIONS ***************************************************/

typedef enum {

	e_Pulse_Code_Dialing1,
	e_Pulse_Code_Dialing2,
	e_Pulse_Code_Dialing3,
	e_Pulse_Code_Dialing4,
	e_Ring_Detect1,
	e_Ring_Detect2,
	e_Ring_Detect3,
	e_Ring_Detect4,
	e_MW1,
	e_MW2,
	e_MW3,
	e_MW4,
#ifdef PROTOTYP_HW_NO_PULSE_CODE_DIALING_EXTENSION
	e_Test_In
#endif
	
} e_InPinId;

typedef enum {

	e_Buf_EW1,
	e_Buf_EW2,
	e_Buf_EW3,
	e_Buf_EW4,
	e_Hook_Switch1,
	e_Hook_Switch2,
	e_Hook_Switch3,
	e_Hook_Switch4,
	e_Buzzer_P,
	e_Buzzer_N,
	e_Alarm_Led,
	e_Warning_Led,
	e_Ready_Led,
	e_Sp_Enabled_Led,
	e_Sp_Free_Led,
	e_Tdm_Input_En,
	e_Test_Out

} e_OutPinId;

typedef enum {
    
    e_HwStartUp,
    e_SwRestart,
    e_TdmBusRestart,
    e_SlaveSync
    
} e_BinInOutInitSource;

/* GLOBAL VARIABLE DECLARATIONS ***********************************************/

extern t_mWireData *g_pEWireBuffer;
extern t_mWireData *g_pMWireBuffer;


/* PUBLIC MACRO DEFINITIONS ***************************************************/
/* PUBLIC FUNCTIONS ***********************************************************/

extern void binInOut_initTxEWireAndRxMWire(t_O4CVConfig *pConfig, e_BinInOutInitSource initSource);
extern void binInOut_startRxMWire(void);
extern void binInOut_startTxEWire(void);
extern void binInOut_startTxEWireAndRxMWire(void);

extern void binInOut_receiveMWire(void);
extern void binInOut_sendEWire(void);

extern UINT8 binInOut_getEWire(UINT16 chNr);
extern UINT8 binInOut_getEWireDelayed(UINT16 chNr);
extern UINT8 binInOut_isEWireRinging(UINT16 chNr);
extern void binInOut_setEWire(UINT16 chNr);
extern void binInOut_clearEWire(UINT16 chNr);
extern void binInOut_clearRingingEWireDetector(UINT16 chNr);

extern UINT8 binInOut_getMWire(UINT16 chNr);
extern UINT8 binInOut_getMWireDelayed(UINT16 chNr);
extern void binInOut_setMWire(UINT16 chNr);
extern void binInOut_clearMWire(UINT16 chNr);
extern void binInOut_insertMWireFrame(UINT16 chNr);
extern UINT8 binInOut_isMWireRinging(UINT16 chNr);
extern void binInOut_clearRingingMWireDetector(UINT16 chNr);

extern void binInOut_enableMWireForwarding(UINT16 chNr);
extern void binInOut_disableMWireForwarding(UINT16 chNr);

extern void binInOut_startMWireSampling(UINT16 chNr);
extern void binInOut_stopMWireSampling( UINT16 chNr);

extern void binInOut_initRingCurrentDetector(void);
extern void binInOut_detectRingCurrent(void);
extern bool binInOut_isRingCurrentOn(UINT16 chNr);
extern void binInOut_clearRingDetector(UINT16 chNr);

extern void binInOut_setOutput(UINT8 val, e_OutPinId pinId);
extern UINT8 binInOut_getInput(e_InPinId pinId);

extern void binInOut_buzzerRing(UINT16 numberOfRings);
extern void binInOut_buzzerOn(void);
extern void binInOut_buzzerOff(void);
extern void binInOut_sampleToBuzzer(void);
extern void binInOut_buzzerRingOneFreq(UINT16 freq, UINT16 duration);
extern void binInOut_sampleToBuzzerOneFreq(void);

extern void binInOut_toggleTestOutPin(void);
extern void binInOut_toggleOutPin(e_OutPinId pinId, UINT8 instance);

#endif
