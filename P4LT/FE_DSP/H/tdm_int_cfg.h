/*-------------------------------------------------------------------------*
 * $Workfile: tdm_int_cfg.h $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Dr. Vladimir Potapov
 * Remarks      : Boost switches still missing
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source O4CV/P4LT/NSK_DSP/H/tdm_int_cfg.h $
 * 
 * 15    08-02-25 9:58 Chtognu
 * Merged changes from ad.equ.branch:
 * bitfield set for  correct NSK_TX_NO_AP_CHx operation
 * 
 * 14    07-09-12 10:17 Chchlee
 * UC_DSP_TX_TDM_MASK3 set to zero because UC debug words were removed
 * 
 * 13    07-08-28 13:12 Chchlee
 * Additional path for injection of NSK signals after allpass filter added
 * 
 * 12    1.03.07 17:35 Chhamaa
 * Defines for new time slots added (O4CV subframe start flags).
 * 
 * 11    10.01.07 10:13 Chhamaa
 * O4CV time slots added
 * 
 * 6     11.10.05 17:23 Chchlee
 * OFDM rx definitions activated again
 * 
 * 5     11.10.05 16:45 Chchlee
 * Unused definitions commented out
 * 
 * 4     17.06.05 15:48 Chhamaa
 * UP_CONV_SETTINGS added to UC_DSP_RX_TDM_MASK3
 * 
 * 3     23.05.05 17:07 Chpeplu
 * define DEBUG_WORD_0_NSD replaced by NSD_EVENTS and Maske3 for master
 * DSP changed
 * 
 * 2     16.02.05 15:52 Chhamaa
 * 
 * 19    11.02.05 18:34 Chhamaa
 * New time slot UP_CONV_SETTINGS added
 * 
 * 18    14.01.05 17:20 Chpeplu
 * New time slot APLC_SNR_CH1 and APLC_SNR_CH2 added
 * 
 * 17    13.01.05 18:46 Chhamaa
 * New time slot MASTER_OUT_FLAGS added
 * 
 * 16    20.12.04 9:31 Chchlee
 * slot for tdmBusAlarmWord added in mask 3
 * 
 * 15    29.09.04 11:07 Leeb02
 * 
 * 13    30.08.04 11:49 Rytz01
 * all debug words active
 * 
 * 12    27.08.04 13:17 Rytz01
 * debug-words NSK removed
 * 
 * 11    27.08.04 8:30 Rytz01
 * definitions for debug words added
 * 
 * 10    25.08.04 10:37 Plueer01
 * TDM-bit mask corrected for NSD DSP (Boost MGR-Input) changed form mask3
 * to mask2
 * 
 * 9     11.08.04 16:58 Leeb02
 * 
 * 8     11.08.04 16:51 Leeb02
 * 
 * 7     11.08.04 14:48 Leeb02
 * Boost switches added
 * 
 * 6     28.06.04 17:17 Unp00611
 * 
 * 5     28.06.04 14:16 Unp00611
 * 
 * 4     7.04.04 10:27 Leeb02
 * Harmonized with BASETYPE.H
 * 
 * 3     5.04.04 9:16 Leeb02
 * Update according to latest internal TDM bus document
 * 
 * 2     04-03-11 18:06 Leeb02
 * Initial ABB version
 *-------------------------------------------------------------------------*/


//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      p4lt_tdm.h
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        06-DEC-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    P4LT TDM time slots allocation masks definition
//             
//                  
// PREMISES:   
//
// REMARKS:    Refernece doc. TM-PTUKT03-1637
//  
// HISTORY:    06.12.03  Initial Version, vgp
//
//---------------------------------------------------------------------------

#ifndef TDM_INT_CFG_H
#define TDM_INT_CFG_H

#include "BASETYPE.H"

/***************************************************************/
/******                                                   ******/
/******        P4LT TDM time slots allocation masks       ******/
/******           for each DSP on the P4LT board          ******/
/******                                                   ******/
/****** Reference Doc no.: TM-PTUKT03-1637  12/25/2003    ******/
/******                                                   ******/
/***************************************************************/



/***************************************************************/
/******                                                   ******/
/******        P4LT TDM channel allocation masks          ******/
/******                                                   ******/
/***************************************************************/

// Mask 1
#define FRAME_ID         BIT0  /* Time Slot #0  : Frame ID, Toggle  BIT */
#define MASTER_DSP_SD    BIT1  /* Time Slot #1  : Master DSP service data */

#define APLC_TX_CH1      BIT2  /* Time Slot #2  : APLC Tx Signal Channel 1 */
#define APLC_TX_CH2      BIT3  /* Time Slot #3  : APLC Tx Signal Channel 2 */
#define APLC_TX_CH3      BIT4  /* Time Slot #4  : APLC Tx Signal Channel 3 */

#define MWIRE_TX_CH      BIT5  /* Time Slot #5  : M-Wire Signalling channel 1 - 3  */
#define EOC_TX_CH        BIT6  /* Time Slot #6  : EOC Tx Data channel 1 - 3 */

#define BOOST_MGR_OUTPUT BIT7  /* Time Slot #7  : Boost Manager Output */

#define FE_DSP_ALARM     BIT8  /* Time Slot #8  :  */
#define FE_DSP_SD        BIT9  /* Time Slot #9  : FE DSP Service Data */

#define NSD_DSP_ALARM    BIT10 /* Time Slot #10  :  */
#define NSD_DSP_SD       BIT11 /* Time Slot #11  : NSD DSP Service Data */

#define NSD_NSD_1ICSD    BIT12 /* Time Slot #12 :  */
#define NSD_NSD_1OCSD    BIT13 /* Time Slot #13 :  */
#define NSD_NSD_2ICSD    BIT14 /* Time Slot #14 :  */
#define NSD_NSD_2OCSD    BIT15 /* Time Slot #15 :  */

#define OFDM_DSP_ALARM   BIT16 /* Time Slot #16 : OFDM DSP Alarms Status Data */
#define OFDM_DSP_SD      BIT17 /* Time Slot #17 : OFDM DSP Service Data */

#define NSK_DSP_ALARM    BIT18 /* Time Slot #18 : NSK DSP Alarms Status Data */
#define NSK_DSP_SD       BIT19 /* Time Slot #19 : NSK DSP Service Data */

#define UC_DSP_ALARM     BIT20 /* Time Slot #20 : UC DSP Alarms Status Data */
#define UC_DSP_SD        BIT21 /* Time Slot #21 : UC DSP Service Data */


#define APLC_RX_CH1_NE   BIT22 /* Time Slot #22 : APLC Rx Signalling channel 1 (non equalized) */
#define APLC_RX_CH2_NE   BIT23 /* Time Slot #23 : APLC Rx Signalling channel 2 (non equalized) */
#define APLC_RX_CH3_NE   BIT24 /* Time Slot #24 : APLC Rx Signalling channel 3 (non equalized) */

#define APLC_RX_CH1_EQ   BIT25 /* Time Slot #25 : APLC Rx Signalling channel 1 (equalized) */
#define APLC_RX_CH2_EQ   BIT26 /* Time Slot #26 : APLC Rx Signalling channel 2 (equalized) */
#define APLC_RX_CH3_EQ   BIT27 /* Time Slot #27 : APLC Rx Signalling channel 3 (equalized) */

#define EWIRE_RX_CH      BIT28 /* Time Slot #28 : E-Wire Signalling channel 1 - 3 */
#define EOC_RX_CH        BIT29 /* Time Slot #29 : EOC Rx Signalling channel 1 - 3 */

#define OFDM_RX_I1       BIT30 /* Time Slot #30 : OFDM Modem Rx Signal (I1) */
#define OFDM_RX_Q1       BIT31 /* Time Slot #31 : OFDM Modem Rx Signal (Q1) */

// Mask 2
#define OFDM_RX_I2       BIT0  /* Time Slot #32 : OFDM Modem Rx Signal (I2) */
#define OFDM_RX_Q2       BIT1  /* Time Slot #33 : OFDM Modem Rx Signal (Q2) */
#define OFDM_RX_I3       BIT2  /* Time Slot #34 : OFDM Modem Rx Signal (I3) */
#define OFDM_RX_Q3       BIT3  /* Time Slot #35 : OFDM Modem Rx Signal (Q3) */
#define OFDM_RX_I4       BIT4  /* Time Slot #36 : OFDM Modem Rx Signal (I4) */
#define OFDM_RX_Q4       BIT5  /* Time Slot #37 : OFDM Modem Rx Signal (Q4) */
#define OFDM_RX_I5       BIT6  /* Time Slot #38 : OFDM Modem Rx Signal (I5) */
#define OFDM_RX_Q5       BIT7  /* Time Slot #39 : OFDM Modem Rx Signal (Q5) */
#define OFDM_RX_I6       BIT8  /* Time Slot #40 : OFDM Modem Rx Signal (I6) */
#define OFDM_RX_Q6       BIT9  /* Time Slot #41 : OFDM Modem Rx Signal (Q6) */

#define OFDM_TX_I1       BIT10 /* Time Slot #42 : OFDM Modem Tx Signal (I1) */
#define OFDM_TX_Q1       BIT11 /* Time Slot #43 : OFDM Modem Tx Signal (Q1) */
#define OFDM_TX_I2       BIT12 /* Time Slot #44 : OFDM Modem Tx Signal (I2) */
#define OFDM_TX_Q2       BIT13 /* Time Slot #45 : OFDM Modem Tx Signal (Q2) */
#define OFDM_TX_I3       BIT14 /* Time Slot #46 : OFDM Modem Tx Signal (I3) */
#define OFDM_TX_Q3       BIT15 /* Time Slot #47 : OFDM Modem Tx Signal (Q3) */
#define OFDM_TX_I4       BIT16 /* Time Slot #48 : OFDM Modem Tx Signal (I4) */
#define OFDM_TX_Q4       BIT17 /* Time Slot #49 : OFDM Modem Tx Signal (Q4) */
#define OFDM_TX_I5       BIT18 /* Time Slot #50 : OFDM Modem Tx Signal (I5) */
#define OFDM_TX_Q5       BIT19 /* Time Slot #51 : OFDM Modem Tx Signal (Q5) */
#define OFDM_TX_I6       BIT20 /* Time Slot #52 : OFDM Modem Tx Signal (I6) */
#define OFDM_TX_Q6       BIT21 /* Time Slot #53 : OFDM Modem Tx Signal (Q6) */

#define NSK_TX_CH1       BIT22 /* Time Slot #54 : NSK Modem Tx Signal Channel 1 */
#define NSK_TX_CH2       BIT23 /* Time Slot #55 : NSK Modem Tx Signal Channel 2 */
#define NSK_TX_CH3       BIT24 /* Time Slot #56 : NSK Modem Tx Signal Channel 3 */

#define NSD_TX_CH1_I     BIT25 /* Time Slot #57 : NSD Tx Signal Channel 1 (I) */
#define NSD_TX_CH1_Q     BIT26 /* Time Slot #58 : NSD Tx Signal Channel 1 (Q) */
#define NSD_TX_CH2_I     BIT27 /* Time Slot #59 : NSD Tx Signal Channel 2 (I) */
#define NSD_TX_CH2_Q     BIT28 /* Time Slot #60 : NSD Tx Signal Channel 2 (Q) */
#define NSD_TX_CH3_I     BIT29 /* Time Slot #61 : NSD Tx Signal Channel 3 (I) */
#define NSD_TX_CH3_Q     BIT30 /* Time Slot #62 : NSD Tx Signal Channel 3 (Q) */

#define BOOST_MGR_INPUT  BIT31 /* Time Slot #63 : Boost manager input */

// Mask 3
#define TDMBUS_ALM_WORD	  BIT0  /* Time Slot #64 : TDMBusAlarmWord Master to NSD*/
#define MASTER_OUT_FLAGS  BIT1  /* Time Slot #65 : Flags Master to FE*/
#define	APLC_SNR_CH1	  BIT2	/* Time Slot #66 : SNR value Ch1 from FE to NSD */
#define APLC_SNR_CH2	  BIT3	/* Time Slot #67 : SNR value Ch2 from FE to NSD */
#define UP_CONV_SETTINGS  BIT4	/* Time Slot #68 : up-converter settings from Master to UC */
#define O4CV_RX_SF_START  BIT5	/* Time Slot #69 : O4CV rx subframe start flags */
#define O4CV_RX_CH1_4	  BIT6	/* Time Slot #70 : O4CV rx data channels 1-4 */
#define O4CV_RX_CH5_8	  BIT7	/* Time Slot #71 : O4CV rx data channels 5-8 */
#define O4CV_RX_CH9_12	  BIT8	/* Time Slot #72 : O4CV rx data channels 9-12 */
#define O4CV_RX_CH13_16	  BIT9	/* Time Slot #73 : O4CV rx data channels 13-16 */
#define O4CV_TX_SF_START  BIT10	/* Time Slot #74 : O4CV tx subframe start flags */
#define O4CV_TX_CH1_4	  BIT11	/* Time Slot #75 : O4CV tx data channels 1-4 */
#define O4CV_TX_CH5_8	  BIT12	/* Time Slot #76 : O4CV tx data channels 5-8 */
#define O4CV_TX_CH9_12	  BIT13	/* Time Slot #77 : O4CV tx data channels 9-12 */
#define O4CV_TX_CH13_16	  BIT14	/* Time Slot #78 : O4CV tx data channels 13-16 */

#define DEBUG_WORD_0_FE	  BIT16 /* Time Slot #80 : Debug word 0 FE to Master*/
#define DEBUG_WORD_1_FE	  BIT17 /* Time Slot #81 : Debug word 1 FE to Master*/
#define DEBUG_WORD_2_FE	  BIT18 /* Time Slot #82 : Debug word 2 FE to Master*/
#define NSD_EVENTS		  BIT19 /* Time Slot #83 : Debug word 0 NSD to Master*/
#define DEBUG_WORD_1_NSD  BIT20 /* Time Slot #84 : Debug word 1 NSD to Master*/
#define DEBUG_WORD_2_NSD  BIT21 /* Time Slot #85 : Debug word 2 NSD to Master*/
#define DEBUG_WORD_0_NSK  BIT22 /* Time Slot #86 : Debug word 0 NSK to Master*/
#define DEBUG_WORD_1_NSK  BIT23 /* Time Slot #87 : Debug word 1 NSK to Master*/
#define DEBUG_WORD_2_NSK  BIT24 /* Time Slot #88 : Debug word 2 NSK to Master*/
#define DEBUG_WORD_0_OFDM BIT25 /* Time Slot #89 : Debug word 0 OFDM to Master*/
#define DEBUG_WORD_1_OFDM BIT26 /* Time Slot #90 : Debug word 1 OFDM to Master*/
#define DEBUG_WORD_2_OFDM BIT27 /* Time Slot #91 : Debug word 2 OFDM to Master*/

#define NSK_TX_NO_AP_CH1  BIT28 /* Time Slot #92 : NSK Modem Tx Signal Channel 1, bypass allpass */
#define NSK_TX_NO_AP_CH2  BIT29 /* Time Slot #93 : NSK Modem Tx Signal Channel 2, bypass allpass */
#define NSK_TX_NO_AP_CH3  BIT30 /* Time Slot #94 : NSK Modem Tx Signal Channel 3, bypass allpass */

#define FRAME_END         BIT31 /* Time Slot #95 :  */

/***************************************************************/
/******                                                   ******/
/******    DSPs TDM transmit and receive slot masks       ******/
/******                                                   ******/
/***************************************************************/

/* Master DSP TDM transmit slots allocation masks */
#define MASTER_DSP_TX_TDM_MASK1   (\
                                   FRAME_ID | MASTER_DSP_SD | BOOST_MGR_OUTPUT | \
                                   APLC_TX_CH1 | APLC_TX_CH2 | APLC_TX_CH3 | \
                                   MWIRE_TX_CH | EOC_TX_CH \
                                  )
#define MASTER_DSP_TX_TDM_MASK2   P4LT_ZERO
#define MASTER_DSP_TX_TDM_MASK3   (TDMBUS_ALM_WORD | MASTER_OUT_FLAGS | UP_CONV_SETTINGS | \
								   O4CV_TX_SF_START | O4CV_TX_CH1_4 | O4CV_TX_CH5_8 | \
								   O4CV_TX_CH9_12 |O4CV_TX_CH13_16 | FRAME_END)
#define MASTER_DSP_TX_TDM_MASK4   P4LT_ZERO

/* Master DSP TDM receive slots allocation masks */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
#define MASTER_DSP_RX_TDM_MASK1   ( \
                                   FE_DSP_SD | NSD_DSP_SD | \
                                   OFDM_DSP_SD | NSK_DSP_SD | UC_DSP_SD |\
                                   FE_DSP_ALARM | NSD_DSP_ALARM | \
                                   OFDM_DSP_ALARM | NSK_DSP_ALARM | UC_DSP_ALARM |\
                                   NSD_NSD_1ICSD | NSD_NSD_1OCSD |\
                                   NSD_NSD_2ICSD | NSD_NSD_2OCSD |\
                                   APLC_RX_CH1_NE | APLC_RX_CH2_NE | APLC_RX_CH3_NE | \
                                   APLC_RX_CH1_EQ | APLC_RX_CH2_EQ | APLC_RX_CH3_EQ | \
                                   EWIRE_RX_CH | EOC_RX_CH  \
                                  )
#define MASTER_DSP_RX_TDM_MASK2   BOOST_MGR_INPUT
#define MASTER_DSP_RX_TDM_MASK3   NSD_EVENTS
#define MASTER_DSP_RX_TDM_MASK4   P4LT_ZERO
*/

/* Teleprotection DSP TDM transmit slots allocation masks */
#define NSD_DSP_TX_TDM_MASK1   (NSD_DSP_ALARM | NSD_DSP_SD | \
                                NSD_NSD_1ICSD | NSD_NSD_1OCSD |\
                                NSD_NSD_2ICSD | NSD_NSD_2OCSD)
#define NSD_DSP_TX_TDM_MASK2   ( \
                               NSD_TX_CH1_I | NSD_TX_CH1_Q | NSD_TX_CH2_I | NSD_TX_CH2_Q | \
                               NSD_TX_CH3_I | NSD_TX_CH3_Q | BOOST_MGR_INPUT \
                              )
#define NSD_DSP_TX_TDM_MASK3   (NSD_EVENTS | DEBUG_WORD_1_NSD | DEBUG_WORD_2_NSD)
#define NSD_DSP_TX_TDM_MASK4   P4LT_ZERO

/* Teleprotection DSP TDM receive slots allocation masks */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
#define NSD_DSP_RX_TDM_MASK1   ( \
                               FRAME_ID | MASTER_DSP_SD | \
                               APLC_RX_CH1_NE | APLC_RX_CH2_NE | APLC_RX_CH3_NE  \
                              )
#define NSD_DSP_RX_TDM_MASK2   P4LT_ZERO
#define NSD_DSP_RX_TDM_MASK3   (TDMBUS_ALM_WORD |APLC_SNR_CH1|APLC_SNR_CH2| FRAME_END)
#define NSD_DSP_RX_TDM_MASK4   P4LT_ZERO
*/

/* Frontend DSP TDM transmit slots allocation masks */
#define FE_DSP_TX_TDM_MASK1   ( \
                               FE_DSP_SD | FE_DSP_ALARM | \
                               APLC_RX_CH1_NE | APLC_RX_CH2_NE | APLC_RX_CH3_NE | \
                               APLC_RX_CH1_EQ | APLC_RX_CH2_EQ | APLC_RX_CH3_EQ | \
                               EWIRE_RX_CH | EOC_RX_CH | \
                               OFDM_RX_I1 | OFDM_RX_Q1 \
                              )
#define FE_DSP_TX_TDM_MASK2   ( \
                               OFDM_RX_I2 | OFDM_RX_I3 | OFDM_RX_I4 | OFDM_RX_I5 | OFDM_RX_I6 | \
                               OFDM_RX_Q2 | OFDM_RX_Q3 | OFDM_RX_Q4 | OFDM_RX_Q5 | OFDM_RX_Q6  \
							  )

#define FE_DSP_TX_TDM_MASK3   (APLC_SNR_CH1 | APLC_SNR_CH2 | DEBUG_WORD_0_FE | DEBUG_WORD_1_FE | DEBUG_WORD_2_FE)
#define FE_DSP_TX_TDM_MASK4   P4LT_ZERO

/* Frontend DSP TDM receive slots allocation masks */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
#define FE_DSP_RX_TDM_MASK1   ( \
                               FRAME_ID | MASTER_DSP_SD | BOOST_MGR_OUTPUT | \
                               MWIRE_TX_CH | EOC_TX_CH | \
                               APLC_TX_CH1 | APLC_TX_CH2 | APLC_TX_CH3 \
                              )
#define FE_DSP_RX_TDM_MASK2   ( \
                               OFDM_TX_I1 | OFDM_TX_I2 | OFDM_TX_I3 | OFDM_TX_I4 | OFDM_TX_I5 | OFDM_TX_I6 | \
                               OFDM_TX_Q1 | OFDM_TX_Q2 | OFDM_TX_Q3 | OFDM_TX_Q4 | OFDM_TX_Q5 | OFDM_TX_Q6 | \
                               NSK_TX_CH1 | NSK_TX_CH2 | NSK_TX_CH3 | \
                               NSD_TX_CH1_I | NSD_TX_CH1_Q | NSD_TX_CH2_I | NSD_TX_CH2_Q | \
                               NSD_TX_CH3_I | NSD_TX_CH3_Q \
                              )
#define FE_DSP_RX_TDM_MASK3   FRAME_END
#define FE_DSP_RX_TDM_MASK4   P4LT_ZERO
*/

/* NSK modem TDM transmit slots allocation masks */
#define NSK_DSP_TX_TDM_MASK1  (NSK_DSP_SD | NSK_DSP_ALARM)
#define NSK_DSP_TX_TDM_MASK2  (NSK_TX_CH1 | NSK_TX_CH2 | NSK_TX_CH3)
#define NSK_DSP_TX_TDM_MASK3  (DEBUG_WORD_0_NSK | DEBUG_WORD_1_NSK | DEBUG_WORD_2_NSK | \
							   NSK_TX_NO_AP_CH1 | NSK_TX_NO_AP_CH2 | NSK_TX_NO_AP_CH3)
#define NSK_DSP_TX_TDM_MASK4  P4LT_ZERO

/* NSK modem TDM receive slots allocation masks */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
#define NSK_DSP_RX_TDM_MASK1  ( \
                               FRAME_ID | MASTER_DSP_SD | BOOST_MGR_OUTPUT | \
                               APLC_RX_CH1_EQ | APLC_RX_CH2_EQ | APLC_RX_CH3_EQ \
                              )
#define NSK_DSP_RX_TDM_MASK2  P4LT_ZERO
#define NSK_DSP_RX_TDM_MASK3  FRAME_END
#define NSK_DSP_RX_TDM_MASK4  P4LT_ZERO
*/

/* OFDM modem TDM transmit slots allocation masks */
#define OFDM_DSP_TX_TDM_MASK1 (OFDM_DSP_SD | OFDM_DSP_ALARM )
#define OFDM_DSP_TX_TDM_MASK2 ( \
                               OFDM_TX_I1 | OFDM_TX_I2 | OFDM_TX_I3 | OFDM_TX_I4 | OFDM_TX_I5 | OFDM_TX_I6 | \
                               OFDM_TX_Q1 | OFDM_TX_Q2 | OFDM_TX_Q3 | OFDM_TX_Q4 | OFDM_TX_Q5 | OFDM_TX_Q6 \
							  )
#define OFDM_DSP_TX_TDM_MASK3 P4LT_ZERO
#define OFDM_DSP_TX_TDM_MASK4 P4LT_ZERO

/* OFDM modem TDM receive slots allocation masks */

#define OFDM_DSP_RX_TDM_MASK1 ( \
                               FRAME_ID | MASTER_DSP_SD | \
                               OFDM_RX_I1 | OFDM_RX_Q1\
					          )
#define OFDM_DSP_RX_TDM_MASK2 ( \
                               OFDM_RX_I2 | OFDM_RX_I3 | OFDM_RX_I4 | OFDM_RX_I5 | OFDM_RX_I6 | \
                               OFDM_RX_Q2 | OFDM_RX_Q3 | OFDM_RX_Q4 | OFDM_RX_Q5 | OFDM_RX_Q6 \
					          )
#define OFDM_DSP_RX_TDM_MASK3 FRAME_END
#define OFDM_DSP_RX_TDM_MASK4 P4LT_ZERO


/* UC TDM transmit slots allocation masks */
#define UC_DSP_TX_TDM_MASK1 (UC_DSP_SD | UC_DSP_ALARM)
#define UC_DSP_TX_TDM_MASK2 P4LT_ZERO
#define UC_DSP_TX_TDM_MASK3	P4LT_ZERO
#define UC_DSP_TX_TDM_MASK4 P4LT_ZERO

/* UC TDM receive slots allocation masks */
/* not used because all DSPs receive all slots, i.e. t_AllDSP_input_tdm_block
#define UC_DSP_RX_TDM_MASK1 (FRAME_ID | MASTER_DSP_SD)
#define UC_DSP_RX_TDM_MASK2 P4LT_ZERO
#define UC_DSP_RX_TDM_MASK3 (UP_CONV_SETTINGS | FRAME_END)
#define UC_DSP_RX_TDM_MASK4 P4LT_ZERO
*/

#endif
