//---------------------------------------------------------------------------
// COPYRIGHT:  (c) 2004 TL-Consulting GmbH, Frankfurt am Main 
//
// MODULE      ad5321_io.asm
//
// PROJECT:    P4LT system board of ETL600
//
// COMPILER    VisualDSP 3.0, service pack 1
//
// DATE        23-11-2004
//
// AUTHOR:     Dr. Vladimir Potapov
//  
// ABSTRACT    AD5321 driver fot frequency control based on I2C driver
//             
//---------------------------------------------------------------------------
/* $Log: /Source/P4LT Prototype 2/OFDM_DSP/C/DCTCXO/ad5321_io.c $
 * 
 * 1     19.01.05 15:39 Chchlee
 * 
 * 1     3.12.04 11:33 Chhamaa
 * 
 * 1     23.11.04 vpotapov
 * 
 */	
//---------------------------------------------------------------------------
#include <cdef21161.h>
#include <def21161.h>
asm("#include <def21161.h>");

#include "../../h/dctcxo/ad5321.h"
#include "../../h/dctcxo/ad5321_externs.h"

//====================================================================
void  segment ( "seg_ext_pmco" ) AD5321_Init()
{
//	unsigned int Tm_Counter;
	
    I2C_WR_Index_Tx = 0;
    I2C_RD_Index_Tx = 0;
    I2C_Bit_Mask = 0x80;

    // Set timer period
	I2C_Tm_Counter = F_CORE_KHZ/(F_SCL_KHZ) - 1;
    Timer_Set(I2C_Tm_Counter);	
	
    // Enable timer
    Timer_Enable();
    I2C_Tm_Disable = 0;
    		
    // initialize Timer interrupt services
    asm("bit set IMASK TMZHI;");

    I2C_Bit_Mask = 0x80;
    I2C_Busy = 0;
    
    // Set Vosc=1.65V
    // Repeated twice in order to go from Vosc=3.22V
    // on start up 
    AD5321_Set_Reg(AD5321_NORMAL_MODE, VOSC165VCODE);     	
    AD5321_Wait();
    asm("nop;");
    AD5321_Set_Reg(AD5321_NORMAL_MODE, VOSC165VCODE);     	
    AD5321_Wait();


}

//==============================================================================

void segment ( "seg_ext_pmco" ) AD5321_Set_Reg(unsigned int Mode, unsigned int Data) 
{
	unsigned int Command, Word24;
    void AD5321_Write(unsigned int);

	Command  = 0x18;  // 00011000: A0 = 0, A1= 0, Write
	Mode    &= 0x3;   // PD1 & PD0
	Data    &= 0xfff; // D11 - D0
	Word24 = (Command<<16) | (Mode<<12) | Data;
    AD5321_Write(Word24);
}

//==============================================================================

void segment ( "seg_ext_pmco" ) AD5321_Write(unsigned int word24)
{
	// State1, write start
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = AD5321_WR_START;
	
	// State2, write command Write register
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = (word24>>16) | AD5321_WR_BYTE;

	// State3, read acknowledge
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = AD5321_RD_ACK;
		
	// State4, write 0 0 PD1 PD2 D11 D10 D9 D8 bits
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = ((word24>>8) & 0xff) | AD5321_WR_BYTE;
	
	// State5, read acknowledge
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = AD5321_RD_ACK;
	
	// State6, write D7 D6 D5 D4 D3 D2 D1 D0 bits
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = (word24 & 0xff) | AD5321_WR_BYTE;
	
	// State7, read acknowledge
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = AD5321_RD_ACK;
	
	// State8, write stop
	AD5321_TX_Buffer[I2C_WR_Index_Tx++] = AD5321_WR_STOP;
	if (I2C_WR_Index_Tx == sizeof(AD5321_TX_Buffer)) I2C_WR_Index_Tx = 0;
	
	if ( I2C_Tm_Disable != 0) { 
      Timer_Set(I2C_Tm_Counter);		
      // Enable timer
      Timer_Enable();
      I2C_Tm_Disable = 0;
	}

}

