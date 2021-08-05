#define DEBUG_V11
// modified by vgp to use with SPORT13 in TDM mode
// SPORT13 interrupt handlers are added 04.09.2003

/* This file contains the interrupt table for the ADSP-21160		*/
/* When the C program exits either by returning from main() or by an 	*/
/* explicit or implicit call to exit(), control will transfer to the 	*/
/* label ___lib_prog_term.  Currently, the ___lib_prog_term label is 	*/
/* defined at the end of the reset vector as an IDLE instruction.    	*/
/* If your application needs to perform some operation AFTER the C   	*/
/* program has finished executing, remove the ___lib_prog_term label 	*/
/* from the runtime header, and place it at the beginning of your    	*/
/* code.							     	*/

/**************************************************************************
Changes made January 2001.

The run-time model states that i0 can't be used in the run-time libs.
Use i2 for interrupt handling instead.

$Log: /branches/Source HSD600/P4LT/OFDM_DSP/ASM/ofdm_dsp_hdr.asm $
 * 
 * 6     5.04.06 15:28 Chhamaa
 * 
 * 6     17.03.06 14:41 Chhamaa
 * 
 * 4     7.09.05 11:24 Ats
 * Exchange from TLC
 * 
 * 3     7.02.05 14:49 Chalgyg
 * OFDM Modem Version 0.09C
 * 
 * 6    Revision of V11 variable word length packing/unpacking, vgp
 * 5    27.01.05   V11 sport varable length i/o support, vgp
 *
 * 4     2.12.04 17:38 Chhamaa
 * 
 * 3     30.09.04 8:27 Leeb02
**************************************************************************/

#define __ADSP21161__  1
#define __2116x__  1
#undef  __ADSP21020__

#include <def21161.h>
#include "../h/util/sig_glob.h"

#define	INT(irp)	\
	push sts;								/*Disable interrupts				*/	\
	JUMP ___z3_int_determiner(DB);	/*jmp to finish setting up		*/	\
	DM(I7,M7)=R0;							/*Save r0 (scratch dreg)		*/	\
	R0=SIGMASK(SIG_##irp);				/*Base of int table				*/

#define RESERVED_INTERRUPT NOP;NOP;NOP;NOP

.GLOBAL	___lib_prog_term;		/* Termination address				*/
.GLOBAL	__done_execution;
.EXTERN	___lib_setup_c;
.extern     ___lib_start;

.EXTERN  ___lib_int_table;
.EXTERN  _main;

.extern _OFDM_TDM_Rec_ISR;
.extern _OFDM_TDM_Trm_ISR;
.extern _Flag_V11_Data_Received;
.extern _Flag_V11_Data_Transmitted; 

.extern _V11_Bit_Buffer_RX;
.extern _V11_RX_Bit_Ptr_Write;
.extern _V11_RX_Bit_Ptr_Read;
.extern _V11_RX_Bit_Buffer_Length;

.extern _V11_W32_2TX_Cur;
.extern _V11_W32_2TX_Next;
.extern _V11_Bit_Buffer_TX;
.extern _V11_TX_Bit_Ptr_Read;
.extern _V11_TX_Bit_Ptr_Write;
.extern _V11_TX_Bit_Buffer_Length;
.extern _V11_Packing;
.extern _V11_Bit_Mask;
.extern _V11_TX_IsStarted;
.extern _V11_RX_IsStarted;
.extern _V11_TX_STKY;
.extern _V11_RX_STKY;

.global TMZHI_Mode1_Save;
.global TMZHI_Astat_Save;
.extern AD5321_WR_Server;

.segment/dm seg_dmda;

.align 2;
.var SP0_Mode1_Save[2];
.align 2;
.var SP2_Mode1_Save[2];
.var SP0_Astat_Save;
.var SP2_Astat_Save;
.align 2;
.var TMZHI_Mode1_Save[2];
.var TMZHI_Astat_Save;

.var _V11_Data_Received;
.var _V11_Data_Transmitted;

.var Save_R0;
.var Save_R2;
.var Save_R4;
.var Save_R6;
.var Save_B0;
.var Save_I0;
.var Save_L0;

.var Save_R1;
.var Save_R3;
.var Save_R5;
.var Save_R7;
.var Save_B1;
.var Save_I1;
.var Save_L1;

#define PEYEN_BR0_DIS ~(PEYEN | BR0)
.endseg;


#ifdef __ADI_LIBEH__
.SEGMENT/DM             .gdt;
.GLOBAL                 ___eh_gdt;
.VAR                    ___eh_gdt[1];

.SEGMENT/DM             .frt;
.GLOBAL                 ___eh_frt;
.VAR                    ___eh_frt[1];
#endif /* __ADI_LIBEH__ */



.SEGMENT/PM	    	seg_rth;				/* Runtime header segment			*/

			RESERVED_INTERRUPT;

___lib_RSTI:	 	NOP;					/* Not really executed				*/
			JUMP ___lib_start;
			NOP;
			NOP;

___lib_IICDI:           INT(IICDI) ;    /* Access to illegal IOP space */
___lib_SOVFI:		INT(SOVF);			/* status/loop/PC stack overflow	*/
___lib_TMZHI:		dm(TMZHI_Mode1_Save) = mode1;
                    bit clr mode1 PEYEN | BR0;
                    jump AD5321_WR_Server;
                    rti;
                    
___lib_VIRPTI:		INT(VIRPTI);		/* external interrupts 				*/
___lib_IRQ2I:		INT(IRQ2);
___lib_IRQ1I:		INT(IRQ1);
___lib_IRQ0I:		INT(IRQ0);
			RESERVED_INTERRUPT;
___lib_SP0I:      dm(SP0_Mode1_Save) = mode1;
                  bit clr mode1 PEYEN | BR0;
                  bit set mode1 CBUFEN;
                  jump SP0_Isr;

___lib_SP1I:      jump _OFDM_TDM_Rec_ISR;
                  rti;
                  rti;
                  rti;

___lib_SP2I:      dm(SP2_Mode1_Save) = mode1;
                  bit clr mode1 PEYEN | BR0;
                  bit set mode1 CBUFEN;
                  jump SP2_Isr;
                  
___lib_SP3I:      jump _OFDM_TDM_Trm_ISR;
                  rti;
                  rti;
                  rti;

___lib_LP0I:        INT(LP0I);      /* link port DMA channel 4 */
___lib_LP1I:        INT(LP1I);      /* link port DMA channel 5 */
___lib_SPIRI:       INT(SPIRI);      /* SPI port receive interrupt */
___lib_SPITI:       INT(SPITI);
			RESERVED_INTERRUPT;
			RESERVED_INTERRUPT;
___lib_EP0I:		INT(EP0I);			/* ext port DMA channel interrupts	*/
___lib_EP1I:		INT(EP1I);
___lib_EP2I:		INT(EP2I);
___lib_EP3I:		INT(EP3I);
___lib_LSRQ:		INT(LSRQ);			/* link service request 			*/
___lib_CB7I:		INT(CB7);			/* circular buffer #7 overflow 	*/
___lib_CB15I:		INT(CB15);			/* circular buffer #15 overflow	*/
___lib_TMZLI:		INT(TMZ);			/* low priority timer 				*/
___lib_FIXI:		INT(FIX);			/* fixed point overflow 			*/
___lib_FLTOI:		INT(FLTO);			/* floating point overflow 		*/
___lib_FLTUI:		INT(FLTU);			/* floating point underflow 		*/
___lib_FLTII:		INT(FLTI);			/* floating point invalid 			*/
___lib_SFT0I:		INT(USR0);			/* user interrupts 0..3 			*/
___lib_SFT1I:		INT(USR1);
___lib_SFT2I:		INT(USR2);
___lib_SFT3I:		INT(USR3);
			RESERVED_INTERRUPT;

___z3_int_determiner:	
			DM(I7,m7)=R1;					
			R1=I2;
			DM(I7,M7)=R1;					/* Save I2 (scratch reg)			*/
			I2=R0;
			DM(I7,m7)=I13;					/* Save I13 (scratch reg)			*/
			I13=DM(5,I2);					/* get disp to jump to				*/
			JUMP (M13, I13) (DB);		/* Jump to dispatcher 				*/
      NOP;                  /* in place of freeze cache   */
			I13=DM(2,I2);					/* rd handler addr (base+2)		*/

/* Note:  It's okay to use PM in getting the above values b'cse z3 has a */
/* linear memory. Therefore dm and pm are the same and we can use either.*/

___lib_start:		
			CALL ___lib_setup_c;			/* Setup C runtime model			*/
#ifdef MAIN_RTS
			CJUMP _main (DB);				/* Begin C program					*/
			DM(I7,M7)=R2;
			DM(I7,M7)=PC;
#else
			JUMP _main;						/* Begin C program					*/
#endif
___lib_start.end:		


.SEGMENT/PM	    	seg_rth;				/* Runtime header segment			*/
/* Setting the __done_execution flag indicates that this processor is	*/
/* finished executing, for the benefit of anyone who may be watching.	*/

___lib_prog_term:	PM(__done_execution)=PC;
			IDLE;
			JUMP ___lib_prog_term;		/* Stay put 							*/
___lib_prog_term.end:

.VAR __done_execution = 0;

.ENDSEG;

.SEGMENT/PM	    	seg_pmco;				/* Runtime header segment			*/
//=============== V11 Sport0 data receive ISR
SP0_Isr:    dm(SP0_Astat_Save) = astat;
            dm(Save_R0) = r0;
            dm(Save_R2) = r2;
            dm(Save_R4) = r4;
            dm(Save_R6) = r6;
            dm(Save_B0) = b0;
            dm(Save_I0) = i0;
            dm(Save_L0) = l0;
            
//          dm(_Flag_V11_Data_Received) = m6;	   
            dm(_V11_RX_IsStarted) =m6; //Flag for OFDM_main.c
            
#ifdef DEBUG_V11
			// check V11 buffer pointers
			r0 = dm(_V11_RX_Bit_Ptr_Read);
			r2 = dm(_V11_RX_Bit_Ptr_Write);
//			r0 = r0 - r2;
			r0 = r2 - r0;
			r2 = dm(_V11_Packing);
			r0 = r0 - r2;
			if gt jump rx_ok;
			r2 = dm(_V11_RX_Bit_Buffer_Length);
			r0 = r0 + r2;
			if gt jump rx_ok;
			dm(_V11_RX_STKY) = m6;
rx_ok:            
#endif            
            r0 = dm(RX0A);
//r0 = 0xaaaaaaaa;
                      
            b0 = _V11_Bit_Buffer_RX;
            i0 = dm(_V11_RX_Bit_Ptr_Write);
            l0 = dm(_V11_RX_Bit_Buffer_Length);
#if 0            
            modify(i0,m6);
            modify(i0,m7);
#endif            

            r4 = dm(_V11_Bit_Mask);         // number of inf. bits per V11 i/o word
            r6 = dm(_V11_Packing);          // MSB first , one bit per word extraction
V11_Unpack:			
            r2 = fext r0 by r4;
            r4 = r4 - 1, dm(i0,m6) = r2;
            r6 = r6 - 1;
            if ne jump V11_Unpack; 

            l0 = 0;
            dm(_V11_RX_Bit_Ptr_Write) = i0;
                                               
            b0 = dm(Save_B0);
            l0 = dm(Save_L0);
            i0 = dm(Save_I0);
            r6 = dm(Save_R6);
            r4 = dm(Save_R4);
            r2 = dm(Save_R2);
            r0 = dm(Save_R0);
            astat = dm(SP0_Astat_Save);
            mode1 = dm(SP0_Mode1_Save);
            nop;
            rti;
            
SP0_Isr.end:
//=============== V11 Sport2 transmit ISR
SP2_Isr:    dm(SP2_Astat_Save) = astat;
            dm(Save_R1) = r1;
            dm(Save_R3) = r3;
            dm(Save_R5) = r5;
            dm(Save_R7) = r7;
            dm(Save_B1) = b1;
            dm(Save_I1) = i1;
            dm(Save_L1) = l1;

//            dm(_Flag_V11_Data_Transmitted)=m6;	   
            dm(_V11_TX_IsStarted) =m6;	//Flag for OFDM_main.c
            
#ifdef DEBUG_V11           
			// check V11 buffer pointers
			r1 = dm(_V11_TX_Bit_Ptr_Write);
			r3 = dm(_V11_TX_Bit_Ptr_Read);
			r1 = r1 - r3;
			r3 = dm(_V11_Packing);
			r1 = r1 - r3;
			if gt jump tx_ok;
			r3 = dm(_V11_TX_Bit_Buffer_Length);
			r1 = r1 + r3;
			if gt jump tx_ok;
			dm(_V11_TX_STKY) = m6;
tx_ok:
#endif            
            l1 = 0;
            i1 = dm(_V11_W32_2TX_Cur);
            r1 = dm(i1,m5);
//r1 = 0xaaaaaaaa;
            dm(TX2A) = r1;
            
            b1 = _V11_Bit_Buffer_TX;
            i1 = dm(_V11_TX_Bit_Ptr_Read);
            l1 = dm(_V11_TX_Bit_Buffer_Length);
#if 0            
            modify(i1,m6);
            modify(i1,m7);
#endif            
            r1 = r1 - r1;

            r5 = dm(_V11_Bit_Mask);         
			r5 = r5 + 1;
            r7 = dm(_V11_Packing);
V11_Pack:
            r5 = r5 - 1, r3 = dm(i1,m6);
            r1 = r1 or fdep r3 by r5;
            r7 = r7 - 1;
            if ne jump V11_Pack;

            l1 = 0;
            dm(_V11_TX_Bit_Ptr_Read) = i1;
            i1 = dm(_V11_W32_2TX_Next);
            dm(i1,m5) = r1;
            r1 = 1;
            r3 = dm(_V11_W32_2TX_Cur);
            r3 = r3 xor r1;
            dm(_V11_W32_2TX_Cur) = r3;
            r3 = dm(_V11_W32_2TX_Next);
            r3 = r3 xor r1;
            dm(_V11_W32_2TX_Next) = r3;
            
                                    
            b1 = dm(Save_B1);
            l1 = dm(Save_L1);
            i1 = dm(Save_I1);
            r7 = dm(Save_R7);
            r5 = dm(Save_R5);
            r3 = dm(Save_R3);
            r1 = dm(Save_R1);
            astat = dm(SP2_Astat_Save);
            mode1 = dm(SP2_Mode1_Save);
            nop;
            rti;
            
SP2_Isr.end:

.ENDSEG;

#ifdef _ADI_SWFA
.SEGMENT/PM       seg_pmda;
.GLOBAL __21160_anomaly_write_location;
.VAR __21160_anomaly_write_location;   // A memory location we can write to
                                       // to flush the FIFO.
#endif
.ENDSEG;
