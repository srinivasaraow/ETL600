/*-------------------------------------------------------------------------*
 * $Workfile: AD6620_utils.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Dr. Vladimir Potapov
 * Remarks      :
 * Purpose      : AD6620 programming through Micro port interface 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/ASM/AD6620_utils.asm $
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 2     7.06.04 13:09 Maag01
 * 
 * 1     28.05.04 16:37 Maag01
 * Initial Version
 * 
 *-------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//  
// HISTORY:    16.10.03  Vladimir Potapov (vgp), Initial Version
//             21.11.03 version for AD6620 32MHz system clock, additional nop;
//
//---------------------------------------------------------------------------
	
#include <def21161.h>
#include <asm_sprt.h>

#include "ad6620.h"


#define REG_MASK     ~0x0000030f
#define REG_AMR_MASK  0x00000300
#define REG_MAX_ADR   0x30d

.segment/pm   seg_pmco;

.extern _Get_DSP_ID;

.global _AD6620_Reset;

.global _AD6620_8Bit_Reg_Write;
.global _AD6620_16Bit_Reg_Write;
.global _AD6620_32Bit_Reg_Write;

.global _AD6620_8Bit_Reg_Read;
.global _AD6620_16Bit_Reg_Read;
.global _AD6620_32Bit_Reg_Read;

.global _AD6620_20Bit_Reg_Write;
.global _AD6620_20Bit_Reg_Read;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// for 32MHz System clock 
//  LAR setting needs delay of 3 instructions of ADSP-21161 as minimum
//  before read or write registers
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//=========================================================
//
// void	AD6620_Reset();
//

_AD6620_Reset:
   leaf_entry;
nop;
   leaf_exit;
_AD6620_Reset.end:

//=========================================================
Test_and_Write_Reg_Addr:
   
   r0 = 1;
   r12 = REG_MASK;
   r12 = r12 and r4;
   if ne rts;
   r0  = REG_AMR_MASK;
   r12 = r0 and r4;
   r12 = r12 xor r0;
   if ne rts;
   r12 = REG_MAX_ADR;
   r12 = r12 - r4;
   if lt rts;
   // write A9=1 & A8=1 of address register
   r0 = 0x3;            
   dm(AD6620_AMR) = r0;   
   // extract from reg. address 4 LSB
   r0 = 0xf;
   r4 = r4 and r0;
   nop;
   dm(AD6620_LAR) = r4;
   r0 = r0 - r0;
   rts;

Reg_Addr_Er_Exit:
   r0 = -1;
   leaf_exit;
      
Test_and_Write_Reg_Addr.end:
//=========================================================
//
// int   AD6620_8Bit_Reg_Write(int reg_address, int reg_data);
//

_AD6620_8Bit_Reg_Write:
   leaf_entry;

   call Test_and_Write_Reg_Addr;
   r0 = pass r0;
   if ne jump Reg_Addr_Er_Exit;
   
   r4 = ~0xff;
   r4 = r4 and r8;
   if ne jump Reg_Addr_Er_Exit;
   dm(AD6620_DR0) = r8;
   
   leaf_exit;
   
_AD6620_8Bit_Reg_Write.end:

//=========================================================
//
// int   AD6620_16Bit_Reg_Write(int reg_address, int reg_data);
//

_AD6620_16Bit_Reg_Write:
   leaf_entry;

   call Test_and_Write_Reg_Addr;
   r0 = pass r0;
   if ne jump Reg_Addr_Er_Exit;

   r4 = ~0xffff;
   r4 = r4 and r8;
   if ne jump Reg_Addr_Er_Exit;
   r4 = fext r8 by 8:8;
   dm(AD6620_DR1) = r4;    // write least significant byte
   r4 = fext r8 by 0:8;
   nop;
   nop;
   dm(AD6620_DR0) = r4;    // write least significant byte
   nop;
         
   leaf_exit;
_AD6620_16Bit_Reg_Write.end:

//=========================================================
//
// int   AD6620_32Bit_Reg_Write(int reg_address, int reg_data);
//

_AD6620_32Bit_Reg_Write:
   leaf_entry;

   call Test_and_Write_Reg_Addr;
   r0 = pass r0;
   if ne jump Reg_Addr_Er_Exit;

   r4 = fext r8 by 24:8;
   dm(AD6620_DR3) = r4;    // write most significant byte
   r4 = fext r8 by 16:8;
   nop;
   nop;
   dm(AD6620_DR2) = r4;    // write third byte
   r4 = fext r8 by 8:8;
   nop;
   nop;
   dm(AD6620_DR1) = r4;    // write second byte
   r4 = fext r8 by 0:8;
   nop;
   nop;
   dm(AD6620_DR0) = r4;    // write least significant byte
   nop;
   nop;
   
   leaf_exit;
_AD6620_32Bit_Reg_Write.end:

//=========================================================
//
// int   AD6620_8Bit_Reg_Read(int reg_address);
//

_AD6620_8Bit_Reg_Read:
   leaf_entry;

   call Test_and_Write_Reg_Addr;
   r0 = pass r0;
   if ne jump Reg_Addr_Er_Exit;

   r0 = dm(AD6620_DR0);
   r0 = fdep r0 by 0:8;
   
   leaf_exit;
_AD6620_8Bit_Reg_Read.end:

//=========================================================
//
// int   AD6620_16Bit_Reg_Read(int reg_address);
//

_AD6620_16Bit_Reg_Read:
   leaf_entry;

   call Test_and_Write_Reg_Addr;
   r0 = pass r0;
   if ne jump Reg_Addr_Er_Exit;

   r4 = dm(AD6620_DR0);
   r0 = fdep r4 by 0:8;
   r4 = dm(AD6620_DR1);
   r0 = r0 or fdep r4 by 8:8;

   
   leaf_exit;
_AD6620_16Bit_Reg_Read.end:

//=========================================================
//
// int   AD6620_32Bit_Reg_Read(int reg_address);
//

_AD6620_32Bit_Reg_Read:
   leaf_entry;

   call Test_and_Write_Reg_Addr;
   r0 = pass r0;
   if ne jump Reg_Addr_Er_Exit;

   r4 = dm(AD6620_DR0);
   r0 = fdep r4 by 0:8;
   r4 = dm(AD6620_DR1);
   r0 = r0 or fdep r4 by 8:8;
   r4 = dm(AD6620_DR2);
   r0 = r0 or fdep r4 by 16:8;   
   r4 = dm(AD6620_DR3);
   r0 = r0 or fdep r4 by 24:8;
   
   leaf_exit;
_AD6620_32Bit_Reg_Read.end:

//=========================================================
//
// int   AD6620_20Bit_Reg_Write(int reg_offset, int *reg_data, int len);
//

_AD6620_20Bit_Reg_Write:
   leaf_entry;

   puts = r1;
   puts = r2;
   puts = r3;
   r0 = i4; puts = r0;   

   r0 = ~0xff;
   r0 = r0 and r4;
   if ne jump Err_Exit;
   r0 = 0xfffffe00;
   r0 = r0 and r12;
   if ne jump Err_Exit;
   r0 = r4 + r12;
   r1 = 0x100;
   r0 = r1 - r0;
   if lt jump Err_Exit;
   r0 = 0x80;            // set write autoincrement mode
   dm(AD6620_AMR) = r0;
   i4 = r8;              // i4 poits vector of RCF coeff. in dm RAM
   r2 = ~0x0fffff;       // 20 bit mask
   nop;
   dm(AD6620_LAR) = r4;  // deposit start address for coefficients
   r3 = r3 - r3;         // error flag
   
   lcntr = r12, do write_lp until lce;
          r0 = dm(i4,m6);  // get coeff.
          r1 = abs r0;
          r1 = r1 and r2;
          if ne r3 = r3 + 1;
          r1 = fext r0 by 16:4;
          dm(AD6620_DR2) = r1;    // write most significant byte [23:16]                    
          r1 = fext r0 by 8:8;
		  nop;
		  nop;
          dm(AD6620_DR1) = r1;    // write second byte [15:8]          
          r1 = fext r0 by 0:8;
		  nop;
		  nop;
write_lp: dm(AD6620_DR0) = r1;    // write least significant byte [7:0]

   r3 = pass r3;
   if ne jump Err_Exit;      
   
   r0 = r0 - r0;
   dm(AD6620_AMR) = r0;           // reset autoincrement write mode

   r1 = gets( 4 );   
   r2 = gets( 3 );   
   r3 = gets( 2 );   
   i4 = gets( 1 );
   leaf_exit;

Err_Exit:
   r0 = -1;
   r1 = gets( 4 );   
   r2 = gets( 3 );   
   r3 = gets( 2 );   
   i4 = gets( 1 );
   leaf_exit;
   
_AD6620_20Bit_Reg_Write.end:

//=========================================================
//
// int   AD6620_20Bit_Reg_Read(int reg_offset, int *reg_data, int len);
//

_AD6620_20Bit_Reg_Read:
   leaf_entry;

   puts = r1;
   puts = r2;
   puts = r3;
   r0 = i4; puts = r0;   

   r0 = ~0xff;
   r0 = r0 and r4;
   if ne jump Err_Exit;
   r0 = 0xfffffe00;
   r0 = r0 and r12;
   if ne jump Err_Exit;
   r0 = r4 + r12;
   r1 = 0x100;
   r0 = r1 - r0;
   if lt jump Err_Exit;
   r0 = 0x40;            // set read autoincrement mode
   dm(AD6620_AMR) = r0;
   i4 = r8;              // i4 poits vector of RCF coeff. in dm RAM
   nop;
   nop;
   dm(AD6620_LAR) = r4;  // needs 2 commands!!! deposit start address of coefficients
   r3 = 0xfff00000;      // mask for negative numbers
      
   lcntr = r12, do read_lp until lce;
          r8 = dm(AD6620_DR0);    // read least significant byte [23:16]
          r0 = fdep r8 by 0:8;
          r8 = dm(AD6620_DR1);    // read second byte [15:8]
          r0 = r0 or fdep r8 by 8:8; 
          r8 = dm(AD6620_DR2);    // read most significant byte [7:0]                    
          r0 = r0 or fdep r8 by 16:4;
          btst r0 by 19;
          if not sz r0 = r0 or r3;
read_lp: r4 = r4 + 1, dm(i4,m6) = r0;   // increment AD6620 address
   
   r0 = r0 - r0;
   dm(AD6620_AMR) = r0;           // rset autoincrement read mode

   r1 = gets( 4 );   
   r2 = gets( 3 );   
   r3 = gets( 2 );   
   i4 = gets( 1 );
   leaf_exit;

_AD6620_20Bit_Reg_Read.end:

//=========================================================

.endseg;

