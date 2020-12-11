/*-------------------------------------------------------------------------*
 * $Workfile: AD6620_Init.h $
 * Part of      : ETL600 / System communication
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2  -NOT a PhD-
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/FE_DSP/H/AD6620_Init.h $
 * 
 * 1     19.01.05 15:31 Chchlee
 * 
 * 2     7.05.04 13:30 Bussli02
 * 
 * 1     16.04.04 10:14 Bussli02
 * Initial version
 * 
 *
 *-------------------------------------------------------------------------*/

#ifndef AD6620_INIT_H
#define AD6620_INIT_H


/* INCLUDE FILES **************************************************************/

// Access to the internal registers of the DDC AD6620 is done by accessing the MS2
// address range of the frontend DSP. The first 7 lowest addresses gives the full
// memory mapped access to the registers.
// MS2 starts at 0x08000000

// AD6620 data exchange defines
#define AD6620_DATA_REGISTER0			0x08000000		// [7:0] used for data word[7:0]
#define AD6620_DATA_REGISTER1			0x08000001		// [7:0] used for data word[15:8]
#define AD6620_DATA_REGISTER2			0x08000002		// [7:0] used for data word[23:16]
#define AD6620_DATA_REGISTER3			0x08000003		// [7:0] used for data word[31:24]
#define AD6620_DATA_REGISTER4			0x08000004		// [7:0] used for data word[35:32]
#define AD6620_RESERVED					0x08000005


#define AD6620_LOW_ADDRESS_REGISTER		0x08000006		// internal low address register select [7:0] of AD6620
#define AD6620_ADDRESS_MODE_REGISTER	0x08000007		// internal high address register select [1:0] of AD6620
														// [5:2] reserved
														// Bit 6: Read Increment
														// Bit 7: Write Increment


// AD6620 internal register defines
#define AD6620_REGISTER_RFC_COEFFICIENT_RAM_START		0x00000000
#define AD6620_REGISTER_RFC_COEFFIEIENT_RAM_END			0x000000ff

#define AD6620_REGISTER_RFC_DATA_RAM_START				0x00000100
#define AD6620_REGISTER_RFC_DATA_RAM_END				0x000001ff

#define AD6620_REGISTER_MODE_CONTROL_REGISTER			0x00000300

#define AD6620_REGISTER_NCO_CONTROL_REGISTER			0x00000301

#define AD6620_REGISTER_NCO_SYNC_CONTROL_REGISTER		0x00000302

#define AD6620_REGISTER_NCO_FREQ						0x00000303

#define AD6620_REGISTER_NCO_PHASE_OFFSET				0x00000304

#define AD6620_REGISTER_CIC2_SCALE_REGISTER				0x00000305

#define AD6620_REGISTER_CIC2_DECIMATION_MINUS1			0x00000306

#define AD6620_REGISTER_CIC5_SCALE_REGISTER				0x00000307

#define AD6620_REGISTER_CIC5_DECIMATION_MINUS1			0x00000308

#define AD6620_REGISTER_OUTPUT_RCF_CONTROL_REGISTER		0x00000309

#define AD6620_REGISTER_RCF_DECIMATION_MINUS1			0x0000030a

#define AD6620_REGISTER_RCF_ADDRESS_OFFSET_REGISTER		0x0000030b

#define AD6620_REGISTER_NUMBER_OF_TAPS_MINUS1			0x0000030c

#define AD6620_REGISTER_RESERVED_MUST_BE_SET_0			0x0000030d


/* PUBLIC TYPE DECLARATIONS ***************************************************/
/* PUBLIC CONSTANT DECLARATIONS ***********************************************/


#endif