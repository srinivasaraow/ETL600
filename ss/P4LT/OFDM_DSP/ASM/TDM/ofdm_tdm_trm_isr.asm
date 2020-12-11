//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      OFDM_TDM_Trm_ISR.asm
//
// PROJECT:    P4LT system board of ETL600 on DRS7 board
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        24-FEB-2003
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    ofdm transmitter SPORT3 TDM interrupt handler 
//             
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    24.02.04  Initial Version, vgp
//             26.06.04  SFC to TDM integration, vgp
//             17.09.04  Master_DSP <- OFDM_DSP Control channel implementation, vgp
//             24.01.05  New format of TDM ofdm alarm time slot, vgp
//
//---------------------------------------------------------------------------
/* $Log: /branches/Source MAS/P4LT/OFDM_DSP/ASM/TDM/ofdm_tdm_trm_isr.asm $
 * 
 * 11    24.07.08 15:13 Chhamaa
 * 
 * 6     5.04.06 15:28 Chhamaa
 * 
 * 6     17.03.06 14:41 Chhamaa
 * 
 * 4     26.04.05 8:41 Chalgyg
 * OFDM_DSP_V0_09R
 * 
 * 3     28.01.05 11:49 Ats
 * 
 * 7     2.12.04 17:38 Chhamaa
 * 
 * 6     30.09.04 8:57 Leeb02
 * 
 * 5     30.09.04 8:27 Leeb02
 */	
#include <def21161.h>
#include <asm_sprt.h>

#include "../../h/ofdm_cfg_types.h"

.global _OFDM_TDM_Trm_ISR;
.global _n_rest_csamples;
.global TX_Semaphore;
.global _data_2tx_overrun; 
.global _data_2tx_request;
.global _data_2tx_grant;
.global _Toggle_TDM_TX;
.global _Ofdm_DSP_SD;
.global _Ofdm_DSP_ALARM;
.global	_TDM_RTS_Flag;      

.extern _NL2Q;
.extern _IsModemStarted;
.extern _out_bptr;              // points received complex array to read
.extern _out_cur_bptr;          // points received complex array to read
.extern _out_first_bptr;
.extern _out_last_bptr;

.extern _ControlTX_Service_Write_Ind;
.extern _ControlTX_Service_Read_Ind;
.extern _ControlTX_Service_Buffer_Ptr;
.extern _ControlTX_Service_Buffer_Length;
.extern _ControlTX_Service_Buffer_Empty;
.extern _ControlTX_Service_Write_Is_Updated;
.extern _ControlTX_Service_Delivered_Words;
.extern _ControlTX_Service_Number_of_Words;
.extern _OFDM_Alarm;
.extern _LifesignWatchDog;

#define LIFESIGN_COUNTER        100
#define OFDM_IO_SIGNAL_CSAMPLES 6
#define OFDM_IO_SIGNAL_RSAMPLES (2*OFDM_IO_SIGNAL_CSAMPLES)

/*---------------------------------------------------------------------------*/
	
.segment/dm    seg_dmda;

.var _data_2tx_overrun; // flag
.var TX_Semaphore;
.var SaveASTATx;
.var _n_rest_csamples;
.var _data_2tx_request;
.var _data_2tx_grant;
.var _Toggle_TDM_TX = 0;
.var _Ofdm_DSP_SD = 0;
.var _Ofdm_DSP_ALARM = 0;
.var _TDM_RTS_Flag = 0;
.var OFDM_ALARM_Slot;

// life sign to master dsp
.var lifesign_couter=0;
.var myLifesign=0;

.var Mode1_TX = SRRFL | SRD1L | CBUFEN | RND32 | IRPTEN;
.align 2;
.var SaveMode1_TX[2];

.segment/pm 	seg_pmco;
	
_OFDM_TDM_Trm_ISR:

     bit clr IMASK SP1I;
     dm(SaveMode1_TX) = mode1;
     bit clr mode1 PEYEN;
     nop;
     mode1 = dm(Mode1_TX);      
     m6 = 1;                                    // wait for are shadows ready
     dm(TX_Semaphore) = m6;
     dm(SaveASTATx) = astat;
     bit set IMASK SP1I;
          
     r0 = dm(_Toggle_TDM_TX);
     r1 = 1;
     r0 = r0 xor r1;
     dm(_Toggle_TDM_TX) = r0;

//jump norm_exit;
     
     m5 = 0;
     // get pointer to tdm tx buffer
     r0 = dm(GP3A);               // get transmitter tdm buffer address
     r0 = bset r0 BY 18;          // set the pci bit 
	 i0 = r0;
	 call CommandTX_Service;
     
     r0 = dm(_IsModemStarted);      // test for modem startup flag
     r0 = pass r0;
     if ne jump verify_request;

      dm(_data_2tx_overrun) = m5;   // clear tx overrun flag
      dm(_data_2tx_grant)   = m5;   // clear tx grant flag
      r1 = 1;
      dm(_data_2tx_request) = r1;
      	 
	  // send zero data 
	  r1 = r1 - r1;
      r2 = OFDM_IO_SIGNAL_RSAMPLES;
cp_zr:  r2 = r2 - 1;
        if ne jump cp_zr(db);
        dm(i0,m6) = r1;
        nop;
        
      // return from interrupt 
      dm(TX_Semaphore) = m5;
      astat = dm(SaveASTATx);
      mode1 = dm(SaveMode1_TX);
      rti;                          // return from interrupt
      
verify_request:
/*
dm(CP1A) = m5;
dm(CP3A) = m5;
nop;
*/
     b1 = dm(_out_first_bptr);	// prepare ring buffer
     i1 = dm(_out_cur_bptr);
     r0 = dm(_out_first_bptr);
     r1 = dm(_out_last_bptr);
     r1 = r1 - r0;
     r0 = r1 + 1;
     r0 = r0 + 1;
     l1 = r0;
     
      // request was granted
     r0 = dm(_data_2tx_grant);    // was request to copy
     r0 = pass r0;
     if eq jump cp_2buf;          // was not granted request to copy     
      
     dm(_data_2tx_grant) = m5;    // clear grant

/*      
dm(CP1A) = m5;
dm(CP3A) = m5;
nop;
*/

cp_2buf:
     // test for buffer overflow
     r0 = dm(_n_rest_csamples);
     r1 = OFDM_IO_SIGNAL_CSAMPLES;
     r1 = r0 - r1;
     if lt jump data_overrun;
     
     // copy data from intermediate buffer in to output
     r1 = OFDM_IO_SIGNAL_RSAMPLES;
cp_buf: r1 = r1 - 1;
        if ne jump cp_buf(db);
        r0 = dm(i1,m6);
        dm(i0, m6) = r0;       // store sample to TDM output
     
     // update buffer pointer   
     dm(_out_cur_bptr) = i1;
     // update complex sample counter
     r0 = dm(_n_rest_csamples);
     r1 = OFDM_IO_SIGNAL_CSAMPLES;
     r0 = r0 - r1;
     dm(_n_rest_csamples) = r0;
     // check number of complex samples in buffer
     r1 = dm(_NL2Q);
     r1 = r0 - r1;
     if ge jump norm_exit;
/*     
dm(CP1A) = m5;
dm(CP3A) = m5;
nop;
*/
     // rise request for data , if number of samples in buffer is smaller than for one OFDM symbol
     dm(_data_2tx_request) = m6;
     
     // DEBUG Phase-Control: Messpunkt Flag1 wird gesetzt, falls TX-Flag gesetzt wird
//*(long*)(IOFLAG) |= FLG1; // setzt Flag 1
bit set FLAGS FLG1;		
	    nop;
     
norm_exit:
     l1 = 0;
     b1 = 0;    
     dm(TX_Semaphore) = l1;
     astat = dm(SaveASTATx);
     mode1 = dm(SaveMode1_TX);
     rti;                         // return from interrupt


data_overrun:
     l1 = 0;
     b1 = 0;    
     dm(_data_2tx_overrun) = m6;
     dm(TX_Semaphore) = l1;
     astat = dm(SaveASTATx);
     mode1 = dm(SaveMode1_TX);
     rti;

//===========================================================	  
CommandTX_Service:

    // test alarm flag
    r2 = dm(_Ofdm_DSP_ALARM);    // test Ofdm_DSP_ALARM variable for 1
	r2 = pass r2;
	if eq jump send_Lifesign;
	  dm(lifesign_couter) = m5;
	  dm(myLifesign) = m5;
	  r1 = 0xffffffff;
      dm(i0,m6) = r1;           // send alarm word 0xffffffff if Ofdm_DSP_ALARM is 1
      jump send_message;

send_Lifesign:	              // Ofdm_DSP_ALARM is eq to 0, send message word, incl. lifesign
    // send life sign	  
	r1 = dm(lifesign_couter);
	r1 = pass r1;
	if eq jump update_MyLifesign; // 3 LSB bits
	r1 = r1 - 1;
	dm(lifesign_couter) = r1;
	r1 = 0xfffffff8;              // Mask to insert internaly generated lifesign in bits 2:0
      r2 = dm(_OFDM_Alarm);
      r2 = r2 and r1;
	r1 = dm(myLifesign);
      r1 = r1 or r2;                //  add life sign to OFDM alarm word
      dm(i0,m6) = r1;               //  send inf. in OFDM Alarm time slot with previous lifesign
	jump send_message;

alarm_LifesignWatchDog:				// PL 
	r1 = r1 - r1;					// PL set LifesignWatchDog to 1
	r1 = r1 + 1;					// PL
	dm(_LifesignWatchDog) = r1;		// PL
	jump send_message;				// PL
	
update_MyLifesign:	
    r0 = LIFESIGN_COUNTER;          // period to update lifesign counter
    dm(lifesign_couter) = r0;
	r1 = 0xfffffff8;              // Mask to insert internaly generated lifesign in bits 2:0
      r2 = dm(_OFDM_Alarm);
      r2 = r2 and r1;
	r1 = dm(myLifesign);
      r1 = r1 or r2;                // add life sign to OFDM alarm word
      dm(i0,m6) = r1;               // send lifesign 8 LSB
    
    r1 = dm(_LifesignWatchDog);		// PL: decrement MainLoop Watchdog
    r1 = r1 - 1;					// PL
    dm(_LifesignWatchDog) = r1;		// PL
    r1 = pass r1;					// PL  if zero, goto alarm_LifesignWatchDog
    if eq jump alarm_LifesignWatchDog; // PL
    								
	r1 = dm(myLifesign);			// else update myLifesign
	r1 = r1 + 1;
      r0 = BIT_3;                   // myLifesign := myLifesign mod 4
      r0 = r0 and r1;
      if ne r1 = r1 - r1;
	dm(myLifesign) = r1;

    // send message
send_message:    
    r0 = dm(_ControlTX_Service_Delivered_Words);
	r0 = pass r0;
	if eq jump verify_nwords(db);
    r1 = dm(_ControlTX_Service_Number_of_Words);
	r1 = r0 + r1;
	  dm(_ControlTX_Service_Number_of_Words) = r1;
	  dm(_ControlTX_Service_Write_Is_Updated) = m6;
	  dm(_ControlTX_Service_Delivered_Words) = m5;
verify_nwords:
    r1 = pass r1;
	if eq jump Nothing_to_transmit;
      // transmit data
	  r1 = dm(_ControlTX_Service_Read_Ind);	  
      i1 = dm(_ControlTX_Service_Buffer_Ptr);
	  m1 = r1;	  
	  r1 = r1 + 1, r0 = dm(m1,i1);
	  dm(i0,m6) = r0;
	  r0 = dm(_ControlTX_Service_Buffer_Length);
	  r0 = r0 - r1;
	  if eq r1 = r1 - r1;
	  dm(_ControlTX_Service_Read_Ind) = r1;
      r1 = dm(_ControlTX_Service_Number_of_Words);
      r1 = r1 - 1;
      dm(_ControlTX_Service_Number_of_Words) = r1;
	  dm(_TDM_RTS_Flag) = m6;       // set flag	  
      rts;

Nothing_to_transmit:
    // transmit all ones on control reply slot
    dm(_ControlTX_Service_Write_Is_Updated) = m6;
	dm(_ControlTX_Service_Delivered_Words) = m5;
    r0 = r0 - r0;
    r0 = r0 - 1;
    dm(i0,m6) = r0;
	dm(_TDM_RTS_Flag) = m6;       // set flag
    rts;
//===========================================================	  

_OFDM_TDM_Trm_ISR.end:

