/*-------------------------------------------------------------------------*
 * $Workfile: AES600.h $
 * Part of      : HMI600
 * Language     : C++
 * Created by   : Timur Vagapov
 * Remarks      :  
 * Purpose      : Header file for AES600 constants and definitions
 * Copyright    : ABB Switzerland Ltd., 2003
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source R4/HMI600/H/AES600.h $
 * 
 * 23    11-03-14 16:04 Chmazar
 * 
 * 23    27.08.10 13:40 Chhamaa
 * Define for AES600_EXTENDED_MODE added.
 * 
 * 21    17.11.08 13:47 Chsanoh
 * - Default boost ratio AES new 6 dB instead of auto-select max.
 * - APLC 2 kHz und DPLC 2 kHz (ModAboveSpeech). With AES in APLC, pilot =
 * 2220 Hz; with NSD in APLC or no TPE in APLC, pilot = 2160 Hz.
 * - Signaling rate (normal /slow)
 * - Config view adaptations
 * - Cosmetics
 * 
 * 19    12.09.08 17:11 Chsanoh
 * AES state mode for line protection command A introduced.
 * 
 * 17    22.12.06 14:11 Chsanoh
 * Mezzanine check box for AES modes 3 and 4 added in developer mode
 * (<Configuration> <AES600...> <Modes>).
 * 
 * 15    20.09.06 12:00 Chhamaa
 * 
 * 14    14.09.06 15:15 Chsanoh
 * TANG window and Rx guard signaling implemented in corresponding masks.
 * 
 * 13    29.08.06 11:37 Chhamaa
 * Defines for AES command frequencies added.
 * 
 * 12    18.08.06 15:13 Chsanoh
 * Cosmetical adaptations to the following bitmasks:
 * 
 * - AES600_TXINPDELAY_MASK	
 * - AES600_TXINPPROLONG_MASK	
 * - AES600_TXMAXTRIP_MASK	
 * - AES600_RXOUTDELAY_MASK
 * - AES600_RXOUTDURATION_MASK	
 * - AES600_RXOUTPROLONG_MASK	
 * - AES600_RXMAXTRIP_MASK
 * 
 * 11    18.08.06 14:38 Chhamaa
 * 
 * 10    15.08.06 9:26 Chhamaa
 * 
 * 9     7.08.06 12:49 Chchlee
 * Save from "old Source AES" branch
 * 
 * 10    21.07.06 14:20 Chhamaa
 * 
 * 9     7.07.06 18:50 Chhamaa
 * 
 * 5     30.06.05 16:20 Chsanoh
 * 
 * 4     8.03.05 15:52 Chpttra1
 * New AES arrays were implemented
 * 
 * 3     7.03.05 17:46 Chpttra1
 * AES structure was optimized
 * 
 * 2     4.03.05 10:05 Chpttra1
 * AES definitions from NSD550.h were transferd
 * 
 * 1     4.03.05 8:08 Chpttra1
 *-------------------------------------------------------------------------*/

#include "basetype.h"
#include "constant.h"

// 
// defines for AES-commands descending priority Tx frame of tdm-bus extended time slot
//
#define  AES_1				0x80000000
#define  AES_2				0x40000000
#define  AES_3				0x20000000
#define  AES_4				0x10000000
#define  AES_5				0x08000000
#define  AES_6				0x04000000
#define  AES_7				0x02000000
#define  AES_8				0x01000000
#define  AES_9				0x00800000
#define  AES_10				0x00400000
#define  AES_11				0x00200000
#define  AES_12				0x00100000
#define  AES_13				0x00080000
#define  AES_14				0x00040000
#define  AES_15				0x00020000
#define  AES_16				0x00010000
#define  AES_17				0x00008000
#define  AES_18				0x00004000
#define  AES_19				0x00002000
#define  AES_20				0x00001000
#define  AES_21				0x00000800
#define  AES_22				0x00000400
#define  AES_23				0x00000200
#define  AES_24				0x00000100

//
// Other TDM-Bus Bits RX frame Extended time slot for AES550 Rx
//
// #define  UNBLOCKING		0x00000080
// #define  NO_RX_GUARD_TDM	0x00000100
#define  TX_CMD_ACK			0x00000200
#define  RX_CMD_ACK			0x00000400
#define  RX_AES_1			0x00000800
#define  RX_AES_2			0x00001000
#define  RX_AES_3			0x00001800
#define  RX_AES_4			0x00002000
#define  RX_AES_5			0x00002800
#define  RX_AES_6			0x00003000
#define  RX_AES_7			0x00003800
#define  RX_AES_8			0x00004000
#define  RX_AES_9			0x00004800
#define  RX_AES_10			0x00005000
#define  RX_AES_11			0x00005800
#define  RX_AES_12			0x00006000
#define  RX_AES_13			0x00006800
#define  RX_AES_14			0x00007000
#define  RX_AES_15			0x00007800
#define  RX_AES_16			0x00008000
#define  RX_AES_17			0x00008800
#define  RX_AES_18			0x00009000
#define  RX_AES_19			0x00009800
#define  RX_AES_20			0x0000A000
#define  RX_AES_21			0x0000A800
#define  RX_AES_22			0x0000B000
#define  RX_AES_23			0x0000B800
#define  RX_AES_24			0x0000C000

// AES command mask defines
#define AES_12_TX_CMD_MASK			0xFFF00000
#define AES_20_TX_CMD_MASK			0xFFFFF000

#define AES_RX_CMD_MASK				0x0000F800

// Number of line protection and AES commands
#define NO_OF_LP_COMMANDS			4
#define NO_OF_AES_COMMANDS			20

//
// defines for AES600 bit values
//
#define	AES600_AES_MODE				BIT0  // (0 - 12 cmd, 1 - 20 cmd, default:0)
#define	AES600_T0					BIT1  // (0 - T01, 1 - T02, default:0)
#define	AES600_T0_AES				BIT2  // (0 - T01, 1 - T02, default:0, Rel.2)
#define	AES600_LP_MODE				BIT3  // (0 - Mode 3,4, 1 - Mode 1,2, default:0)
#define	AES600_SEC_DEP_CMD			BIT4  // (0 - icreased dependability, 1 - increased security, default:1)
#define	AES600_SEC_DEP_CMD_AES		BIT5  // (0 - icreased dependability, 1 - increased security, default:1, Rel. 2)
#define	AES600_RESET_RX_ALARM		BIT6  // (0 - automatically, 1 - manually, default:0)
#define	AES600_NSD_ETL_GUARD		BIT7  // (0 - ETL pilot, 1 - own guard, default:0)
#define	AES600_LOOPTEST_ON_OFF		BIT8  // (0 - off, 1 - on, default:0)
#define	AES600_PERSIST_ON_OFF		BIT9  // (0 - off, 1 - on, default:0)
#define	AES600_RXOUTDURPROL_A		BIT10 // (0 - Rx Output Prolongation, 1 - Rx Output Duration, default:0)
#define	AES600_RXOUTDURPROL_B		BIT11 // (0 - Rx Output Prolongation, 1 - Rx Output Duration, default:1)
#define AES600_MODE_BOOST_RATIO		BIT12 // (0 = user select, 1 = auto select)
#define AES600_TANG_ON_OFF			BIT13 // (0 = TANG window off, 1 = TANG window on)
#define AES600_DEVELOPER_MEZZANINE	BIT14 // (0 = Mezzanine bit will not be set on G4AI, 1 = Mezzanine bit will be set on G4AI)
											  // Only relevant in developer mode for AES-modes 3 and 4!
#define AES600_CMDA_STATE_MODE		BIT15 // (0 = state mode for command A of group A off, 1 = state mode for command A of group A on)
#define AES600_SIGNALING_MODE		BIT16 // (0 = normal (3 cycles), 1 = fast (2 cycles))
#define AES600_EXTENDED_MODE		BIT17 // (0 = normal mode (stepping), 1 = extended mode (state) with 4 independent line cmd

#define AES600_INIT					(AES600_SEC_DEP_CMD | AES600_RXOUTDURPROL_B | \
									 AES600_DEVELOPER_MEZZANINE)

#define	AES600_TXINPDELAY_MASK		0x0000000F	// 4  bit for TxInpComDelay[] (range 0...10)			 (<< 0)
#define	AES600_TXINPPROLONG_MASK	0x0003FFF0	// 14 bit for TxInpComProlongationAES[] (range 0...3000) (<< 4)
#define	AES600_TXMAXTRIP_MASK		0x00FC0000	// 6  bit for TxInpComMonit_DurationAES[] (range 1...60) (<<18)
#define	AES600_RXOUTDELAY_MASK		0x0F000000	// 4  bit for RxInpComDelay[] (range 0...10)			 (<<24)


#define	AES600_RXOUTDURATION_MASK	0x00000FFF	// 12 bit for RxOutComDurationAES[] (range 0...1000)	 (<< 0)
#define	AES600_RXOUTPROLONG_MASK	0x03FFF000	// 14 bit for RxOutComProlongationAES[] (range 0...3000) (<<12)
#define	AES600_RXMAXTRIP_MASK		0xFC000000	// 6  bit for RxOutComMonit_DurationAES[] (range 1...60) (<<26)


// Number of AES Tones
#define NO_OF_TONES_AES		9

// integr. length for continouse command evaluation 
#define	INT_LENGHT_CC		32

// guard frequency +/- shift frequency (* 5.0 instead of *10  -> frequency shift is twice) 
#define	GUARD_FREQ_SHIFT	((AF_SAMPLE_FREQ)/(INT_LENGHT_CC * 10.0))

//# OF INTEGRATIONS FOR THE FREQUENCY DETECTORS 
#define N1	25
#define N2	(2*N1)
#define N3	(4*N1)   
 
// CLIPPING CONSTANT FOR THE INPUT SIGNAL 
#define SCON	0.90            
                          
// UNBLOCKING THRESHOLD 
// Ref: Command A->  Power=0.7 == +1.2dB			 gemäss Sample AF Rx Signal CH1 mit MMI
// Ref: Signal 300Hz: Power (T03)= 0.5657 == +0.3dB gemäss Sample AF Rx Signal CH1 mit MMI
// Ref: Signal 300Hz: Power (T03)= 0.4542 == -0.6dB gemäss Sample AF Rx Signal CH1 mit MMI
// UN_THRES_HI = -10dB
#define UN_THERS_HI			0.0528

// UN_THRES_LOW = -14dB
#define UN_THERS_LOW		0.0210 

// Alpha values for AES550
#define ALPHA_N1_LOW		0.81982
#define ALPHA_N2_LOW		0.69971
#define ALPHA_N3_LOW		0.52002

#define ALPHA_N1_HIGH		0.89990
#define ALPHA_N2_HIGH		0.81982
#define ALPHA_N3_HIGH		0.69971 

// Window lenght for coded and nocoded
#define CODE_SEQUENZ_LENGTH	2.6

#define RX_WNDO1_SEC_LOW	((CODE_SEQUENZ_LENGTH-ALPHA_N1_LOW) * N1)
#define RX_WNDO2_SEC_LOW	((CODE_SEQUENZ_LENGTH-ALPHA_N2_LOW) * N2)
#define RX_WNDO3_SEC_LOW	((CODE_SEQUENZ_LENGTH-ALPHA_N3_LOW) * N3)
#define RX_WNDO1_SEC_HIGH	((CODE_SEQUENZ_LENGTH-ALPHA_N1_HIGH) * N1)
#define RX_WNDO2_SEC_HIGH	((CODE_SEQUENZ_LENGTH-ALPHA_N2_HIGH) * N2)
#define RX_WNDO3_SEC_HIGH	((CODE_SEQUENZ_LENGTH-ALPHA_N3_HIGH) * N3)

// AES command frequencies
#define FREQU1				426.67
#define FREQU2				640.00
#define FREQU3				853.33
#define FREQU4				1066.67
#define FREQU5				1280.00
#define FREQU6				1493.33
#define FREQU7				1706.67
#define FREQU8				1920.00
#define FREQU9				2133.33

