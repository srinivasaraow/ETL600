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
 * $Log: /Source/P4LT/Master_DSP/H/equalizerTypes.h $
 * 
 * 18    12.10.05 11:43 Chchlee
 * commented out lines removed
 * 
 * 17    9.03.05 11:12 Chhesch5
 * 
 * 16    2.02.05 15:55 Chhamaa
 * Defines ENTZERRER_MESSUNG_ARR_LENGTH and MAX_ANZAHL_MITTELUNGEN
 * reactivated for prototype 2 FW
 * 
 * 15    2.02.05 11:25 Chhesch5
 * 
 * 14    1.02.05 18:19 Chhamaa
 * Defines cleaned up
 * 
 * 13    28.01.05 15:50 Chhesch5
 * 
 * 12    25.01.05 18:03 Chhesch5
 * EntzerrQualityType renamed to  t_EqualizerQuality 
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


/* GLOBAL CONSTANT DECLARATIONS ********************************************/

// Number of user selectable equalizer lengths (none, short, medium, long)
// used in equalizerSettings
#define	NUM_OF_EQUALIZER_LENGTH			4

#define EQUALIZER_SETTINGS_NONE			0L
#define EQUALIZER_SETTINGS_SHORT		1L
#define EQUALIZER_SETTINGS_MEDIUM		2L
#define EQUALIZER_SETTINGS_LONG			3L

// Number of equalizer fir filter taps
#define EQUALIZER_SHORT_LENGTH			25L
#define EQUALIZER_MIDDLE_LENGTH			43L
#define EQUALIZER_LONG_LENGTH			63L
#define EQUALIZER_STANDARD_LENGTH		32L

// Maximum number of equlizer fir filter taps
#define EQUALIZER_MAX_LENGTH			64L    

// Maximum number of pseudo noise sequence accumulations used for channel measurement
#define MAX_ANZAHL_MITTELUNGEN			292L
#define MAX_NUM_OF_ACCUMULATIONS		292L

// Change also NOISELEVEL on same value in pn_gener.asm !!!
#define NOISELEVEL						0.1F

// PN accumulator length
#define ENTZERRER_MESSUNG_ARR_LENGTH	256
#define	PN_ACCUMULATOR_LENGTH			256

// Equalizer transfer function length
#define EQUALIZER_TF_LENGTH				129

// Equalizer frequency range [Hz]
#define LOW_FREQUENCY_CUTOFF			300.0F
#define HIGH_FREQUENCY_CUTOFF			4050.0F
#define PILOT_CHANNEL_BANDWIDTH			240.0F

// Useful AF bandwidth for derivation calculation in dB 
#define LOW_FREQUENCY_BOUNDARY			300.0F
#define HIGH_FREQUENCY_BOUNDARY			3900.0F 

// Optimisation max energy in step response
#define EQUALIZER_FINE_SHIFT			10L 

// Equalizer properties
#define	EQUALIZER_WITH_GROUP_DELAY_EQUALISATION		0
#define EQUALIZER_WITHOUT_GROUP_DELAY_EQUALISATION	1

// Defines for equalizer status
#define EQUALIZER_STATUS_EMPTY			0		// status data not available
#define EQUALIZER_STATUS_ACTIVE			1		// status data of active equalizer
#define	EQUALIZER_STATUS_NEW			2		// status data of latest channel measurement


/* GLOBAL TYPE DECLARATIONS ************************************************/

// Equalizer configuration data structure

typedef struct {

  	unsigned long	equalizerSettings;					// bitfield
	long			onlyAmplitudeEqualizationActive;	// Is only available from Release 3.1,  
												        // is set to 0 in all previous Releases
} t_EqualizerConfig;



// Equalizer status data structure

typedef struct {

	long	flag;
	float	pnAccu[PN_ACCUMULATOR_LENGTH];

} t_EqualizerStatus;



#endif
