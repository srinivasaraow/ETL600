/*
$Log: /Source/P4LT Prototype 2/OFDM_DSP/ASM/DCTCXO/i2c_driver.asm $
 * 
 * 1     19.01.05 15:36 Chchlee
 * 
 * 3     2.12.04 17:38 Chhamaa
 * 
 * 2     30.09.04 8:27 Leeb02
*/

#include <asm_sprt.h>
#include <def21161.h>

#include "../../H/DCTCXO/AD5321_defs.h"

.extern _I2C_WR_Index_Tx;
.extern _I2C_RD_Index_Tx;
.extern _AD5321_TX_Buffer;
.extern _I2C_TSlotCount;
.extern _I2C_Bit_Mask;
.extern _I2C_Busy;
.extern _I2C_RD_Word;
.extern _I2C_NACK;
.extern _I2C_Tm_Counter;
.extern _I2C_Tm_Disable;
.extern TMZHI_Mode1_Save;
.extern TMZHI_Astat_Save;

.global _Timer_Enable;
.global _Timer_Disable;
.global _Timer_Set;
.global _AD5321_Delay;
.global _AD5321_Wait;
.global AD5321_WR_Server;

.section /dm seg_dmda;
.var save_r0;
.var save_r1;
.var save_r2;
.var save_r4;
.var save_i1;
.var save_m1;
.var save_m5;
.var save_m6;
.var save_ustat1;

.section /pm seg_pmco;

//=============================================================================== 
_Timer_Set:
  leaf_entry;
     TPERIOD = r4;
     TCOUNT = r4;
  leaf_exit;
_Timer_Set.end:  
//=============================================================================== 		
_Timer_Enable:
  leaf_entry; 
  bit set mode2 TIMEN;
  leaf_exit;
_Timer_Enable.end:
//=============================================================================== 		
_Timer_Disable:
  leaf_entry; 
  bit clr mode2 TIMEN;
  leaf_exit;
_Timer_Disable.end:
//=============================================================================== 
_AD5321_Delay:
  leaf_entry;
  lcntr = r4, do AD5321_dly until lce;
AD5321_dly: nop;
  leaf_exit;
_AD5321_Delay.end:
//=============================================================================== 
_AD5321_Wait:
  leaf_entry;
  puts = r0;
AD5321_tst:
  r0 = dm(_I2C_Busy);
  r0 = pass r0;
  if ne jump AD5321_tst;
  r0 = gets(1);
  leaf_exit;
_AD5321_Wait.end:
//=============================================================================== 		
Timer_Disable:
  bit clr mode2 TIMEN;
  rts;
//=============================================================================== 
I2C_Set_SCL_DirOut:
  ustat1 = dm(IOFLAG);
  rts(db);
    bit set ustat1 AD5321_SCL_DIR_CONTROL;  // set SCL for output
    dm(IOFLAG) = ustat1;
//=============================================================================== 
I2C_Set_SDA_DirOut:
  ustat1 = dm(IOFLAG);
  rts(db);
    bit set ustat1 AD5321_SDA_DIR_CONTROL;  // set SDA for output
    dm(IOFLAG) = ustat1;
//=============================================================================== 
I2C_Set_SDA_DirIn:
  ustat1 = dm(IOFLAG);
  rts(db);
    bit clr ustat1 AD5321_SDA_DIR_CONTROL;  // set SDA for input
    dm(IOFLAG) = ustat1;
//=============================================================================== 
I2C_Set_SDA_Lo:
  ustat1 = dm(IOFLAG);
  rts(db);
    bit clr ustat1 AD5321_SDA_VAL_CONTROL;  // set SDA line Low
    dm(IOFLAG) = ustat1;
//=============================================================================== 
I2C_Set_SDA_Hi:
  ustat1 = dm(IOFLAG);
  rts(db);
    bit set ustat1 AD5321_SDA_VAL_CONTROL;  // set SDA line High
    dm(IOFLAG) = ustat1;
//=============================================================================== 
I2C_Read_SDA:
/*
  ustat1 = dm(IOFLAG);
  r0 = r0 - r0;
  bit tst ustat1 AD5321_SDA_VAL_CONTROL;
  if ne r0 = r0 + 1;
*/
  r0 = dm(IOFLAG);
  r1 = AD5321_SDA_VAL_CONTROL;
  r0 = r0 and r1;
  if eq rts;
  r0 = m6;
  rts;
//=============================================================================== 
I2C_Set_SCL_Lo:
  ustat1 = dm(IOFLAG);
  rts(db);
    bit clr ustat1 AD5321_SCL_VAL_CONTROL;  // set SCL line Low
    dm(IOFLAG) = ustat1;
//=============================================================================== 
I2C_Set_SCL_Hi:
  ustat1 = dm(IOFLAG);
  rts (db);
    bit set ustat1 AD5321_SCL_VAL_CONTROL;  // set SCL line High
    dm(IOFLAG) = ustat1;
//=============================================================================== 
//
// Function initiates start condition of I2C port
//
// sampling is eq to 4 or pi/2
//
//  globals: I2C_TSlotCount
//           I2C_Bit_Mask
//           I2C_Busy
//
I2C_Start:

  call I2C_Set_SDA_DirOut;    // Set SDA for output

  r0 = dm(_I2C_TSlotCount);
  r0 = pass r0;
  if ne jump I2C_Start_nz;
//    dm(_I2C_Busy) = m6;       //TimeSlot=0, set buzy flag
	call I2C_Set_SCL_Hi;      //Set serial clock to Hi
	call I2C_Set_SDA_Hi;      //Set serial data to Hi
	dm(_I2C_TSlotCount) = m6; //Next time slot #1
	rts;
I2C_Start_nz:
  r0 = r0-1;
  if ne jump I2C_Start_2;
	call I2C_Set_SCL_Hi;      //TimeSlot=1, set serial clock to Hi 
	call I2C_Set_SDA_Lo;      //Set serial data to Lo
	r0 = 2;
	dm(_I2C_TSlotCount) = r0; //Next time slot #2
	rts;
I2C_Start_2:
	call I2C_Set_SCL_Lo;      //TimeSlot=2, set serial clock to Lo 
	call I2C_Set_SDA_Lo;      //Set serial data to Lo
	dm(_I2C_TSlotCount) = m5; //Next time slot #0
	dm(_I2C_RD_Word) = m5;    // I2C_RD_WOrd = 0
	r0 = 0x80;
	dm(_I2C_Bit_Mask) = r0;   // mask out MSB bit
    rts;
//=============================================================================== 
//
// Function initiates stop condition of I2C port
//
// sampling is eq to 4 or pi/2
//
//  globals: I2C_TSlotCount
//           I2C_Busy
//
I2C_Stop:

  call I2C_Set_SDA_DirOut;    // Set SDA for output

  r0 = dm(_I2C_TSlotCount);
  r0 = pass r0;
  if ne jump I2C_Stop_nz;
	call I2C_Set_SCL_Lo;      //TimeSlot=0, set serial clock to Low
	call I2C_Set_SDA_Lo;      //Set serial data to Low
	dm(_I2C_TSlotCount) = m6; //Next time slot #1
	rts;
I2C_Stop_nz:
  r0 = r0-1;
  if ne jump I2C_Stop_2;
	call I2C_Set_SCL_Hi;      //TimeSlot=1, set serial clock to High
	call I2C_Set_SDA_Lo;      //Set serial data to Lo
	r0 = 2;
	dm(_I2C_TSlotCount) = r0; //Next time slot #2
	rts;
I2C_Stop_2:
	call I2C_Set_SCL_Hi;      //TimeSlot=2, set serial clock to High
	call I2C_Set_SDA_Hi;      //Set serial data to High
	dm(_I2C_TSlotCount) = m5; //Next time slot #0
//    dm(_I2C_Busy) = m5;       //clear buzy flag
    rts;

//=============================================================================== 
//
// Function writes one bit via I2C port
//
// sampling is equal to 4 or pi/2
//
//  globals: I2C_TSlotCount
//           I2C_Bit_Mask
//
//  input: register r4
//
I2C_Bit_Write:
    
  call I2C_Set_SDA_DirOut;    // Set SDA for output

  r0 = dm(_I2C_TSlotCount);
  r0 = pass r0;
  if ne jump I2C_Bit_Write_nz;
	call I2C_Set_SCL_Lo;      //TimeSlot=0, set serial clock to Low
	r4 = pass r4;
	if ne jump WR_Bit1;
	  call I2C_Set_SDA_Lo;      // set bit to zero
	  dm(_I2C_TSlotCount) = m6; //Next time slot #1
	  rts;
WR_Bit1:
	  call I2C_Set_SDA_Hi;      //set bit to one
	  dm(_I2C_TSlotCount) = m6; //Next time slot #1
	  rts;
I2C_Bit_Write_nz:
  r0 = r0 - 1;
  if ne jump I2C_Bit_Write_23;
      call I2C_Set_SCL_Hi;     // TimeSlot=1, set serial clock High
	  r0 = 2;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #2
	  rts;
I2C_Bit_Write_23:
  r0 = r0 - 1;
  if ne jump I2C_Bit_Write_3;
      call I2C_Set_SCL_Hi;     // TimeSlot=2, set serial clock High
	  r0 = 3;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #3
	  rts;
I2C_Bit_Write_3:
      call I2C_Set_SCL_Lo;     // TimeSlot=3, set serial clock Low
	  dm(_I2C_TSlotCount) = m5; //Next time slot #0
      r0 = dm(_I2C_Bit_Mask);
	  r0 = lshift r0 by -1;
	  dm(_I2C_Bit_Mask) = r0;
	  r0 = pass r0;
	  if ne rts;
	  r0 = 0x80;
	  dm(_I2C_Bit_Mask) = r0;
	  rts;

//=============================================================================== 
//
// Function reads one bit via I2C port
//
// sampling is eq to 4 or pi/2
//
//  globals: I2C_TSlotCount
//           I2C_Bit_Mask
//           I2C_RD_Word
//
// used reg: r0, r4
//
I2C_Bit_Read:
    
  call I2C_Set_SDA_DirIn;     // Set SDA for input

  r0 = dm(_I2C_TSlotCount);
  r0 = pass r0;
  if ne jump I2C_Bit_Read_nz;
	call I2C_Set_SCL_Lo;      //TimeSlot=0, set serial clock to Low
	dm(_I2C_TSlotCount) = m6; //Next time slot #1
	rts;
I2C_Bit_Read_nz:
  r0 = r0 - 1;
  if ne jump I2C_Bit_Read_23;
      call I2C_Set_SCL_Hi;     // TimeSlot=1, set serial clock High
	  r0 = 2;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #2
	  rts;
I2C_Bit_Read_23:
  r0 = r0 - 1;
  if ne jump I2C_Bit_Read_3;
      call I2C_Set_SCL_Hi;     // TimeSlot=2, set serial clock High
	  call I2C_Read_SDA;
	  r0 = pass r0;
	  if eq jump I2C_Bit0_Read;
	    r0 = dm(_I2C_RD_Word);
		r4 = dm(_I2C_Bit_Mask);
		r0 = r0 or r4;
		dm(_I2C_RD_Word) = r0;
I2C_Bit0_Read:
	  r0 = 3;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #3
	  rts;
I2C_Bit_Read_3:
      call I2C_Set_SCL_Lo;     // TimeSlot=3, set serial clock Low
	  dm(_I2C_TSlotCount) = m5; //Next time slot #0
      r0 = dm(_I2C_Bit_Mask);
	  r0 = lshift r0 by -1;
	  dm(_I2C_Bit_Mask) = r0;
	  r0 = pass r0;
	  if ne rts;
	  r0 = 0x80;
	  dm(_I2C_Bit_Mask) = r0;
	  rts;

//=============================================================================== 
//  globals: I2C_NACK - if 0 then acknoledged 

I2C_Read_ACK:
    
  call I2C_Set_SDA_DirIn;    // Set SDA for input

  r0 = dm(_I2C_TSlotCount);
  r0 = pass r0;
  if ne jump I2C_Read_ACK_nz;
	call I2C_Set_SCL_Lo;      //TimeSlot=0, set serial clock to Low
	dm(_I2C_TSlotCount) = m6; //Next time slot #1
	rts;
I2C_Read_ACK_nz:
  r0 = r0 - 1;
  if ne jump I2C_Read_ACK_23;
      call I2C_Set_SCL_Hi;     // TimeSlot=1, set serial clock High
	  r0 = 2;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #2
	  rts;
I2C_Read_ACK_23:
  r0 = r0 - 1;
  if ne jump I2C_Read_ACK_3;
      call I2C_Set_SCL_Hi;     // TimeSlot=2, set serial clock High
	  r0 = 3;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #3
	  rts;
I2C_Read_ACK_3:
      call I2C_Set_SCL_Lo;      // TimeSlot=3, set serial clock Low
      call I2C_Read_SDA;
      dm(_I2C_NACK)=r0;
	  dm(_I2C_TSlotCount) = m5; //Next time slot #0
	  r0 = 0x80;
	  dm(_I2C_Bit_Mask) = r0;
	  rts;

//=============================================================================== 
I2C_Write_ACK:
    
  call I2C_Set_SDA_DirOut;    // Set SDA for output
  call I2C_Set_SDA_Lo;

  r0 = dm(_I2C_TSlotCount);
  r0 = pass r0;
  if ne jump I2C_Write_ACK_nz;
	call I2C_Set_SCL_Lo;      //TimeSlot=0, set serial clock to Low
	dm(_I2C_TSlotCount) = m6; //Next time slot #1
	rts;
I2C_Write_ACK_nz:
  r0 = r0 - 1;
  if ne jump I2C_Write_ACK_23;
      call I2C_Set_SCL_Hi;     // TimeSlot=1, set serial clock High
	  r0 = 2;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #2
	  rts;
I2C_Write_ACK_23:
  r0 = r0 - 1;
  if ne jump I2C_Write_ACK_3;
      call I2C_Set_SCL_Hi;     // TimeSlot=2, set serial clock High
	  r0 = 3;                  
	  dm(_I2C_TSlotCount) = r0; //Next time slot #3
	  rts;
I2C_Write_ACK_3:
      call I2C_Set_SCL_Lo;     // TimeSlot=3, set serial clock Low
	  dm(_I2C_TSlotCount) = m5; //Next time slot #0
	  r0 = 0x80;
	  dm(_I2C_Bit_Mask) = r0;
	  rts;

//=============================================================================

AD5321_WR_Server:            
      dm(TMZHI_Astat_Save) = astat;
      dm(save_r0) = r0;
      dm(save_r1) = r1;
      dm(save_r2) = r2;
      dm(save_r4) = r4;
      dm(save_i1) = i1;
      dm(save_m1) = m1;
      dm(save_m5) = m5;
      dm(save_m6) = m6;
      dm(save_ustat1) = ustat1;
	  m5 = 0;
	  m6 = 1;
	  
      r0 = dm(_I2C_WR_Index_Tx);
      r1 = dm(_I2C_RD_Index_Tx);
	  r0 = r0 xor r1;
	  if eq jump AD5321_Nothing_2do;
        call I2C_Set_SCL_DirOut;	    
        i1 = _AD5321_TX_Buffer;
		m1 = dm(_I2C_RD_Index_Tx);
		r1 = dm(m1,i1);            // r1 = descriptor + data
		r0 = AD5321_DESCRIPTOR;
		r0 = r0 and r1;            // r0 = descriptor
        r2 = AD5321_WR_START;
		r2 = r0 xor r2;
		if ne jump AD5321_Chk_WR_Byte;
          call I2C_Start;               // write I2C start sequence
		  jump AD5321_WR_Server_Epil;		                         

AD5321_Chk_WR_Byte:
        r2 = AD5321_WR_BYTE;
		r2 = r0 xor r2;
		if ne jump AD5321_Chk_WR_Stop;
		  r4 = r4 - r4;
          r0 = dm(_I2C_Bit_Mask);
		  r0 = r0 and r1;
		  if ne r4 = r4 + 1;
		  call I2C_Bit_Write;
		  jump AD5321_WR_Server_Epil;		                         

AD5321_Chk_WR_Stop:
        r2 = AD5321_WR_STOP;
		r2 = r0 xor r2;
		if ne jump AD5321_Chk_RD_ack;
          call I2C_Stop;               // write I2C stop sequence
		  jump AD5321_WR_Server_Epil;		                         

AD5321_Chk_RD_ack:
        r2 = AD5321_RD_ACK;
		r2 = r0 xor r2;
		if ne jump AD5321_Chk_WR_ack;
          call I2C_Read_ACK;           // read acknoledge
		  jump AD5321_WR_Server_Epil;		                         

AD5321_Chk_WR_ack:
        r2 = AD5321_WR_ACK;
		r2 = r0 xor r2;
		if ne jump xxx;
          call I2C_Write_ACK;           // write acknoledge

AD5321_WR_Server_Epil:
        r0 = dm(_I2C_TSlotCount);
		r0 = pass r0;
		if ne jump xxx;
        r0 = dm(_I2C_Bit_Mask);
		r1 = 0x80;
		r0 = r0 xor r1;
		if ne jump xxx;
		r0 = dm(_I2C_RD_Index_Tx);
		r0 = r0 + 1;
		dm(_I2C_RD_Index_Tx) = r0;
		r1 = AD5321_WR_BUF_LENGTH;
		r0 = r0 xor r1;
        if ne jump xxx;
        dm(_I2C_RD_Index_Tx) = m5;
        r0 = dm(save_r0);
        r1 = dm(save_r1);
        r2 = dm(save_r2);
        r4 = dm(save_r4);
        i1 = dm(save_i1);
        m1 = dm(save_m1);
        m5 = dm(save_m5);
        m6 = dm(save_m6);
        ustat1 = dm(save_ustat1);
        astat = dm(TMZHI_Astat_Save);
        mode1 = dm(TMZHI_Mode1_Save);
        rti;

AD5321_Nothing_2do:
      r0 = dm(_I2C_Busy);
	  r0 = pass r0;
      if ne jump xxx;
	  call Timer_Disable;
      dm(_I2C_Tm_Disable) = m6;
xxx1:      
      dm(_I2C_Busy) = m5;
xxx:      
      r0 = dm(save_r0);
      r1 = dm(save_r1);
      r2 = dm(save_r2);
      r4 = dm(save_r4);
      i1 = dm(save_i1);
      m1 = dm(save_m1);
      m5 = dm(save_m5);
      m6 = dm(save_m6);
      ustat1 = dm(save_ustat1);
      astat = dm(TMZHI_Astat_Save);
      mode1 = dm(TMZHI_Mode1_Save);
      rti;
AD5321_WR_Server.end:      
_AD5321_WR_Server.end:

//==============================================================================


