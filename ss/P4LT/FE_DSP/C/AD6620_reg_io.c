/*-------------------------------------------------------------------------*
 * $Workfile: AD6620_reg_io.c $
 * Part of      : ETL600 / P4LT
 * Language     : C
 * Created by   : Dr. Vladimir Potapov
 * Remarks      :  
 * Purpose      : AD6620 programming through Micro port interface
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/FE_DSP/C/AD6620_reg_io.c $ 
 * 
 * 2     2.06.05 13:29 Chbrbus
 * made new init function to reset AD6620 and ADS1605. Loading of AD6620
 * registers ans resetting is now in 2 functions. They messed up the
 * upconverter FS timing restrictions.
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 4     9.12.04 11:29 Chhamaa
 * 
 * 4     19.11.04 21:17 Potapov
 * 
 * 3     18.06.04 13:38 Maag01
 * 
 * 2     3.06.04 15:53 Maag01
 * 
 * 1     28.05.04 13:29 Maag01
 * Initial version
 * 
 * 
 *-------------------------------------------------------------------------*/


/* INCLUDE FILES **************************************************************/


#include <def21161.h>
#include <math.h>

// AD6620 Definitions
#include "AD6620.h"
#include "AD6620_utils.h"

//=========================================================


#define CLK_FREQ_AD6620_HZ	32768000.0		// clock frequency in [Hz]

#define NOFF   0		// Phase offset

// Decimations factors

#define CIC2_DEC  2
#define CIC5_DEC 32
#define RCF_DEC   8

#define NTAP     256          // Number of filter coefficients

int  Coef[NTAP] = {
#include "ad6620_rcf.inc"
   };


//=============================================================================== 
// Resets AD6620 using flag 6 of the DSP
void AD6620_HW_Reset(void)
{
	long tempWait = 0;

	// initialize flag and set to 1
	*(long*)(IOFLAG) |= FLG6O;
	*(long*)(IOFLAG) |= FLG6;

	// reset AD6620 active low
	*(long*)(IOFLAG) &= ~(FLG6);
    for(tempWait = 200;tempWait;tempWait--)	
	{
		asm("nop;");
	}
	*(long*)(IOFLAG) |= FLG6;
}

  				  
//=============================================================================== 
// Starts AD6620 in real single channel mode
// and Master sync 
int AD6620_Start()
{
   return AD6620_8Bit_Reg_Write(AD6620_MCR, 0x08);
}
//=============================================================================== 
// Puts AD6620 in software reset mode
int AD6620_SRESET()
{
   return AD6620_8Bit_Reg_Write(AD6620_MCR, 0x09);
}

//=============================================================================== 
// Set AD6620 Mode Control Register
//
// data format:
// bits 7:4 - reserved bits and sould be written with zeros 
//
// 0000xxx1 - AD6620 is in SOFT_RESET mode and defined as sync slave
// 0000x000 - Single Channel Real Mode
// 0000x010 - Diversity Channel Real Mode
// 0000x100 - Single Channel Complex Mode
// 00000xxx - AD6620 in Slave Mode, SYNC pins are inputs 
// 00001xxx - AD6620 in Master Mode, SYNC pins are outputs
// 
inline int Set_AD6620_MCR(int data)
{

   return AD6620_8Bit_Reg_Write(AD6620_MCR, ( int )data);

}
//=============================================================================== 
// Set AD6620 NCO Control Register
//
// data format:
// bits 7:3 - reserved bits and sould be written with zeros 
//
// 00000xx1 - NCO is by-passed 
// 00000x1x - Activate Phase Dither
// 000001xx - Activate Amplitude Dither
// 
inline int Set_AD6620_NCO_CR(int data)
{

   return AD6620_8Bit_Reg_Write(AD6620_NCO_CR, data);

}
//===============================================================================
// AD6620NSCR 0x302 // AD6620 NCO SYNC Control Register (32 bit register)
// Set AD6620 NCO Control Register
//
// data format:
// bits 31:0 - NCO SYNC mask shadow 
//
// 
inline int Set_AD6620_NCO_FREQ_CR(int data)
{

      return AD6620_32Bit_Reg_Write(AD6620_NCO_SYNC_CR, data);

}
//=============================================================================== 
// AD6620NFR  0x303 // AD6620 NCO Frequency Control Word Register (32 bit register)
// data format:
//    32 bit unsigned integer
// 
inline int Set_AD6620_NCO_Frequency(int data)
{

   return AD6620_32Bit_Reg_Write(AD6620_NCO_FREQ, data);
}

//===============================================================================
// AD6620NPO  0x304 // AD6620 NCO Phase Offset Register (16 bit register)
// Set AD6620 NCO Phase Offset Register
//
// data format:
// bits 15:0 - NCO phase offset 
//
// 
inline int Set_AD6620_NCO_Phase(int data)
{

   return AD6620_16Bit_Reg_Write(AD6620_NCO_PHASE, data);

}
//===============================================================================
// AD6620SC2  0x305 // AD6620 CIC2 Scale Factor Register (8 bit register) 
// Set AD6620 CIC2 Scale Factor Register
//
// data format:
// bits 2:0  CIC2 Scale Factor
//        3  Reserved
//        4  Exp Invert
//      7:5  ExpOff
// 
inline int Set_AD6620_CIC2_Scale(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_CIC2_SC, data);
}
//===============================================================================
// AD6620MC2  0x306 // AD6620 CIC2 decimation factor register (8 bit register: 2-16 (1-15))
// Set AD6620 CIC2 Decimation Factor Register
//
// data format:
// bits 3:0  CIC2 Decimation Factor -1 
// 
inline int Set_AD6620_CIC2_Decimation(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_CIC2_DEC, data-1);
}
//===============================================================================
// AD6620SC5  0x307 // AD6620 CIC5 Scale Factor Register (5 bit register)
// Set AD6620 CIC5 Scale Factor Register
//
// data format:
// bits 4:0  CIC5 Scale Factor
//      7:5  Reserved
// 
inline int Set_AD6620_CIC5_Scale(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_CIC5_SC, data);
}
//===============================================================================
// AD6620MC5  0x308 // AD6620 CIC5 Decimation Factor Registerb (8 bit register: 1-32 (0-31))
// Set AD6620 CIC5 Decimation Factor Register
//
// data format:
// bits 4:0  CIC5 Decimation Factor -1 
// 
inline int Set_AD6620_CIC5_Decimation(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_CIC5_DEC, data-1);
}
//===============================================================================
// AD6620RCR  0x309 // AD6620 RCF Scale & Control Register (4 bit register)
// Set AD6620 RCF Control Register
//
// data format:
// bits 2:0  RCF scale factor
//        3  Unique B Flag (0 -> Normal Mode, 1 -> Unique B Mode) 
//      7:4  Reserved   
// 
inline int Set_AD6620_RCF_Scale(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_RCF_SC, data);
}
//===============================================================================
// AD6620MRC  0x30a // AD6620 RCF Decimation Factor Register  (8 bit register: 1-32 (0-31))
// Set AD6620 RCF Decimation Factor Register
//
// data format:
// bits 4:0  RCF Decimation Factor -1 
// 
inline int Set_AD6620_RCF_Decimation(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_RCF_DEC, data-1);
}
//===============================================================================
// AD6620ROF  0x30b // AD6620 RCF Address Offset Register (8 bit register)
// Set AD6620 RCF Address Offset Register (8 bit register)
//
// data format:
// bits 7:0  RCF address offset register 
// 
inline int Set_AD6620_RCF_Offset(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_RCF_OFF, data);
}
//===============================================================================
// AD6620RNT  0x30c // Ad6620 RCF # Taps Register (8 bit register: max. 256 taps)
// Set AD6620 RCF Ntaps Register (8 bit register)
//
// data format:
// bits 7:0  RCF Ntaps-1 
// 
inline int Set_AD6620_RCF_Ntaps(int data)
{
   return AD6620_8Bit_Reg_Write(AD6620_RCF_NTAPS, data-1); 
}
//=============================================================================== 
// data format:
//    none
//  AD6620 Reserved Register !!!!!! must be written with zeros for CORRECT OPERATION
// 
inline int Set_AD6620_RESERVED()
{
   return AD6620_8Bit_Reg_Write(AD6620_RESERVED, 0); 
}

//=============================================================================== 
// data format:
//    none
//  write coefficient in to RCF Coefficient RAM
// 
inline int Set_AD6620_RCF_Coef(int Start_addr,int *Coef, int len)
{

  return AD6620_20Bit_Reg_Write(Start_addr, Coef, len);
}


//=============================================================================== 
//
// Initialize AD6620 in Master Single Channel Real Mode 
//
// Fsys = 32768 kHz
// Decimation factor = 512 ( CIC2=4, CIC5=32, RCF=4)
// Complex sample rate = 64 kHz
//

int AD6620_Init_Test(unsigned int Carrier_Frequency_Hz)
{
  int  i, addr,status;
  unsigned int  NCO_freq_word;

//  int RCoef[NTAP];
  				

//  AD6620 hard reset
  AD6620_Reset();                 
  
// calculate NCO frequency word
  NCO_freq_word = (unsigned int)(pow(2,32)*((float)Carrier_Frequency_Hz/CLK_FREQ_AD6620_HZ));

  status = Set_AD6620_MCR(0x09);    // Set AD6620 in to SOFTWARE RESET SLAVE Mode of operation
status = AD6620_8Bit_Reg_Read(AD6620_MCR);

  // write filter coefficients
  status = Set_AD6620_RCF_Coef(NOFF,Coef,NTAP);

//AD6620_20Bit_Reg_Read(NOFF, RCoef, NTAP);

// write control registers
  status = Set_AD6620_NCO_CR(0);
  status = Set_AD6620_NCO_FREQ_CR(0xffffffff);
//status = AD6620_32Bit_Reg_Read(AD6620_NCO_SYNC_CR);
  
  status = Set_AD6620_NCO_Frequency(NCO_freq_word);    
//status = AD6620_32Bit_Reg_Read(AD6620_NCO_FREQ);
  
  status = Set_AD6620_NCO_Phase(0);
//status = AD6620_16Bit_Reg_Read(AD6620_NCO_PHASE);

  status = Set_AD6620_CIC2_Scale(6);
  status = Set_AD6620_CIC2_Decimation(CIC2_DEC);

  status = Set_AD6620_CIC5_Scale(17);  //??
  status = Set_AD6620_CIC5_Decimation(CIC5_DEC);

  status = Set_AD6620_RCF_Scale(2);    //??
  status = Set_AD6620_RCF_Decimation(RCF_DEC);
  status = Set_AD6620_RCF_Offset(NOFF);
  status = Set_AD6620_RCF_Ntaps(NTAP);

  Set_AD6620_RESERVED();          // Never ever comment this line 

  return 0;
}

