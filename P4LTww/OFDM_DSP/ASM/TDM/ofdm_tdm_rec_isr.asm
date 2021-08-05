//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      OFDM_TDM_Rec_ISR.asm
//
// PROJECT:    P4LT system board of ETL600 on DRS7 board
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        24-FEB-2004
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    ofdm receiving SPORT1 TDM interrupt handler 
//             
//                  
// PREMISES:   
//
// REMARKS:    
//  
// HISTORY:    24.02.04  Initial Version, vgp
//             26.06.04  SFC to TDM integration, vgp
//             16.09.04  Master_DSP -> OFDM_DSP Control channel implementation, vgp
//
//---------------------------------------------------------------------------
/* $Log: /branches/Source MAS/P4LT/OFDM_DSP/ASM/TDM/ofdm_tdm_rec_isr.asm $
 * 
 * 10    24.07.08 15:13 Chhamaa
 * 
 * 5     5.04.06 15:28 Chhamaa
 * 
 * 5     17.03.06 14:41 Chhamaa
 * 
 * 3     5.07.05 11:15 Chalgyg
 * OFDM_DSP_V0_09U
 * 
 * 1     19.01.05 15:37 Chchlee
 * 
 * 6     30.09.04 8:57 Leeb02
 * 
 * 5     30.09.04 8:27 Leeb02
 */	
#include <def21161.h>
#include <asm_sprt.h>

#include "../../h/ofdm_cfg_types.h"

.global _OFDM_TDM_Rec_ISR;
.global _data_2rx_request;
.global _data_2rx_grant;

.extern _NL2Q;
.extern _IsModemStarted;
.extern _in_bptr;              // points received complex array to read
.extern _in_first_bptr;
.extern _in_cur_bptr;
.extern _in_last_bptr;
.extern _Toggle_TDM_TX;

// TDM Control Channel buffer variables
.extern _ControlRX_Service_Write_Ind;
.extern _ControlRX_Service_Read_Ind;
.extern _ControlRX_Service_Buffer_Ptr;
.extern _ControlRX_Service_Buffer_Length;

.extern _ControlRX_Service_Message_Ready;
.extern _ControlRX_Service_Busy_Flag;
.extern _ControlRX_Service_Corrupted_Message;
.extern _ControlRX_Service_Number_of_Messages;
.extern _ControlRX_Service_Number_of_Words;
.extern _ControlRX_Service_Read_Is_Updated;
.extern _ControlRX_Service_Buffer_Overrun;
.extern _ControlRX_Service_Delivered_Words;
.extern _ControlRX_Service_Secondary_Start;

.global _data_2rx_overrun;
.global n_rx_csamples; 
.global RX_Semaphore;
.global _Master_DSP_SD;
.global _Frame_ID;
.global _TDM_DTR_Flag;

#define OFDM_IO_SIGNAL_CSAMPLES 6
#define OFDM_IO_SIGNAL_RSAMPLES (2*OFDM_IO_SIGNAL_CSAMPLES)
#define TDM_ALERT  (2*OFDM_IO_SIGNAL_CSAMPLES)

/*---------------------------------------------------------------------------*/

.segment/dm    seg_dmda;

.var n_rx_csamples;                                            // number of complex samples in buffer
//.var n_blk_rx;                                                 // number of full Q*OFDM_NL2 blocks in input buffer
.var _data_2rx_overrun; // flag
.var SaveASTATx;
.var RX_Semaphore;
.var _data_2rx_request;
.var _data_2rx_grant;
.var _Master_DSP_SD;
.var _Frame_ID;
.var _TDM_DTR_Flag = 0;
.var RX_Message_End_Index;
.var RX_Message_Index;


.var Mode1_RX = SRRFL | SRD1H | CBUFEN | RND32 | IRPTEN;

.align 2;
.var SaveMode1_RX[2];

.segment/pm 	seg_pmco;
	
_OFDM_TDM_Rec_ISR:

     bit clr imask SP3I;
     dm(SaveMode1_RX) = mode1;
     bit clr mode1 PEYEN;
     nop;
     mode1 = dm(Mode1_RX);
     m6 = 1;                           // wait for are shadows ready
     dm(SaveASTATx) = astat;
     dm(RX_Semaphore) = m6;
     bit set imask SP3I;

yyy: 
     r4 = dm(GP1A);               // get received tdm buffer address
     m5 = 0;
     r4 = bset r4 BY 18;          // set the pci bit 
     b4 = 0;
     l4 = 0;    
     i4 = r4;
     r4 = dm(_IsModemStarted);      // test for modem startup flag
     r4 = pass r4;                
     if ne jump ModemIsStarted;     // jump if modem started
      dm(_data_2rx_overrun) = m5;   // clear RX overrun flag
      dm(_data_2rx_request) = m5;   // clear RX request for copying
      dm(_data_2rx_grant)   = m5;   // clear RX grant flag
      dm(n_rx_csamples) = m5;       // clear complex sample counter in rx buffer
//      dm(n_blk_rx) = m5;            // clear counter of number of OFDM symbol in rx buffer
	  
	  // get control int information from master dsp
      r4 = dm(i4,m6);              // get Frame_ID
	  dm(_Frame_ID)=r4;            // store Frame_ID
      r4 = dm(i4,m6);              // r4 = message word from Master_DSP_SD time slot      
	  dm(_Master_DSP_SD)=r4;       // store Control Data from Master DSP
	  call CommandRX_Service;	  
	  dm(RX_Semaphore) = m5;
      astat = dm(SaveASTATx);
      mode1 = dm(SaveMode1_RX);
      rti;                          // return from interrupt
      
ModemIsStarted:
//jump norm_exit;
     r4 = dm(GP1A);               // get received tdm buffer address
/*
r7 = r7 - r7;
dm(CP1A) = r7;
dm(CP2A) = r7;
nop; 
*/
     r4 = bset r4 BY 18;          // set the pci bit 
     i4 = r4;
     r4 = dm(i4,m6);              // get Frame_ID
	 dm(_Frame_ID)=r4;            // store Frame_ID
     r4 = dm(i4,m6);              // r4 = message word from Master_DSP_SD time slot      
	 dm(_Master_DSP_SD)=r4;       // store Control Data from Master DSP
     
     b5 = dm(_in_first_bptr);	// prepare ring buffer
     i5 = dm(_in_cur_bptr);
     r4 = dm(_in_first_bptr);
     r5 = dm(_in_last_bptr);
     r5 = r5 - r4;
     r4 = r5 + 1;
     r4 = r4 + 1;
     l5 = r4;
          
     r4 = dm(_data_2rx_request);
     r4 = pass r4;                // test for rx request
     if eq jump cp_2buf;          // NO REQUEST FOR READING, copy received samples into buffer

      // was request to main to copy
      r4 = dm(_data_2rx_grant);
      r4 = pass r4;               // test grant from modem
      if eq jump cp_2buf;         // test number of blocks in buffer

      // request was granted
/*
r7 = r7 - r7;
dm(CP1A) = r7;
dm(CP2A) = r7;
nop; 
*/
      dm(_data_2rx_grant) = m5;   // clear grant
      r4 = dm(n_rx_csamples);     // load number of complex samples in rx buffer
      r4 = pass r4;
      if lt jump data_overrun1;


cp_2buf:
/*
r7 = r7 - r7;
dm(CP1A) = r7;
dm(CP3A) = r7;
nop;
//jump yyy; 
*/

     // copy received TDM samples in to buffer
      r4 = OFDM_IO_SIGNAL_RSAMPLES;
	  r5 = dm( i4, m6 );    // get data from tdm
	  f5 = pass f5;
cp_tdm:
         bit tst ASTAT ASTAT_AI;
         if tf r5 = r5 - r5;
		 f6 = f5 * f5;             
		 bit tst ASTAT ASTAT_MI;   
		 if tf r5 = r5 - r5;		 	
		 r4 = r4 - 1;
		 if ne jump cp_tdm(db);     
		  r6 = dm( i4, m6 );                  // get data from tdm
          f5 = pass f6, dm( i5, m6 ) = r5;    // store data in buffer

     // update pointer to intermediate buffer    
     dm(_in_cur_bptr) = i5;     
     // update number of complex samples in buffer
     r4 = OFDM_IO_SIGNAL_CSAMPLES;
     r5 = dm(n_rx_csamples);
     r4 = r4 + r5;
     dm(n_rx_csamples) = r4;   // update number of complex samples in buffer
     // test number of complex samples
     r5 = dm(_NL2Q);            // load number of complex samples in OFDM symbol
     r4 = r4 - r5;
     if lt jump norm_exit;

     dm(_data_2rx_request) = m6; // set request for data processing, if
								 //	 if number of samples in buffer is at least sufficient for one OFDM symbol

// DEBUG Phase-Control: Messpunkt Flag2 wird gesetzt, falls RX-Flag gesetzt wird
//*(long*)(IOFLAG) |= FLG2; // setzt Flag 2
bit set FLAGS FLG2;		
	    nop;
								 
//     dm(n_blk_rx) = m6;		
/*   
r7 = r7 - r7;
dm(CP1A) = r7;
dm(CP3A) = r7;
nop; 
jump yyy;
*/
         
norm_exit:
	 call CommandRX_Service;	  
     b5 = 0;
     l5 = 0;    
     dm(RX_Semaphore) = m5;
     astat = dm(SaveASTATx);
     mode1 = dm(SaveMode1_RX);
     rti;                          // return from interrupt


data_overrun1:
	 call CommandRX_Service;	  
     b5 = 0;
     l5 = 0;    
     dm(_data_2rx_overrun) = m6;
     dm(RX_Semaphore) = m5;
     astat = dm(SaveASTATx);
     mode1 = dm(SaveMode1_RX);
     rti;                          // return from interrupt

     
//==============================================================
CommandRX_Service:	  
      // update Number of Messages in Buffer
      r5 = dm(_ControlRX_Service_Read_Is_Updated);
      r5 = pass r5;
      if ne jump Get_Message_Word;
        r6 = dm(_ControlRX_Service_Number_of_Messages);
        r6 = r6 - 1;
        dm(_ControlRX_Service_Number_of_Messages) = r6;
        r6 = dm(_ControlRX_Service_Number_of_Words);
        r5 = dm(_ControlRX_Service_Delivered_Words);
        r6 = r6 - r5;
        dm(_ControlRX_Service_Number_of_Words) = r6;
        dm(_ControlRX_Service_Delivered_Words) = m5;
        dm(_ControlRX_Service_Read_Is_Updated) = m6;
      
Get_Message_Word:      
      // Get Masters's DSP message word from TDM bus 
      r4 = dm(_Master_DSP_SD);              // r4 = message word from Master_DSP_SD time slot      
      
      // test buffer overrun flag
      r5 =  dm(_ControlRX_Service_Buffer_Overrun);
      r5 = pass r5;
      if ne rts;  // if buffer was overrun rts
      
      // Mesage word for START MESSAGE code
      r5 = TDM_SERVICE_START_CHAR;
	  r5 = r4 xor r5;             // compare received word with "start"
      if ne jump Non_Start_Message;
        // "START" message word is received
        r5 = dm(_ControlRX_Service_Busy_Flag);
        r5 = pass r5;
        if eq jump New_Message_Start_Is_OK;
          dm(_ControlRX_Service_Busy_Flag) = m5;
          nop;
          // ignore previsous message
          // update number of received chars.
          r6 = dm(_ControlRX_Service_Number_of_Words);
          r5 = dm(RX_Message_Index);
          r6 = r6 - r5;
          dm(_ControlRX_Service_Number_of_Words) = r6;
          // update buffer write index
	      r6 = dm(_ControlRX_Service_Write_Ind);
	      r6 = r6 - r5;
          if ge jump Secondary_Start_Flag; 
            r5 = dm(_ControlRX_Service_Buffer_Length);
            r6 = r6 + r5;
Secondary_Start_Flag: 
          dm(_ControlRX_Service_Write_Ind) = r6;
          dm(_ControlRX_Service_Secondary_Start) = m6;         
New_Message_Start_Is_OK:
        dm(_ControlRX_Service_Busy_Flag) = m6;
        dm(RX_Message_Index) = m5;
        jump Store_Message_Word;
Non_Start_Message:
      r5 = dm(_ControlRX_Service_Busy_Flag);
      r5 = pass r5;
      if eq rts;
      // varify validity of TDM address message
      r5 = dm(RX_Message_Index);
      r6 = r5 - 1;
      if ne jump Test_For_Control_Messages;
        // look for the right OFDM DSP address
        r6 = e_TDMServiceAddr_OFDM;
        r6 = r4 - r6;
        if eq jump Store_Message_Word;
          // ignore message because of wrong TDM address
//          dm(_ControlRX_Service_Message_Ready) = m5; // --??
          // update number of words counter
          r5 = dm(_ControlRX_Service_Number_of_Words);
          r5 = r5 - 1;
          dm(_ControlRX_Service_Number_of_Words) = r5;
          // update write index
	      r5 = dm(_ControlRX_Service_Write_Ind);
	      r5 = r5 - 1;
          if ge jump Ignore_Message; 
            r6 = dm(_ControlRX_Service_Buffer_Length);
            r5 = r5 + r6;
Ignore_Message: 
          dm(_ControlRX_Service_Write_Ind) = r5;
          dm(_ControlRX_Service_Busy_Flag) = m5;
          // return
          rts;
Test_For_Control_Messages:      
      r6 = 3;         // index of Message Size, starting with 0
      r6 = r5 - r6;
      if ne jump Test_for_end_word;
        r6 = 5;       // index of Message END, staring with 0 
        r6 = r4 + r6; // r4 = SIZE of User Data
        dm(RX_Message_End_Index) = r6;
        jump Store_Message_Word;
Test_for_end_word:
      if lt jump Store_Message_Word;
      r6 = dm(RX_Message_End_Index);
      r6 = r6 - r5;
      if ne jump Store_Message_Word;
      r5 = TDM_SERVICE_END_CHAR;
      r5 = r4 xor r5;
      if ne jump Corrupted_Message;
        dm(_ControlRX_Service_Message_Ready) = m6;
        dm(_ControlRX_Service_Busy_Flag) = m5;
        r5 = dm(_ControlRX_Service_Number_of_Messages);
        r5 = r5 + 1;
        dm(_ControlRX_Service_Number_of_Messages) = r5; 
        jump Store_Message_Word;
Corrupted_Message:
      // update number of received chars.
      r6 = dm(_ControlRX_Service_Number_of_Words);
      r5 = dm(RX_Message_Index);
      r6 = r6 - r5;
      dm(_ControlRX_Service_Number_of_Words) = r6;
      // update buffer write index
	  r6 = dm(_ControlRX_Service_Write_Ind);
	  r6 = r6 - r5;
      if ge jump Corrupted_Message_Flag; 
        r5 = dm(_ControlRX_Service_Buffer_Length);
        r6 = r6 + r5;
Corrupted_Message_Flag: 
      dm(_ControlRX_Service_Write_Ind) = r6;
      dm(_ControlRX_Service_Corrupted_Message) = m6;         
      dm(_ControlRX_Service_Busy_Flag) = m5;
      rts;
Store_Message_Word:
      r5 = dm(RX_Message_Index);
      r5 = r5 + 1;
      dm(RX_Message_Index) = r5;                  
      r5 = dm(_ControlRX_Service_Number_of_Words);
      r6 = dm(_ControlRX_Service_Buffer_Length);
      r6 = r6 - r5;
      if gt jump RX_Buffer_OK;
	    dm(_ControlRX_Service_Buffer_Overrun)=m6;
	    rts;
RX_Buffer_OK:
	  r5 = dm(_ControlRX_Service_Write_Ind);	  
      i5 = dm(_ControlRX_Service_Buffer_Ptr);
	  dm(_Master_DSP_SD) = r4;     // store Frame_ID
	  m4 = r5;	  
	  r5 = r5 + 1, dm(m4,i5) = r4;
	  r4 = dm(_ControlRX_Service_Buffer_Length);
	  r4 = r4 - r5;
	  if eq r5 = r5 - r5;
	  dm(_ControlRX_Service_Write_Ind) = r5;
      r5 = dm(_ControlRX_Service_Number_of_Words);
      r5 = r5 + 1;
      dm(_ControlRX_Service_Number_of_Words) = r5;
	  
	  dm(_TDM_DTR_Flag) = m6;      // set data ready flag
      rts;
//===========================================================	  
          
_OFDM_TDM_Rec_ISR.end:

