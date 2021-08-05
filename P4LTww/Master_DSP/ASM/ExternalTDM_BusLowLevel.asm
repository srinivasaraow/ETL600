/*-------------------------------------------------------------------------*
 * $Workfile: ExternalTDM_BusLowLevel.asm $
 *
 * $Log: /Source/P4LT/Master_DSP/ASM/ExternalTDM_BusLowLevel.asm $
 * 
 * 2     18.09.05 16:14 Chchlee
 * CalculateFletcherChecksum removed
 * 
 * 1     19.01.05 15:47 Chchlee
 * 
 * 7     11.01.05 9:20 Chbrbus
 * Old FloatCheck removed, switched to ETL600 ArrayFloatCheck solution
 * 
 * 6     23.08.04 15:38 Leeb02
 * Some comments added
 * 
 * 5     8.07.04 16:01 Bussli02
 * Removed all G4AI extended time slot stuff
 * 
 * 4     8.07.04 14:59 Bussli02
 * External TDM Bus from Master DSP working (basic time slots tested)
 * 
 * 3     26.05.04 10:54 Bussli02
 * 
 *
 *-------------------------------------------------------------------------*/


  
/*-----------------------ASSEMBLER DIRECTIVES------------------------------*/

.PRECISION=40;
.round_nearest;  
#include "asm_sprt.h"
#include "def21161.h"


.segment/dm seg_dmda;

// For saving mode1 register
.var			_SavedMode1_register;

.endseg;

/*****************************************************************************
  Function:      void ComputeRxBasicHardTiming(volatile long *ActiveTDM_DMA_Ptr);
														r4
   
  Description: 	 Low level assembler function for extracting all basic 
				 time slot information (O4LE and G4AI) from the external 
				 TDM Bus DMA buffer - optimized from AES550
				  	                    
  Inputs:      	 ActiveTDM_DMA_Ptr   -  valid Rx DMA buffer pointer (all 32/16bit timeslots are received)

  Constant
  Tables:		 TDM_BasicTimeslotLookupTable
				 SlotID_ArrayMask

  Outputs:		 HardTimingInformationFromTDM
                 SoftTimingInformationStruct                                                            
                                                                                                                                        
  Return:      	 none
        	
  Globals:       none 

  Important:     Function is rewritten for a block cycle of 10.666 kHz                                                    	
******************************************************************************
*/


/*-----------------  external SDRAM Data memory 32 Bit  ---------------------------*/

.SEGMENT/DM		ext_dmda;

// constant tables, must be generated during startup by another function
.EXTERN _TDM_BasicTimeslotLookupTable;
.EXTERN _SlotID_ArrayMask;

// This is the interface for the FT1.2 bridge in both directions (RX and TX) 
// CAUTION: Must be included once, is used in 2 assembler functions
.EXTERN _SoftTimingInformationStruct;

.ENDSEG;

/*---------------------  Internal Data memory 32 Bit  -----------------------------*/

.SEGMENT/DM		seg_dmda;

// Output struct for ComputeRxBasicHardTiming
.EXTERN _HardTimingInformationFromTDM;

.ENDSEG;


/*---------------------  Internal Program memory 48 Bit  --------------------------*/

.SEGMENT/PM     seg_pmco;

.GLOBAL _ComputeRxBasicHardTiming;

_ComputeRxBasicHardTiming:

				leaf_entry;

				dm(_SavedMode1_register) = mode1;		// Switch off SIMD (speeddsp library uses it)
				
initHT_Rx:		bit clr mode1 PEYEN|IRPTEN;			/* Global interrupt disable (interrupt routines use secodary registers) */
				bit set mode1 SRRFH|SRD1L|SRD1H; 		/* use secondary registers (register file reg8-15) */
				nop;									/* wait for switchover */
				
				
beginHT_Rx:		i0 = _TDM_BasicTimeslotLookupTable;
				i1 = _SlotID_ArrayMask;
				i2 = _HardTimingInformationFromTDM;
				i3 = _SoftTimingInformationStruct;
				i4 = r4;								// DMA Buffer location
				
				// Get all latest received Slot IDs 0-6 (7 TDM cards only)
				r9 = dm(0, i4);
				r10 = FEXT r9 BY 0:3;
				r11 = FEXT r9 BY 16:3;
				dm(0,i2) = r10;
				dm(3,i2) = r11;
				r9 = dm(1, i4);
				r10 = FEXT r9 BY 0:3;
				r11 = FEXT r9 BY 16:3;
				dm(6,i2) = r10;
				dm(9,i2) = r11;
				r9 = dm(2, i4);
				r10 = FEXT r9 BY 0:3;
				r11 = FEXT r9 BY 16:3;
				dm(12,i2) = r10;
				dm(15,i2) = r11;
				r9 = dm(3, i4);
				r10 = FEXT r9 BY 0:3;
				dm(18,i2) = r10;
				
				// Get the soft timing information into the Rx shift registers for odd and even cases
				// cycle on master DSP is now 10.666 KHz, so only 1 soft timing bit is available now
				r9 = dm(11, i3);		// Get soft timing shift register value (ReceivedSoftTimingBits)
				r10 = dm(10, i3);		// ActiveBasicTimeSlot
				r11 = FEXT r10 BY 0:1;	// LSB:    ActiveBasicTimeSlot & BIT0
				r10 = FEXT r10 BY 1:3;	// Offset: (ActiveBasicTimeSlot & BIT1-3) >> 4
				r14 = 0x00000010;		// Soft timing bit even timeslots (DST, Bit 4)
				m4 = r10;				// m4 = ActiveBasicTimeSlot
				r12 = 1;
				r8 = 0x00100000;		// Soft timing bit odd timeslots (DST, Bit 4 + 16)
				COMP(r12, r11);
				IF EQ r14 = r8;			// R14 is bit to test 
				r15 = 0x80000000;		// set MSB
				r9 = FEXT r9 BY 1:31;	// ReceivedSoftTimingBits >>= 1
				r12 = r15 OR r9;		// Set MSB if soft timing bit is true (r12 = ReceivedSoftTimingBits | BIT31 )
				r13 = dm(m4, i4);		// Get basic time slot value
				r13 = r13 AND r14;		// test if DST set (r13 &= DST mask)
				COMP(r13, r14);			// EQ if DST was 1, NE if DST was 0
				IF NE r12 = r9;			 
				dm(11, i3) = r12;		// write back: ReceivedSoftTimingBits = r12
				
				// Check basic time slot checksum and transfer received words to buffer
				r9 = dm(0, i4);
				r10 = FEXT r9 BY 4:8;					// Extract 8 Databits
				m0 = r10;
				r11 = dm(m0,i0);
				r12 = dm(0, i1);						// Slot ID
				r11 = r11 + r12;
				r8 = FEXT r11 BY 0:16;
				r13 = FEXT r9 BY 0:16;
				COMP(r13, r8);
				IF NE JUMP error0;
				r13 = FEXT r9 BY 5:7;
				dm(1, i2) = r13;
				r13 = 0;
				dm(2, i2) = r13;							// Set back error status
				jump eval1;
error0:			r15 = dm(2, i2);
				r15 = r15 + 1;
				dm(2, i2) = r15;
eval1:			r10 = FEXT r9 BY 20:8;					// next additional7Bits
				m0 = r10;
				r11 = dm(m0,i0);
				r12 = dm(1, i1);						// Slot ID
				r11 = r11 + r12;
				r12 = FEXT r11 BY 0:16;
				r13 = FEXT r9 BY 16:16;
				COMP(r12, r13);
				IF NE JUMP error1;
				r13 = FEXT r9 BY 21:7;
				dm(4,i2) = r13;
				r13 = 0;
				dm(5, i2) = r13;							// Set back error status
				jump eval2;
error1:			r15 = dm(5, i2);
				r15 = r15 + 1;
				dm(5,i2) = r15;
				
eval2:			r9 = dm(1, i4);
				r10 = FEXT r9 BY 4:8;					// Extract 8 Databits
				m0 = r10;
				r11 = dm(m0,i0);
				r12 = dm(2, i1);						// Slot ID
				r11 = r11 + r12;
				r8 = FEXT r11 BY 0:16;
				r13 = FEXT r9 BY 0:16;
				COMP(r13, r8);
				IF NE JUMP error2;
				r13 = FEXT r9 BY 5:7;
				dm(7, i2) = r13;
				r13 = 0;
				dm(8, i2) = r13;							// Set back error status
				jump eval3;
error2:			r15 = dm(8, i2);
				r15 = r15 + 1;
				dm(8, i2) = r15;
eval3:			r10 = FEXT r9 BY 20:8;					// next additional7Bits
				m0 = r10;
				r11 = dm(m0,i0);
				r12 = dm(3, i1);						// Slot ID
				r11 = r11 + r12;
				r12 = FEXT r11 BY 0:16;
				r13 = FEXT r9 BY 16:16;
				COMP(r12, r13);
				IF NE JUMP error3;
				r13 = FEXT r9 BY 21:7;
				dm(10,i2) = r13;
				r13 = 0;
				dm(11, i2) = r13;							// Set back error status
				jump eval4;
error3:			r15 = dm(11, i2);
				r15 = r15 + 1;
				dm(11, i2) = r15;
				
eval4:			r9 = dm(2, i4);
				r10 = FEXT r9 BY 4:8;					// Extract 8 Databits
				m0 = r10;
				r11 = dm(m0,i0);
				r12 = dm(4, i1);						// Slot ID
				r11 = r11 + r12;
				r8 = FEXT r11 BY 0:16;
				r13 = FEXT r9 BY 0:16;
				COMP(r13, r8);
				IF NE JUMP error4;
				r13 = FEXT r9 BY 5:7;
				dm(13, i2) = r13;
				r13 = 0;
				dm(14, i2) = r13;							// Set back error status
				jump eval5;
error4:			r15 = dm(14, i2);
				r15 = r15 + 1;
				dm(14, i2) = r15;
eval5:			r10 = FEXT r9 BY 20:8;					// next additional7Bits
				m0 = r10;
				r11 = dm(m0,i0);
				r12 = dm(5, i1);						// Slot ID
				r11 = r11 + r12;
				r12 = FEXT r11 BY 0:16;
				r13 = FEXT r9 BY 16:16;
				COMP(r12, r13);
				IF NE JUMP error5;
				r13 = FEXT r9 BY 21:7;
				dm(16,i2) = r13;
				r13 = 0;
				dm(17, i2) = r13;							// Set back error status
				jump eval6;
error5:			r15 = dm(17, i2);
				r15 = r15 + 1;
				dm(17,i2) = r15;
				
eval6:			r9 = dm(3, i4);
				r10 = FEXT r9 BY 4:8;					// Extract 8 Databits
				m0 = r10;
				r11 = dm(m0,i0);
				r12 = dm(6, i1);						// Slot ID
				r11 = r11 + r12;
				r8 = FEXT r11 BY 0:16;
				r13 = FEXT r9 BY 0:16;
				COMP(r13, r8);
				IF NE JUMP error6;
				r13 = FEXT r9 BY 5:7;
				dm(19, i2) = r13;
				r13 = 0;
				dm(20, i2) = r13;							// Set back error status
				jump returnHT_Rx;
error6:			r15 = dm(20, i2);
				r15 = r15 + 1;
				dm(20, i2) = r15;

														/* use primary DAG1L and DAG2L registers */
														/* save data pointers in dm for alternative register switchover */
									  					/* Global interrupt enable */
returnHT_Rx:	mode1 = dm(_SavedMode1_register);		// use primary registers and restore mode1 register
				nop;
				leaf_exit;								/* go back to funtion */
			

.ENDSEG;		                 
		                 

/*****************************************************************************
  Function:      void GenerateTxBasicHardTiming(volatile long *ActiveTDM_DMA_Ptr);
														r4

  Description: 	 Low level assembler function for generating all basic 
				 time slot information (O4LE and G4AI) to the external 
				 TDM Bus DMA buffer - optimized from AES550
				  	                    
  Inputs:      	 HardTimingInformationToTDM
				 SoftTimingInformationStruct

  Constant
  Tables:		 TDM_BasicTimeslotLookupTable
				 SlotID_ArrayMask

  Outputs:		 ActiveTDM_DMA_Ptr   -  valid Tx DMA buffer pointer                                                           
                                                                                                                                        
  Return:      	 none
        	
  Globals:       none 

  Important:     Function is rewritten for a block cycle of 10.666 kHz                                                    	
******************************************************************************
*/

/*---------------------  Internal Data memory 32 Bit  -----------------------------*/

.SEGMENT/DM		seg_dmda;

.EXTERN _HardTimingInformationToTDM;

.ENDSEG;

/*---------------------  Internal Program memory 48 Bit  --------------------------*/

//  call:  void GenerateTxBasicHardTiming(long *DMA_Tx_BufferPtr);



.SEGMENT/PM     seg_pmco;

.GLOBAL _GenerateTxBasicHardTiming;

_GenerateTxBasicHardTiming:

				leaf_entry;

				dm(_SavedMode1_register) = mode1;		// Switch off SIMD (speeddsp library uses it)

initHT_Tx:		bit clr mode1 PEYEN|IRPTEN;				/* Global interrupt disable (interrupt routines use secodary registers) */
				bit set mode1 SRRFH|SRD1L|SRD1H; 		/* use secondary registers (register file reg8-15) */
				nop;									/* wait for switchover */

beginHT_Tx:		i0 = _TDM_BasicTimeslotLookupTable;
				i1 = _SlotID_ArrayMask;
				i2 = _HardTimingInformationToTDM;
				i3 = _SoftTimingInformationStruct;
				//i4 = _G4AI_TogglingBitInformation;
				i5 = r4;								// DMA Buffer location
				
				// Prepare TimeSlot 0
				r9 = dm(1, i2);							// Get first additional_7Bits Timeslot 0 and bitshift
				r9 = FDEP r9 BY 1:7;
				r10 = dm(0, i3);						// Get first soft timing bit and OR with Additional_7Bits
				r11 = r9 OR r10;
				m0 = r11;
				r13 = dm(m0, i0);						// Lookup Checksum and basic Timeslot word 
				r14 = dm(0, i1);						// Slot ID
				r13 = r13 + r14;						// Add Slot ID and Checksum
				r15 = FEXT r13 BY 0:16;					// AND 0x0000ffff

				// Prepare TimeSlot 1
				r9 = dm(4, i2);							// next Additional7Bits
				r9 = FDEP r9 BY 1:7;
				r10 = dm(1, i3);
				r11 = r9 OR r10;
				m0 = r11;
				r13 = dm(m0, i0);						// Lookup Checksum and basic Timeslot word 
				r14 = dm(1, i1);						// Slot ID
				r13 = r13 + r14;
				r13 = FDEP r13 BY 16:16;				// AND 0xffff0000
				// Mux Timeslot 0 and 1 and write to external TDM Bus Tx DMA Buffer
				r15 = r15 OR r13;
				dm(0, i5) = r15;						// Write to TDM Buffer
				
				// Prepare TimeSlot 2
				r9 = dm(7, i2);							// Get next additional_7Bits Timeslot 0 and bitshift
				r9 = FDEP r9 BY 1:7;
				r10 = dm(2, i3);						// Get next soft timing bit and OR with Additional_7Bits
				r11 = r9 OR r10;
				m0 = r11;
				r13 = dm(m0, i0);						// Lookup Checksum and basic Timeslot word 
				r14 = dm(2, i1);						// Slot ID
				r13 = r13 + r14;						// Add Slot ID and Checksum
				r15 = FEXT r13 BY 0:16;					// AND 0x0000ffff
				
				// Prepare TimeSlot 3
				r9 = dm(10, i2);						// next Additional7Bits
				r9 = FDEP r9 BY 1:7;
				r10 = dm(3, i3);
				r11 = r9 OR r10;
				m0 = r11;
				r13 = dm(m0, i0);						// Lookup Checksum and basic Timeslot word
				r14 = dm(3, i1);						// Slot ID
				r13 = r13 + r14;
				r13 = FDEP r13 BY 16:16;				// AND 0xffff0000
				// Mux Timeslot 2 and 3 and write to external TDM Bus Tx DMA Buffer
				r15 = r15 OR r13;
				dm(1, i5) = r15;						// Write to TDM Buffer
				
				// Prepare TimeSlot 4
				r9 = dm(13, i2);						// Get next additional_7Bits Timeslot 0 and bitshift
				r9 = FDEP r9 BY 1:7;
				r10 = dm(4, i3);						// Get next soft timing bit and OR with Additional_7Bits
				r11 = r9 OR r10;
				m0 = r11;
				r13 = dm(m0, i0);						// Lookup Checksum and basic Timeslot word 
				r14 = dm(4, i1);						// Slot ID
				r13 = r13 + r14;						// Add Slot ID and Checksum
				r15 = FEXT r13 BY 0:16;
				
				// Prepare TimeSlot 5
				r9 = dm(16, i2);						// next Additional7Bits
				r9 = FDEP r9 BY 1:7;
				r10 = dm(5, i3);
				r11 = r9 OR r10;
				m0 = r11;
				r13 = dm(m0, i0);						// Lookup Checksum and basic Timeslot word 
				r14 = dm(5, i1);						// Slot ID
				r13 = r13 + r14;
				r13 = FDEP r13 BY 16:16;				// AND 0xffff0000
				// Mux Timeslot 4 and 5 and write to external TDM Bus Tx DMA Buffer
				r15 = r15 OR r13;
				dm(2, i5) = r15;						// Write to TDM Buffer
				
				// Prepare TimeSlot 6
				r9 = dm(19, i2);						// Get next additional_7Bits Timeslot 0 and bitshift
				r9 = FDEP r9 BY 1:7;
				r10 = dm(6, i3);						// Get next soft timing bit and OR with Additional_7Bits
				r11 = r9 OR r10;
				m0 = r11;
				r13 = dm(m0, i0);						// Lookup Checksum and basic Timeslot word 
				r14 = dm(6, i1);						// Slot ID
				r13 = r13 + r14;						// Add Slot ID and Checksum
				r15 = FEXT r13 BY 0:16;					// AND 0x0000ffff
				// Write TimeSlot 6 to external TDM Bus Tx DMA Buffer
				dm(3, i5) = r15;						// Write to TDM Buffer
			

														/* use primary DAG1L and DAG2L registers */
														/* save data pointers in dm for alternative register switchover */
				  										/* Global interrupt enable */
returnHT_Tx:	mode1 = dm(_SavedMode1_register);		// use primary registers and restore mode1 register
				nop;
				leaf_exit;								/* go back to funtion */
			

.ENDSEG;		                 
		                 

           
	
