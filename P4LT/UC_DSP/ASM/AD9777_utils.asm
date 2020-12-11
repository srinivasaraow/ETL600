/*-------------------------------------------------------------------------*
 * $Workfile: AD9777_utils.asm $
 * Part of      : ETL600 / P4LT
 * Language     : asm
 * Created by   : Vladimir Potapov (vgp)
 * Remarks      :
 * Purpose      : AD9777 serial port programming through SPI port on FPGA
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/UC_DSP/ASM/AD9777_utils.asm $
 * 
 * 1     20.01.05 8:32 Chchlee
 * 
 * 4     5.07.04 10:20 Bussli02
 * Reduced SPICLK to 6,144 Mhz
 * 
 * 3     27.05.04 14:39 Maag01
 * AD9777_ADR defined as FLS0  (FLAG0 slave device select enable)
 * 
 * 2     26.05.04 16:38 Maag01
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/
	
#include 	<def21161.h>
#include    <asm_sprt.h>

.segment/pm   seg_pmco;

.global _AD9777_Write;   
.global _AD9777_Read;  

#define AD9777_ADR	FLS0	  // FLAG0 slave device select enable  

//=========================================================
// Prototype: int AD9777_Write(address, data);
// Usage: status = AD9777_Write(address, data);
//=========================================================

_AD9777_Write:  
   leaf_entry;
   
   // extract 8 LSB from the address word
   // bit7 - R/W
   // bit6 - I6n
   // bit5 - I5n
   // bit4 - I4
   // bit3 - I3
   // bit2 - I2
   // bit1 - I1
   // bit0 - I0

   r0 = 0xff;
   r4 = r4 and r0;
   r4 = lshift r4 by 8;
      
   // extract 8 LSB from the data word
   r0 = 0xff;
   r8 = r8 and r0;
   
   // form write SPI data transfer frame
   r4 = r4 or r8;

   // reset SPI
   dm(SPICTL) = m5;           // clear SPI control register
   
   // deposit SPI control word
   ustat1 = dm(SPICTL);       // ustat1 = 0
   // SPRINT is set to avoid RBS error (maybe error in silicon)
   // BAURD1 -> serial clock F_core/16 = 6.144 MHz. anomaly 20
   bit set ustat1 SPIEN | SPRINT | MS | DF | WL16 | BAUDR2 | PSSE | NSMLS | DCPH0 | AD9777_ADR;
   
   // write to TX buffer
   dm(SPITX) = r4;            // 16 bit word to transfer
 
   // enable SPI
   dm(SPICTL) = ustat1;

   // output byte mask
   r4 = 0xff;
   
   // wait for response
wt4rx:
   ustat1 = dm(SPISTAT);
   bit tst ustat1 RXS0;
   if not TF jump wt4rx;

   // get SPI word
   r0 = dm(SPIRX);

   // reset SPI
   dm(SPICTL) = m5;           // clear SPI control register

   // mask output byte      
   r0 = r0 and r4;
            
   leaf_exit;
_AD9777_Write.end:  

//=========================================================
// Prototype: int AD9777_Read(address);
// usage:      data = AD9777_Read(address);
//=========================================================

_AD9777_Read:
   leaf_entry;

   // extract 8 LSB from the word
   r0 = 0xff;
   r4 = r4 and r0;
   r4 = bset r4 by 7;        

   // form write SPI data transfer frame with 0 data field
   r4 = lshift r4 by 8;

   // reset SPI
   dm(SPICTL) = m5;           // clear SPI control register
   
   // deposit SPI control word
   ustat1 = dm(SPICTL);       // ustat1 = 0
   // SPRINT is set to avoid RBS error (maybe error in silicon)
   // BAURD1 -> serial clock F_core/16 = 6.144 MHz
   bit set ustat1 SPIEN | SPRINT | MS | DF | WL16 | BAUDR2 | PSSE | NSMLS | DCPH0 | AD9777_ADR;
   
   // write to TX buffer
   dm(SPITX) = r4;            // 16 bit word to transfer
   
   // enable SPI
   dm(SPICTL) = ustat1;
   
   // output byte mask
   r4 = 0xff;
   
   // wait for response
wt4rx1:
   ustat1 = dm(SPISTAT);
   bit tst ustat1 RXS0;
   if not TF jump wt4rx1;

   // get received word
   r0 = dm(SPIRX);
      
   // reset SPI
   dm(SPICTL) = m5;           // clear SPI control register

   // mask output byte
   r0 = r0 and r4;
      
   leaf_exit;
_AD9777_Read.end:


.endseg;
