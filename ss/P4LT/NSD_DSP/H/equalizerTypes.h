/*-------------------------------------------------------------------------*
 * $Workfile: equalizerTypes.h $
 * Part of      : ETL600 / APLC
 * Language     : C
 * Created by   : Author
 * Remarks      :  
 * Purpose      : Equalizer global type declarations
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Master_DSP/H/equalizerTypes.h $
 * 
 * 1     19.01.05 15:49 Chchlee
 * 
 * 11    19.01.05 15:04 Chhesch5
 * 
 * 10    18.01.05 18:55 Chhamaa
 * t_EqualizerStatus added
 * 
 * 9     17.01.05 18:52 Chhamaa
 * EQUALIZER_STATUS_EMPTY added
 * 
 * 8     13.01.05 17:03 Chhamaa
 * Defines for equalizer status upload added
 * 
 * 7     6.01.05 15:36 Chhamaa
 * PNControllerStatusType and ChannelMeasurementStatusType moved to
 * Equalizer.h
 * 
 * 6     13.04.04 17:13 Nohl01
 * 
 * 5     8.04.04 10:19 Nohl01
 * 
 * 4     7.04.04 11:15 Leeb02
 * 
 * 3     30.03.04 9:15 Nohl01
 * Terminology "bitmask" changed to "bitfield" within comments.
 * 
 * 2     29.03.04 17:02 Nohl01
 * None-bitmask variables of type UINT8 and UINT32 changed to INT32.
 * Bitmask variables commented.
 * 
 * 1     5.03.04 11:15 Maag01
 * Initial Version
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef	EQUALIZER_TYPES_H
#define	EQUALIZER_TYPES_H

/* INCLUDE FILES ***********************************************************/

#include "basetype.h"


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

// Bit maskes für EntzerrerEinstellungen                                  
                                  
#define	ENTZERRER_STANDARD				0L 
#define	ENTZERRER_KURZ					1L
#define	ENTZERRER_MITTEL				2L
#define	ENTZERRER_LANG					3L



/* Definition der 4 verschiedenen Equalizerlängen */
#define EQUALIZER_SHORT_LENGTH			25L
#define EQUALIZER_MIDDLE_LENGTH			43L
#define EQUALIZER_LONG_LENGTH			63L
#define EQUALIZER_STANDARD_LENGTH		32L
#define P4LB_DEEQUALIZER_LENGTH			25L

// Number of cycles until an equalizer calculation is executed

#define EQUALIZER_NUMBER_OF_CYCLES_TO_GRAP_CPU		140000

// Equalizer CRCs for no equalizer (first coefficient 1, all others 0)
#define CRC_STANDARD_EQUALIZER_CH1		63708L
#define CRC_STANDARD_EQUALIZER_CH2		63708L
#define CRC_STANDARD_EQUALIZER_CH3		63708L

/* Definition maximaler Filterlänge Equalizer */
#define EQUALIZER_MAX_LENGTH			64L    

/* Definition maximale Anzahl Mittelungen beim Messen des Kanals */
#define MAX_ANZAHL_MITTELUNGEN			292L

// Change also NOISELEVEL on same value in pn_gener.asm !!!
#define NOISELEVEL						0.1F
  
/* Anzahl Koeffizienten, die auf einmal in den Equalizerfilter transferiert werden */
#define NUMBER_OF_COEF_TRANSFER			1L

/* Laenge des Messarrays */

#define ENTZERRER_MESSUNG_ARR_LENGTH		256


/* Definition Uebertragungsbandbreite Kanal, Aktiver Bereich des Equalizers in Hz*/
#define LOW_FREQUENCY_CUTOFF	300.0F
#define HIGH_FREQUENCY_CUTOFF	4050.0F
#define PILOT_CHANNEL_BANDWIDTH	240.0F

/* Useful AF Bandwidth for derivation calculation in dB */
#define LOW_FREQUENCY_BOUNDARY	300.0F
#define HIGH_FREQUENCY_BOUNDARY	3900.0F 

/* Optimisation Max Energy in Step Response */

#define EQUALIZER_FINE_SHIFT			10L 

/* Defines Messwerte Status im EntzerrerStatusType */
/*
#define AKTUELL		1L
#define NEU			2L
*/


// Defines zur Steuerung, ob Equalizer mit oder ohne Laufzeitentzerrung gewünscht wird (Ab Rel. 3.1)
#define	EQUALIZER_WITH_GROUP_DELAY_EQUALISATION		0
#define EQUALIZER_WITHOUT_GROUP_DELAY_EQUALISATION	1

// Defines for equalizer status upload
#define EQUALIZER_STATUS_EMPTY		0		// status data not available
#define EQUALIZER_STATUS_ACTIVE		1		// status data of active equalizer
#define	EQUALIZER_STATUS_NEW		2		// status data of latest channel measurement


/* GLOBAL TYPE DECLARATIONS ************************************************/

/*
// Datenstruktur für Konfigurationsdaten Entzerrer 
// 
typedef struct {
	// channel 1
	long	EntzerrerMaxAnzahlMittelungen;		
  	long	EntzerrerEinstellungen;
  	long	Req_EntzerrerEinstellungen;
	long 	CRC_Equalizer[4];
	long	OnlyAmplitudeResponseActiveCh1;	// Ab Release 3.1 verfügbar, alle vorhergehenden MMI Releases auf 0
	// channel 2
	long	EntzerrerMaxAnzahlMittelungen_CH2;		
  	long	EntzerrerEinstellungen_CH2;
  	long	Req_EntzerrerEinstellungen_CH2;
	long 	CRC_Equalizer_CH2[4];
	long	OnlyAmplitudeResponseActiveCh2;	// Ab Release 3.1 verfügbar, alle vorhergehenden MMI Releases auf 0
} EntzerrerKonfigType; */


// Equalizer configuration data structure

typedef struct {
	
	long			maxNumberOfIterationsAveraging;		
  	unsigned long	equalizerSettings;					// bitfield
  	unsigned long	requestedEqualizerSettings;			// bitfield
	long			CRC_Equalizer[4];
	long			onlyAmplitudeEqualizationActive;	// Is only available from Release 3.1,  
												        // is set to 0 in all previous Releases
} t_EqualizerConfig;


// Equalizer status data structure

/* replaced by t_EqualizerStatus
typedef struct {

	long	MesswerteStatus;
	float	EntzerrerMessungArr[256];
	long 	crc_checksum_old;		// from release 3.0 on no longer nessessary
	long 	dummy1;
	long 	dummy2;
	float	EntzerrerKoeffArr[256];
	long	EntzerrerSchwerpunktPos;
	long	CRC_CheckFIR;
	
} EntzerrerStatusType;
*/

typedef struct {

	long	flag;
	float	pnAccu[256];
	long 	crc_checksum_old;		// from release 3.0 on no longer nessessary
	long 	dummy1;
	long 	dummy2;
	float	coef[256];
	long	balancePoint;
	long	crc_checkFIR;
	
} t_EqualizerStatus;


typedef	struct {

	long 	Laufzeit[4];		//Laufzeit in 1/10 ms
	long	LMSError[4];		//LMS Error in Promille
	long	Abweichung[4];		//Mittlere Abweichung in 1/10 dB
	long 	dummy1;
	long 	dummy2;
	long 	dummy3;
	long 	dummy4;

} EntzerrerQualityType;


#endif
