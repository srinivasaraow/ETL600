//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2003 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      p4lt_tdm_defs.h
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        06-DEC-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    P4LT TDM parameters definition
//             
//                  
// PREMISES:   
//
// REMARKS:    Refernece doc. TM-PTUKT03-1637
//  
// HISTORY:    06.12.03  Initial Version, vgp
//
//---------------------------------------------------------------------------


#ifndef P4LT_TDM_DEFS_H
#define P4LT_TDM_DEFS_H

#include <complex.h>
/***************************************************************/
/******                                                   ******/
/******        P4LT TDM parameters difinition             ******/
/******                                                   ******/
/****** Reference Doc no.: TM-PTUKT03-1637  11/04/2003    ******/
/******                    TM-PTUKT03-1637  12/05/2003    ******/ 
/******                                                   ******/
/***************************************************************/

#define DSP_Fcclk_Hz           98304000 // FE ADSP-21161N core clock frequency in Hz

#define P4LT_TDM_NCH_MAX             96 // number of time slots in TDM frame
#define P4LT_TDM_SLEN                32 // number of bits in TDM time slot
#define FE_TDM_N_RX_MAX         TDM_NCH // number of time slots for transmittion
#define FE_TDM_N_TX_MAX         TDM_NCH // number of time slots for reception

#define SLEN_MAX                     32 // ADSP-21161N maximum allowed bits per time slot
#define TDM_NCH_MAX                 128 // ADSP_21161N maximum allowed time slot in tDM bus

#define TDM_MASTER_ON                     1 // DSP generates IFS signal of serial port
#define TDM_MASTER_OFF                    0 // DSP accepts IFS signal on serial port
#define TDM_ISSPORT13_OFF                 0 // use SPORT02 as TDM
#define TDM_ISSPORT13_ON                  1 // use SPORT13 as TDM
#define TDM_ISSPORT02_ON  TDM_ISSPORT13_OFF // use SPORT02 as TDM


#define OFDM_IO_SIGNAL_CSAMPLES       6 // number of OFDM complex samples in TDM frame
#define TP_TX_SIGNAL_CSAMPLES         3 // number of transmitted TP complex samples in TDM frame
#define OFDM_IO_SIGNAL_RSAMPLES       (sizeof(complex_float)*OFDM_IO_SIGNAL_CSAMPLES)
#define TP_TX_SIGNAL_RSAMPLES         (sizeof(complex_float)*TP_TX_SIGNAL_CSAMPLES)

#define TDM_ALERT  (2*OFDM_IO_SIGNAL_CSAMPLES)

#endif
