/*-------------------------------------------------------------------------*
 * $Workfile	: DuSlic.H  $
 * Part of 	    : ETL600 / O4CV
 * Language     : C
 * Created by   : Vladimir Pogorilyy
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/DuSlic.h $
 * 
 * 10    08-02-15 10:51 Chrobif
 * + DuSlic_DeactivateFxsIf() added.
 * 
 * 9     10.12.07 13:28 Chhamaa
 * Comment added.
 * 
 * 8     10.12.07 11:59 Chhamaa
 * Functions void DuSlic_startLEC() and void DuSlic_stopLEC() removed.
 * 
 * 7     07-09-24 14:32 Chvopog
 * Added:	VOLTAGE_LEVEL		0x0D00 //it is aprx 12VDC
 * 
 * 6     07-09-13 14:55 Chrobif
 * DuSlic_isOffHookDelayed() added.
 * 
 * 5     07-09-04 10:06 Chvopog
 * - LEC Switch ON/OFF added
 * - Voltage measuremets changed
 * 
 * 4     07-09-03 16:32 Chvopog
 * VOLTAGE_LEVEL	 changed at 15VDC
 * 
 * 3     07-08-23 16:50 Chrobif
 * DuSlic_writeCOP() input parameter mode, impedanceInd and ringerFreq
 * introduced.
 * DuSlic_verifyCOP() input parameter selectChannel removed and memcmp()
 * used instead of for loop.
 * DuSlic_startRingTripMeasurement() input parameter selectChannel
 * intoduced.
 * Entire DuSlic configuration moved to DuSlic_config(). Furthermore
 * DuSlic configuration was made dependenc on configuration structure.
 * DuSlic gain set to 6dB. Return value randomValue introduced.
 * 
 * 2     07-08-08 13:53 Chalnot
 * Led functions deleted.
 * 
 * 1     07-06-04 11:46 Chvopog
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
#ifndef  __DuSlic_DEFINED
#define __DuSlic_DEFINED

/* INCLUDE FILES **************************************************************/
#include <cdefBF538.h>
#include <ccblkfn.h>
#include <sysreg.h>

#include "Basetype_O4CV.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
#define	VOLTAGE_LEVEL		0x0D00 //it is aprx 12VDC

#define	SLIC_POWERDOWN			0x01	// Set Power Down High Impedance mode of the Slic
#define	SLIC_ACTIVE				0x02
#define	SLIC_RING				0x03
#define	SLIC_INTERRUPT			0x04
#define	SLIC_READ_INTERRUPT		0x05
#define	SLIC_START_LEC			0x06
#define	SLIC_STOP_LEC			0x07
/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/
void DuSlic_SpiFIFO(void);
bool DuSlic_SpiWriteFIFO(unsigned short sendCommand,unsigned char selectChannel);

void DuSlic_readAllInt(void);
void DuSlic_readIntChannel(unsigned short selectChanne);

void DuSlic_writeALLRegistersSOP(unsigned char selectChannel);
bool DuSlic_verifyALLRegistersSOP(unsigned char selectChannel);

void DuSlic_writeSOP(unsigned short selectChannel, unsigned short registerAdress, unsigned short writeByte);
void DuSlic_readSOP(unsigned short selectChannel,unsigned short registerAdress);

void DuSlic_writeCOP(unsigned short selectChannel, unsigned char mode, unsigned char impedanceInd, unsigned char ringerFreq);
void DuSlic_readCOP(unsigned short selectChannel);
bool DuSlic_verifyCOP(void);

void DuSlic_writePOP(unsigned short selectChannel,unsigned short *writeByte);
void DuSlic_readPOP(unsigned short selectChannel);
bool DuSlic_verifyPOP(unsigned short selectChannel);

bool DuSlic_VoltageMeasurement(unsigned short selectChannel);
unsigned int DuSlic_getRandom_long(unsigned short selectChannel);
void DuSlic_startRingTripMeasurement(unsigned short selectChannel);

void DuSlic_readInt(unsigned short selectChannel);
void DuSlic_hardwarereset(void);
void DuSlic_softreset(unsigned short selectDuslic);
void DuSlic_softresetChannel(unsigned short selectChannel);
void DuSlic_resyncPCM(unsigned short selectDuslic);
void DuSlic_startEDSP(unsigned short selectChannel);
void DuSlic_modePowerdown(unsigned short selectChannel);
void DuSlic_modeActive(unsigned short selectChannel);
void DuSlic_modeActiveRing(unsigned short selectChannel);
void DuSlic_LinecardSwitchSelect(unsigned short selectChannel, unsigned short selectState);
void DuSlic_OffHook(unsigned short selectChannel, unsigned short selectState);
    
bool DuSlic_isOffHook(       unsigned short selectChannel);
bool DuSlic_isOffHookDelayed(unsigned short selectChannel);
bool DuSlic_isOverTemperature(unsigned short selectChannel);
bool DuSlic_isSyncFail(unsigned short selectChannel);	 
bool DuSlic_isEDSPFail(unsigned short selectChannel);	 
bool DuSlic_isRingTripThreshold(unsigned short selectChannel);

UINT32 DuSlic_config(void);
void DuSlic_DeactivateFxsIf(void);
#endif 

