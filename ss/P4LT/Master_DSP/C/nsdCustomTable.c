/*-------------------------------------------------------------------------*
 * $Workfile: nsdCustomTable.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2005
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source O4CV/R1LA/src/apps/Lan600/nsdCustomTable.c $ 
 * 
 * 38    08-01-03 10:58 Chmabur1
 * added newline to end of file
 * 
 * 37    19.12.07 9:11 Chchlee
 * Variable "returnValue" removed/ conditional compilation for
 * "calculateChecksum" to avoid warning during R1LA compilation
 * 
 * 35    29.11.06 11:17 Chchlee
 * #ifdef structure modified 
 * 
 * 34    29.11.06 9:49 Chchlee
 * test tones adjusted
 * 
 * 33    29.11.06 9:19 Chpeplu
 * new line added
 * 
 * 32    29.11.06 8:45 Chpeplu
 * In txTable_Mode1-3 and rxTable_Mode1-3 frequency combinations for
 * commands changed
 * 
 * 31    27.11.06 19:20 Chchlee
 * Checksum added. Version in unpacked custom table removed
 * 
 * 30    23.11.06 12:10 Chchlee
 * unpacking now generates "000000" for unused tones also if there are
 * unused commands
 * 
 * 29    23.11.06 11:32 Chchlee
 * testTone for Modes 2-99 set to 20
 * 
 * 28    23.11.06 7:47 Chchlee
 * Guard command in Mode 3 and 99 corrected
 * 
 * 27    22.11.06 13:48 Chchlee
 * Mode 99 added. Unused commands considered.
 * 
 * 26    20.11.06 17:16 Chchlee
 * NSD extended custom table finalized
 * 
 * 25    17.11.06 11:27 Chchlee
 * debug switches turned off
 * 
 * 24    17.11.06 7:55 Chchlee
 * Improved code and commenting
 * 
 * 23    15.11.06 15:58 Chchlee
 * #endif moved
 * 
 * 22    15.11.06 15:50 Chchlee
 * File I/O added
 * 
 * 21    14.11.06 16:17 Chchlee
 * code for generation of custom.nsd added
 * 
 * 20    14.11.06 14:05 Chmaflu
 * change customtableValues
 * 
 * 19    14.11.06 9:57 Chchlee
 * return value "BOOL" added to "nsdCustomTable_readCustomTableFile"
 * 
 * 18    10.11.06 8:50 Chchlee
 * Newline added to end of file to please DSP compiler
 * 
 * 17    9.11.06 19:55 Chchlee
 * unpacking implemented
 * 
 * 16    9.11.06 7:54 Chchlee
 * packer of nsdCustomTable_packEx implemented
 * 
 * 15    8.11.06 9:34 Chchlee
 * Newline added to end of file to please DSP compiler
 * 
 * 14    8.11.06 9:26 Chchlee
 * First test version with NSD extended custom table
 * 
 * 10    16.03.05 10:22 Chpeplu
 * longIndex must be set to zero after reset table
 * 
 * 9     15.03.05 17:47 Chpeplu
 * Function nsdCustomTable_pack() corrected
 * 
 * 8     11.03.05 16:24 Chpeplu
 * Testfunction name changed to testcustom()
 * 
 * 7     9.03.05 14:04 Chpeplu
 * Some functions moved to NSD_CustomTableConverter.c
 * 
 * 6     8.03.05 17:30 Chpeplu
 * function GenerateTxPriorityTable() added but not yet finish
 * 
 * 5     8.03.05 10:45 Chpeplu
 * Replace not used frequency combination (NOT_USED_FREQUENCY_COMBINATION)
 * by 0x0000 for code table added!
 * 
 * 4     8.03.05 9:33 Chpeplu
 * functions ConvertCustom2CommandTable() and GenerateDetectorCodeTable()
 * added
 * 
 * 3     2.03.05 17:03 Chpeplu
 * Command table for NSD600 Default filled in testTable1
 * 
 * 2     21.02.05 19:25 Chchlee
 * finalized
 * 
 * 1     21.02.05 9:39 Chchlee
 *-------------------------------------------------------------------------*/


//#define DEBUG_NSD_EXTENDED_CUSTOM_TABLES


/*
Size of NSD extended packed custom tables:

			#bits	#longs
Mode 1		209		6.53 --> 7
Mode 2		220		6.88 --> 7
Mode 3		232		7.25 --> 8
Mode 99		232		7.25 --> 8   Mode 3 without the commands A and E

*/



/* INCLUDE FILES **************************************************************/
#include "nsdCustomTable.h"
#include "NSD_Types.h"

#include "assert.h"

#include "stdio.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// ******** begin of test code for R1.X custom table ************
/*
t_NSDCustomTable testTableIn = 
{	// command   A          B          C          D          E          F						Combination		Command
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			not used
				{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
				{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
				{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			CD
				{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			ABCD
				{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			BCD
				{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			ACD
				{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			ABD
				{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			D
				{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			BD
				{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			AD
				{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			ABC
				{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			C
				{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			BC
				{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			AC
				{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			B
				{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			AB
				{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			A
};

t_NSDCustomTable default1UnpackedTable = 
	{	
		// command 
		// A          B          C          D          E          F						Combination		Command
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 1:	F1 / F2			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 2:	F1 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 3:	F1 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 4:	F1 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 5:	F1 / F6			not used
		{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 6:	F1 / F7			Test
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 7:	F2 / F3			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 8:	F2 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 9:	F2 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 10:	F2 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 11:	F2 / F7			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 12:	F3 / F4			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 14:	F3 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 15:	F3 / F7			not used
		{TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_X, TERNARY_X, TERNARY_X}, // row 16:	F4 / F5			C
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 17:	F4 / F6			not used
		{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 18:	F4 / F7			not used
		{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 19:	F5 / F6			B
		{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 20:	F5 / F7			AB
		{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_X, TERNARY_X}, // row 21:	F6 / F7			A
	};

t_NSDCustomTable testTable2;

t_PackedNSDCustomTable packedTestTable;

unsigned long nn_commands;
*/
// ******** end of test code for R1.X custom table ************

//########################################################################################################
// NSD extended custom tables for test
//########################################################################################################
#ifdef DEBUG_NSD_EXTENDED_CUSTOM_TABLES


	static SEGMENT_EXT_DATA t_RxTxTable txTable_Mode1 =
	{	// command   A          B          C          D          E          F						Combination		Command
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_1}, // row 1:	F1 / F2			F
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0}, // row 2:	F1 / F3			CD
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_1, TERNARY_0}, // row 3:	F1 / F4			DE
					{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0}, // row 4:	F1 / F5			ABCD
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_0, TERNARY_1}, // row 5:	F1 / F6			DF
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_1}, // row 6:	F1 / F7			EF
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_0}, // row 7:	F2 / F3			E
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0}, // row 8:	F2 / F4			BD		
					{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0}, // row 9:	F2 / F5			BCD
					{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0}, // row 10:	F2 / F6			ACD
					{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0}, // row 11:	F2 / F7			ABD
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0}, // row 12:	F3 / F4	-		D		
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 13:	F3 / F5			T
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0}, // row 14:	F3 / F6	-		AD
					{TERNARY_1, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0}, // row 15:	F3 / F7	-		ABC
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0}, // row 16:	F4 / F5	-		C
					{TERNARY_0, TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0}, // row 17:	F4 / F6	-		BC
					{TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0}, // row 18:	F4 / F7	-		AC
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 19:	F5 / F6	-		B
					{TERNARY_1, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 20:	F5 / F7	-		AB
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 21:	F6 / F7	-		A	
	};


	static SEGMENT_EXT_DATA t_RxTxTable txTable_Mode2 = 
	{	// command   A          B          C          D          E          F						Combination		Command
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1}, // row 1:	F1 / F2			F
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_1}, // row 2:	F1 / F3			DEF
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1}, // row 3:	F1 / F4			CEF
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1}, // row 4:	F1 / F5			BEF
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1}, // row 5:	F1 / F6			AEF
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 6:	F1 / F7			T
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_0}, // row 7:	F2 / F3			D
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_1}, // row 8:	F2 / F4			DF
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1}, // row 9:	F2 / F5			CF
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1}, // row 10:	F2 / F6			BF
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1}, // row 11:	F2 / F7			AF
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0}, // row 12:	F3 / F4			E
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0}, // row 14:	F3 / F6			BE
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0}, // row 15:	F3 / F7			AE
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0}, // row 16:	F4 / F5			C
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0}, // row 17:	F4 / F6			CE		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_1, TERNARY_0}, // row 18:	F4 / F7			DE		
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 19:	F5 / F6			B		
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1}, // row 20:	F5 / F7			EF
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 21:	F6 / F7			A
	};

	static SEGMENT_EXT_DATA t_RxTxTable rxTable_Mode2 = 
	{	// command   A  B  C  D  E  F		        Combination		Command
					{0, 0, 0, 0, 0, 0}, // row 1:	F1 / F2			F
					{0, 0, 0, 1, 1, 1}, // row 2:	F1 / F3			DEF
					{0, 0, 0, 0, 0, 0}, // row 3:	F1 / F4			CEF
					{0, 0, 0, 0, 0, 0}, // row 4:	F1 / F5			BEF
					{0, 0, 0, 0, 0, 0}, // row 5:	F1 / F6			AEF		
					{0, 0, 0, 0, 0, 0}, // row 6:	F1 / F7			T
					{0, 0, 0, 1, 0, 0}, // row 7:	F2 / F3			D		
					{0, 0, 0, 1, 0, 1}, // row 8:	F2 / F4			DF
					{0, 0, 0, 0, 0, 0}, // row 9:	F2 / F5			CF
					{0, 0, 0, 0, 0, 0}, // row 10:	F2 / F6			BF
					{0, 0, 0, 0, 0, 0}, // row 11:	F2 / F7			AF
					{0, 0, 0, 0, 0, 0}, // row 12:	F3 / F4			E		
					{0, 0, 0, 0, 0, 0}, // row 13:	F3 / F5			not used
					{0, 0, 0, 0, 0, 0}, // row 14:	F3 / F6			BE		
					{0, 0, 0, 0, 0, 0}, // row 15:	F3 / F7			AE		
					{0, 0, 0, 0, 0, 0}, // row 16:	F4 / F5			C		
					{0, 0, 0, 0, 0, 0}, // row 17:	F4 / F6			CE	
					{0, 0, 0, 1, 1, 0}, // row 18:	F4 / F7			DE
					{0, 0, 0, 0, 0, 0}, // row 19:	F5 / F6			B
					{0, 0, 0, 0, 0, 0}, // row 20:	F5 / F7			EF
					{0, 0, 0, 0, 0, 0}, // row 21:	F6 / F7			A
	};

	static SEGMENT_EXT_DATA t_RxTxTable txTable_Mode3 = 
	{	// command   A          B          C          D          E          F						Combination		Command
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1}, // row 1:	F1 / F2			F
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_1}, // row 2:	F1 / F3			ABCEF
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1}, // row 3:	F1 / F4			CF		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_1}, // row 4:	F1 / F5			ABCF		
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1}, // row 5:	F1 / F6			AF		
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 6:	F1 / F7			T
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1}, // row 7:	F2 / F3			EF		
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1}, // row 8:	F2 / F4			AEF
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_0}, // row 9:	F2 / F5			CE		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_0}, // row 10:	F2 / F6			ABCE
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_1}, // row 11:	F2 / F7			BEF
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0}, // row 12:	F3 / F4			E		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0}, // row 14:	F3 / F6			BE
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0}, // row 15:	F3 / F7			AE		
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0}, // row 16:	F4 / F5			C		
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_1}, // row 17:	F4 / F6			BF		
					{TERNARY_0, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_1, TERNARY_1}, // row 18:	F4 / F7			CEF
					{TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 19:	F5 / F6			B		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0}, // row 20:	F5 / F7			ABC
					{TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 21:	F6 / F7			A
	};

	static SEGMENT_EXT_DATA t_RxTxTable rxTable_Mode3 = 
	{	// command   A  B  C  D  E  F		        Combination		Command
					{0, 0, 0, 0, 0, 0}, // row 1:	F1 / F2		F
					{1, 1, 1, 0, 1, 1}, // row 2:	F1 / F3		ABCEF	
					{0, 0, 0, 0, 0, 0}, // row 3:	F1 / F4		CF		
					{1, 1, 1, 0, 0, 1}, // row 4:	F1 / F5		ABCF	
					{0, 0, 0, 0, 0, 0}, // row 5:	F1 / F6		AF		
					{0, 0, 0, 0, 0, 0}, // row 6:	F1 / F7		T	
					{0, 0, 0, 0, 0, 0}, // row 7:	F2 / F3		EF		
					{0, 0, 0, 0, 0, 0}, // row 8:	F2 / F4		AEF	
					{0, 0, 0, 0, 0, 0}, // row 9:	F2 / F5		CE		
					{1, 1, 1, 0, 1, 0}, // row 10:	F2 / F6		ABCE	
					{0, 0, 0, 0, 0, 0}, // row 11:	F2 / F7		BEF	
					{0, 0, 0, 0, 0, 0}, // row 12:	F3 / F4		E		
					{0, 0, 0, 0, 0, 0}, // row 13:	F3 / F5		not used	
					{0, 0, 0, 0, 0, 0}, // row 14:	F3 / F6		BE	
					{0, 0, 0, 0, 0, 0}, // row 15:	F3 / F7		AE		
					{0, 0, 0, 0, 0, 0}, // row 16:	F4 / F5		C		
					{0, 0, 0, 0, 0, 0}, // row 17:	F4 / F6		BF		
					{0, 0, 0, 0, 0, 0}, // row 18:	F4 / F7		CEF	
					{0, 0, 0, 0, 0, 0}, // row 19:	F5 / F6		B		
					{1, 1, 1, 0, 0, 0}, // row 20:	F5 / F7		ABC	
					{0, 0, 0, 0, 0, 0}, // row 21:	F6 / F7		A		
	};


	// mode 99 is copied from Mode 3, with unused commands A and E

	static SEGMENT_EXT_DATA t_RxTxTable txTable_Mode99 = 
	{	// command   A          B          C          D          E          F						Combination		Command
					{TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_1}, // row 1:	F1 / F2			F
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_X, TERNARY_1}, // row 2:	F1 / F3			ABCEF
					{TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_1}, // row 3:	F1 / F4			CF		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_X, TERNARY_1}, // row 4:	F1 / F5			ABCF		
					{TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_1}, // row 5:	F1 / F6			AF		
					{TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_0}, // row 6:	F1 / F7			T
					{TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_1}, // row 7:	F2 / F3			EF		
					{TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_1}, // row 8:	F2 / F4			AEF
					{TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_0}, // row 9:	F2 / F5			CE		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_X, TERNARY_0}, // row 10:	F2 / F6			ABCE
					{TERNARY_X, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_1}, // row 11:	F2 / F7			BEF
					{TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_0}, // row 12:	F3 / F4			E		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_X}, // row 13:	F3 / F5			not used
					{TERNARY_X, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_0}, // row 14:	F3 / F6			BE
					{TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_0}, // row 15:	F3 / F7			AE		
					{TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_0}, // row 16:	F4 / F5			C		
					{TERNARY_X, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_1}, // row 17:	F4 / F6			BF		
					{TERNARY_X, TERNARY_0, TERNARY_1, TERNARY_0, TERNARY_X, TERNARY_1}, // row 18:	F4 / F7			CEF
					{TERNARY_X, TERNARY_1, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_0}, // row 19:	F5 / F6			B		
					{TERNARY_X, TERNARY_X, TERNARY_X, TERNARY_0, TERNARY_X, TERNARY_0}, // row 20:	F5 / F7			ABC
					{TERNARY_X, TERNARY_0, TERNARY_0, TERNARY_0, TERNARY_X, TERNARY_0}, // row 21:	F6 / F7			A
	};

	static SEGMENT_EXT_DATA t_RxTxTable rxTable_Mode99 = 
	{	// command   A  B  C  D  E  F		        Combination		Command
					{0, 0, 0, 0, 0, 0}, // row 1:	F1 / F2		F
					{1, 1, 1, 0, 1, 1}, // row 2:	F1 / F3		ABCEF	
					{0, 0, 0, 0, 0, 0}, // row 3:	F1 / F4		CF		
					{1, 1, 1, 0, 0, 1}, // row 4:	F1 / F5		ABCF	
					{0, 0, 0, 0, 0, 0}, // row 5:	F1 / F6		AF		
					{0, 0, 0, 0, 0, 0}, // row 6:	F1 / F7		T	
					{0, 0, 0, 0, 0, 0}, // row 7:	F2 / F3		EF		
					{0, 0, 0, 0, 0, 0}, // row 8:	F2 / F4		AEF	
					{0, 0, 0, 0, 0, 0}, // row 9:	F2 / F5		CE		
					{1, 1, 1, 0, 1, 0}, // row 10:	F2 / F6		ABCE	
					{0, 0, 0, 0, 0, 0}, // row 11:	F2 / F7		BEF	
					{0, 0, 0, 0, 0, 0}, // row 12:	F3 / F4		E		
					{0, 0, 0, 0, 0, 0}, // row 13:	F3 / F5		not used	
					{0, 0, 0, 0, 0, 0}, // row 14:	F3 / F6		BE	
					{0, 0, 0, 0, 0, 0}, // row 15:	F3 / F7		AE		
					{0, 0, 0, 0, 0, 0}, // row 16:	F4 / F5		C		
					{0, 0, 0, 0, 0, 0}, // row 17:	F4 / F6		BF		
					{0, 0, 0, 0, 0, 0}, // row 18:	F4 / F7		CEF	
					{0, 0, 0, 0, 0, 0}, // row 19:	F5 / F6		B		
					{1, 1, 1, 0, 0, 0}, // row 20:	F5 / F7		ABC	
					{0, 0, 0, 0, 0, 0}, // row 21:	F6 / F7		A		
	};

//########################################################################################################
// End of NSD extended custom tables for test
//########################################################################################################
#endif


#define NSD_EXTENDED_PACKING_VERSION  0
#define NSD_EXTENDED_FILE_VERSION     0
#define NSD_EXTENDED_FILENAME         "custom.nsd"

#define TONE_SIZE    5  // number of bits to code a tone in the NSD extended packed table
#define VERSION_SIZE 3  // number of bits to code the version in the NSD extended packed table

/* LOCAL TYPE DECLARATIONS ***************************************************/


/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/
//

/* LOCAL FUNCTIONS ***********************************************************/
static BOOL putBits (unsigned long bits, unsigned long numberOfBits, 
			         unsigned long * bitPos, t_PackedNSDCustomTable table);

static BOOL getBits (unsigned long * bits, unsigned long numberOfBits, 
			         unsigned long * bitPos, t_PackedNSDCustomTable table);

#ifdef VISUALC // does not work unter Visual DSP

static unsigned long calculateChecksum(t_NSDExtCustomTableFile * pNSDExtCustomTableFile);

#endif

#ifdef DEBUG_NSD_EXTENDED_CUSTOM_TABLES

void testFunction(void);

#endif

/* IMPLEMENTATION ************************************************************/


//############################################################
//  Start of code and test code for R1.X custom tables
//############################################################

/*
void testcustom(void)
{
	unsigned long nCommands;

	nsdCustomTable_pack (TRUE,  default1UnpackedTable, packedTestTable);
	nsdCustomTable_pack (FALSE, testTable2, packedTestTable);

	nCommands = nsdCustomTable_getNCommands(testTable2);

	nn_commands = nCommands;
}
*/

//############################################################
//  End of code and test code for R1.X custom tables
//############################################################


BOOL SEGMENT_EXT_CODE nsdCustomTable_pack(BOOL                  doPack,  // TRUE: pack, FALSE: unpack
										 t_NSDCustomTable       unpacked, 
										 t_PackedNSDCustomTable packed)
{
	int rowIndex, commandIndex;
	
	int longIndex = 0;
	int bitIndex  = 0;

#ifdef DEBUG_NSD_EXTENDED_CUSTOM_TABLES

	testFunction(); // temporarily for test of NSD extended custom table

#endif

	// reset table before write to the table
	if(doPack)
	{
		// reset packed table before write to the table
		for (longIndex = 0;longIndex < PACKED_NSD_CUSTOM_TABLE_SIZE; longIndex++)
		{
			packed[longIndex] = 0;
		}
	}
	else
	{
		// reset unpacked table before write to the table
		for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++)
		{
			for (commandIndex = 0; commandIndex < COMMAND_SIZE; commandIndex++) // for all commands
			{
				unpacked[rowIndex][commandIndex] = 0;
			}
		}
	}
	//
	longIndex = 0;
	// now tables are ready to write
	//
	for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++) // for all rows
	{
		for (commandIndex = 0; commandIndex < COMMAND_SIZE; commandIndex++) // for all commands
		{
			int i;
			for (i = 0; i < TERNARY_SYMBOL_SIZE; i++) // add/remove a ternary symbol
			{
				if (PACKED_NSD_CUSTOM_TABLE_SIZE == longIndex)
				{
					return FALSE; // Table exhausted. Exit with error
				}

				if(doPack)
				{
					packed[longIndex] |= ((unpacked[rowIndex][commandIndex] >> i) & BIT0 ) << bitIndex;
				}
				else
				{
					unpacked[rowIndex][commandIndex] |= ((packed[longIndex] >> bitIndex) & BIT0)  << i;
				}

				// increment bitIndex and/or longIndex
				if (NR_OF_BITS_IN_LONG == ++bitIndex)
				{
					bitIndex = 0;
					longIndex++;
				}
			}
		}
	}
	return TRUE;
}

unsigned long SEGMENT_EXT_CODE nsdCustomTable_getNCommands(t_NSDCustomTable table)
{
	int rowIndex, commandIndex;

	unsigned long nCommands = 0;
	
	for (commandIndex = 0; commandIndex < COMMAND_SIZE; commandIndex++) // for all commands
	{
		for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++) // for all rows
		{
			if (table[rowIndex][commandIndex] != TERNARY_X) // first "X" found means that command is used
			{
				nCommands++; // increment number of commands found
				break;       // go to next command
			}

		}
	}
	return nCommands;
}

//############################################################
//  Start of code for NSD extended custom tables
//############################################################
	
BOOL SEGMENT_EXT_CODE nsdCustomTable_packEx(BOOL                     doPack,  // TRUE: pack, FALSE: unpack
										    t_NSDExtCustomTable    * unpacked, 
										    t_PackedNSDCustomTable   packed)
{
	const unsigned long UnusedTone[] = 
							{TERNARY_X, TERNARY_X, TERNARY_X, 
							 TERNARY_X, TERNARY_X, TERNARY_X};

	const unsigned long UnusedRxTableEntry[] = 
							{TERNARY_0, TERNARY_0, TERNARY_0, 
							 TERNARY_0, TERNARY_0, TERNARY_0};

	BOOL          success = TRUE;	// return value
	unsigned long bitIndex = 0;
	unsigned long unusedTones = 0;
	unsigned long nrEntries = 0;
	BOOL	      processedTxTones[NR_DUALTONES] = {FALSE}; // remembers the already processed tones for step #11

	unsigned long guardCommandIndex;
	unsigned long commandIx;
	unsigned long tone;


	if(doPack)
	{
		memset (packed, 0, sizeof(t_PackedNSDCustomTable)); // clear table

		// #1: add version
		success &= putBits (NSD_EXTENDED_PACKING_VERSION, VERSION_SIZE, &bitIndex, packed); 

		// #2: add used commands
		success &= putBits (unpacked->usedCommands, COMMAND_SIZE, &bitIndex, packed); 

		// #3: add test tone
		success &= putBits (unpacked->testTone, TONE_SIZE, &bitIndex, packed);                
		
		if (0 != unpacked->testTone) // do not pack test tone later if there is no test tone
		{
			processedTxTones[unpacked->testTone - 1] = TRUE;
		}

		// #4: add number of unused tone combinations
		for (tone = 0; tone < NR_DUALTONES; tone++) // count unused tone combinations
		{
			if (0 == memcmp(unpacked->txTable[tone], &UnusedTone, sizeof(UnusedTone)))
			{
				processedTxTones[tone] = TRUE;
				unusedTones++;
			}
		}

		success &= putBits (unusedTones, TONE_SIZE, &bitIndex, packed); // add number of unused tone combinations

		// #5:  add unused tone combinations
		for (tone = 0; tone < NR_DUALTONES; tone++)
		{
			if (0 == memcmp(unpacked->txTable[tone], &UnusedTone, sizeof(UnusedTone)))
			{
				success &= putBits (tone + 1, TONE_SIZE, &bitIndex, packed);
			}
		}
	
		// #6: count number of entries with ternary symbols or entry where rx command <> tx command
		for (tone = 0; tone < NR_DUALTONES; tone++)
		{
			if (0 != memcmp(&unpacked->rxTable[tone], &UnusedRxTableEntry, sizeof(UnusedRxTableEntry))) 
			{	// found a non-null entry in the rx table (i.e. rx command <> tx command)
				nrEntries++;  // increment and skip tx table to avoid counting the entry again in the tx table
			} 
			else
			{
				if (!processedTxTones[tone]) // exclude already processed tx tone entries
				{
					// check if at least one command is TERNARY_X ignoring unused commands
					// --> condition is "TERNARY_X and used command"
					for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) // from A to F
					{
						if ((TERNARY_X == unpacked->txTable[tone][commandIx]) && (unpacked->usedCommands & (BIT0 << commandIx)) )
						{
							nrEntries++;  // found one --> break
							break;
						}
					}
				}
			}
		}

		success &= putBits (nrEntries, TONE_SIZE, &bitIndex, packed); // add nrEntries
	
		// now, add the entries with ternary symbols or entry where rx command <> tx command
		for (tone = 0; tone < NR_DUALTONES; tone++)
		{
			BOOL bFoundTernaryEntry = FALSE;
			BOOL bFoundRxEntry      = FALSE;

			if (!processedTxTones[tone]) // exclude already processed tx tone entries
			{
				// check if at least one command is TERNARY_X 
				for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++)
				{
					if ((TERNARY_X == unpacked->txTable[tone][commandIx]) && (unpacked->usedCommands & (BIT0 << commandIx)) )
					{
						bFoundTernaryEntry = TRUE;
						break;
					}
				}

				// check if there is an entry in the rx table
				bFoundRxEntry = memcmp(&unpacked->rxTable[tone], &UnusedRxTableEntry, sizeof(UnusedRxTableEntry)); // found a non-null entry

				if (bFoundTernaryEntry || bFoundRxEntry)
				{
					// #7: add tone number
					success &= putBits (tone + 1, TONE_SIZE, &bitIndex, packed);

					// #8: add tx command
					for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) // pack the ternary command and add
					{
						// add "0" for unused commands. Will be replaced by "x" on unpacking
						unsigned long command = unpacked->usedCommands & (BIT0 << commandIx) ? unpacked->txTable[tone][commandIx] : TERNARY_0;
						success &= putBits (command, TERNARY_SYMBOL_SIZE, &bitIndex, packed);
					}
				
					if (bFoundRxEntry)
					{
						// #9: add "1" = TRUE
						success &= putBits (1, 1, &bitIndex, packed);

						// #10: add rx command
						for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) // pack the binary command and add
						{
							success &= putBits (unpacked->rxTable[tone][commandIx], 1, &bitIndex, packed);
						}
					}
					else
					{
						// #9: add "0" = FALSE
						success &= putBits (0, 1, &bitIndex, packed);
					}

					processedTxTones[tone] = TRUE;
				}
			}
		}

		// #11: now, add the remaining entries
		for (tone = 0; tone < NR_DUALTONES; tone++)
		{
			if (!processedTxTones[tone])
			{
				for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) // pack the binary command and add
				{
					// add "0" for unused commands. Will be replaced by "x" on unpacking
					unsigned long command = unpacked->usedCommands & (BIT0 << commandIx) ? unpacked->txTable[tone][commandIx] : 0;
					success &= putBits (command, 1, &bitIndex, packed);
				}
			}
		}

		// #12: add the number of guard commands
		success &= putBits (unpacked->nrOfGuardCommands, TONE_SIZE, &bitIndex, packed);

		// #13: add the guard commands
		for (guardCommandIndex = 0; guardCommandIndex < unpacked->nrOfGuardCommands; guardCommandIndex++)
		{
			for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) // pack the ternary command and add
			{
				success &= putBits (unpacked->guardCommands[guardCommandIndex][commandIx], TERNARY_SYMBOL_SIZE, &bitIndex, packed);
			}
		}
	}
	else // unpack
	{
		unsigned long ix, version;

		memset (unpacked, 0, sizeof(t_NSDExtCustomTable)); // clear table

		// #1: get version
		success &= getBits (&version, VERSION_SIZE, &bitIndex, packed); 

		if (NSD_EXTENDED_PACKING_VERSION != version)
		{
			return FALSE;
		}

		// #2: get used commands
		success &= getBits (&unpacked->usedCommands, COMMAND_SIZE, &bitIndex, packed); 

		// #3: get test tone
		success &= getBits (&unpacked->testTone, TONE_SIZE, &bitIndex, packed);                

		if (0 != unpacked->testTone) // do not unpack test tone later if there is no test tone
		{
			processedTxTones[unpacked->testTone - 1] = TRUE;
		}

		// #4: get number of unused tone combinations
		success &= getBits (&unusedTones,   TONE_SIZE, &bitIndex, packed); 

		// #5:  get unused tone combinations
		for (ix = 0; ix < unusedTones; ix++)
		{
			// get the tone
			success &= getBits (&tone,   TONE_SIZE, &bitIndex, packed);

			--tone; // we use the range 0..20 for tones 1..21

			memcpy (&unpacked->txTable[tone], &UnusedTone, sizeof(UnusedTone)); // enter a "XXXXXX" in tx table

			processedTxTones[tone] = TRUE;
		}

		// #6: get the number of entries with ternary symbols or entry where rx command <> tx command
		success &= getBits (&nrEntries,   TONE_SIZE, &bitIndex, packed);

		// now, get the entries with ternary symbols or entry where rx command <> tx command
		for (ix = 0; ix < nrEntries; ix++)
		{
			BOOL bHasRxTableEntry;

			// #7: get tone number
			success &= getBits (&tone,   TONE_SIZE, &bitIndex, packed);
			
			--tone; // we use the range 0..20 for tones 1..21

			// #8: get the ternary tx command and store it in the tx table
			for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) 
			{
				success &= getBits (&unpacked->txTable[tone][commandIx], TERNARY_SYMBOL_SIZE, &bitIndex, packed);
		
				if (!(unpacked->usedCommands & (BIT0 << commandIx))) // replace "0" by "X" for NOT used commands
				{
					unpacked->txTable[tone][commandIx] = TERNARY_X;
				}
			}

			// #9: get the flag
			success &= getBits (&bHasRxTableEntry, 1, &bitIndex, packed);

			if(bHasRxTableEntry)
			{
				// #10: get binary rx command
				for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) 
				{
					success &= getBits (&unpacked->rxTable[tone][commandIx], 1, &bitIndex, packed);
				}
			}

			processedTxTones[tone] = TRUE;
		}

		// #11: now, get the remaining entries
		for (tone = 0; tone < NR_DUALTONES; tone++)
		{
			if(!processedTxTones[tone])
			{
				// get the binary tx command
				for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++) 
				{
					success &= getBits (&unpacked->txTable[tone][commandIx], 1, &bitIndex, packed);

					if (!(unpacked->usedCommands & (BIT0 << commandIx))) // replace "0" by "X" for unused commands
					{
						unpacked->txTable[tone][commandIx] = TERNARY_X;
					}

				}
			}
		}

		// #12: get the number of guard commands
		success &= getBits (&unpacked->nrOfGuardCommands, TONE_SIZE, &bitIndex, packed);

		// #13: get the ternary guard commands
		for (guardCommandIndex = 0; guardCommandIndex < unpacked->nrOfGuardCommands; guardCommandIndex++)
		{
			for (commandIx = 0; commandIx < COMMAND_SIZE; commandIx++)
			{
				success &= getBits (&unpacked->guardCommands[guardCommandIndex][commandIx], TERNARY_SYMBOL_SIZE, &bitIndex, packed);
			}
		}
	}

	return success;
}


static BOOL SEGMENT_EXT_CODE putBits (unsigned long bits, unsigned long numberOfBits, 
			                          unsigned long * bitPos, t_PackedNSDCustomTable table)
{
	unsigned long i;

	if (numberOfBits > NR_OF_BITS_IN_LONG)
		return FALSE;

	for (i = 0; i < numberOfBits; i++, (*bitPos)++)
	{
		if (*bitPos > NR_OF_BITS_IN_LONG * (sizeof(t_PackedNSDCustomTable) / sizeof(unsigned long)))
		{
			return FALSE;
		}
		else
		{
			unsigned long longIx = *bitPos / NR_OF_BITS_IN_LONG; // get index to long array
			unsigned long bitIx  = *bitPos % NR_OF_BITS_IN_LONG; // get bit index
			
			if (bits & (BIT0<<i)) // if bit is set, set it in table
			{
				table [longIx] |= (BIT0<<bitIx);
			}
		}
	}
	return TRUE;
}


static BOOL SEGMENT_EXT_CODE getBits (unsigned long * bits, unsigned long numberOfBits, 
			                          unsigned long * bitPos, t_PackedNSDCustomTable table)
{
	unsigned long i; //  bit index in "bits"

	if (numberOfBits > NR_OF_BITS_IN_LONG)
		return FALSE;

	*bits = 0; // clear bits

	for (i = 0; i < numberOfBits; i++, (*bitPos)++)
	{
		if (*bitPos > NR_OF_BITS_IN_LONG * (sizeof(t_PackedNSDCustomTable) / sizeof(unsigned long)))
		{
			return FALSE;
		}
		else
		{
			unsigned long longIx = *bitPos / NR_OF_BITS_IN_LONG; // get index to long array
			unsigned long bitIx  = *bitPos % NR_OF_BITS_IN_LONG; // get bit index

			BOOL theBit = table [longIx] & (BIT0<<bitIx); // get the bit from the table

			if (theBit)
			{
				*bits |= BIT0<<i;
			}
		}
	}
	return TRUE;
}


//########################################################################################################
// NSD extended custom tables code for test
//########################################################################################################

#ifdef DEBUG_NSD_EXTENDED_CUSTOM_TABLES

void testFunction(void)
{
	BOOL success;

	unsigned int entryIx;

	t_PackedNSDCustomTable     packed;

	t_NSDExtCustomTable        unpackedOut;
	t_NSDExtCustomTable      * pUnpackedIn;
	t_NSDExtCustomTable      * pUnpackedOut = &unpackedOut;

	t_NSDExtCustomTableFile    NSDExtCustomTableFile;

	success = nsdCustomTable_readCustomTableFile(&NSDExtCustomTableFile);
	assert(success);

	for (entryIx = 0; entryIx < 4; entryIx++) // 4 modes (1,2,3,99)
	{

	// test with mode 1
		pUnpackedIn = &NSDExtCustomTableFile.entries[entryIx].customTable;

		success = nsdCustomTable_packEx(TRUE,  pUnpackedIn, packed); // pack it 
		assert(success);

		success = nsdCustomTable_packEx(FALSE, pUnpackedOut,packed);  // and unpack it again
		assert(success);

		success = (0 == memcmp(pUnpackedIn, pUnpackedOut, sizeof(t_NSDExtCustomTable) )); // compare the tables

		if (!success) // debug support: located first delta in long values
		{
			unsigned short ix;
			for (ix = 0; ix < (sizeof(t_NSDExtCustomTable) / sizeof(unsigned long)); ix++)
			{
				int xx=0;
				if ( ((unsigned long *)pUnpackedIn)[ix] != ((unsigned long *)pUnpackedOut)[ix])
				{
					xx++;
				}
			}
		
		}


		assert(success);
	}

	success = success;
}

// Returns a selected custom table. Memory is provided by the caller

BOOL SEGMENT_EXT_CODE nsdCustomTable_getNSDextCustomTableForTest(unsigned long id, t_NSDExtCustomTable * tb)
{
	memset(tb, 0, sizeof(*tb));

	switch (id)
	{
		case 1:
		{
	    			 tb->usedCommands = BIT5|BIT4|BIT3|BIT2|BIT1|BIT0; // F-A
	    			 tb->testTone = 13;
			memcpy (&tb->txTable, &txTable_Mode1, sizeof (t_RxTxTable));
	    			 tb->nrOfGuardCommands = 0;
		}
		break;

		case 2:
		{
	    			 tb->usedCommands = BIT5|BIT4|BIT3|BIT2|BIT1|BIT0; // F-A
	    			 tb->testTone = 6;
			memcpy (&tb->txTable, &txTable_Mode2, sizeof (t_RxTxTable));
			memcpy (&tb->rxTable, &rxTable_Mode2, sizeof (t_RxTxTable));
	    			 tb->nrOfGuardCommands = 0;
		}
		break;

		case 3:
		{
	    			 tb->usedCommands = BIT5|BIT4|BIT3|BIT2|BIT1|BIT0; // F-A
	    			 tb->testTone = 6;
			memcpy (&tb->txTable, &txTable_Mode3, sizeof (t_RxTxTable));
			memcpy (&tb->rxTable, &rxTable_Mode3, sizeof (t_RxTxTable));

	    			 tb->nrOfGuardCommands = 1;  // = xxx1xx
					 tb->guardCommands[0][0]  = TERNARY_X; // A
					 tb->guardCommands[0][1]  = TERNARY_X; // B
					 tb->guardCommands[0][2]  = TERNARY_X; // C
					 tb->guardCommands[0][3]  = TERNARY_1; // D
					 tb->guardCommands[0][4]  = TERNARY_X; // E
					 tb->guardCommands[0][5]  = TERNARY_X; // F
		}
		break;

		case 99: // test table without commands A and E. Otherwise like Mode 3
		{
	    			 tb->usedCommands = BIT5|/*BIT4|*/BIT3|BIT2|BIT1/*|BIT0*/; // F,D,C,B
	    			 tb->testTone = 6;
			memcpy (&tb->txTable, &txTable_Mode99, sizeof (t_RxTxTable));
			memcpy (&tb->rxTable, &rxTable_Mode99, sizeof (t_RxTxTable));

	    			 tb->nrOfGuardCommands = 1;  // = xxx1xx
					 tb->guardCommands[0][0]  = TERNARY_X; // A
					 tb->guardCommands[0][1]  = TERNARY_X; // B
					 tb->guardCommands[0][2]  = TERNARY_X; // C
					 tb->guardCommands[0][3]  = TERNARY_1; // D
					 tb->guardCommands[0][4]  = TERNARY_X; // E
					 tb->guardCommands[0][5]  = TERNARY_X; // F
		}
		break;

		default: return FALSE;
	}

	return TRUE;
}

#endif // DEBUG_NSD_EXTENDED_CUSTOM_TABLES


BOOL SEGMENT_EXT_CODE nsdCustomTable_readCustomTableFile (t_NSDExtCustomTableFile * pNSDExtCustomTableFile)
{
	BOOL success     = TRUE;  // initialization needed if compiled without compiler switches

	//############################################################
	//  This code creates a custom table in memory 
	// (for test and custom table file creation)
	//############################################################

#ifdef DEBUG_NSD_EXTENDED_CUSTOM_TABLES

	t_NSDExtCustomTableFile   NSDExtCustomTableFile_ForTest;
	t_NSDExtCustomTableFile * pNSDExtCustomTableFile_ForTest = &NSDExtCustomTableFile_ForTest;


	t_NSDExtCustomTable nsdExtCustomTable;

	t_TPEApplication defaultApplication_Mode1  = {PERMISSIVE,  PERMISSIVE, PERMISSIVE, PERMISSIVE, DIRECT,      DIRECT};
	t_TPEApplication defaultApplication_Mode2  = {PERMISSIVE,  PERMISSIVE, PERMISSIVE, PERMISSIVE, DIRECT,      DIRECT}; 
	t_TPEApplication defaultApplication_Mode3  = {PERMISSIVE,  PERMISSIVE, PERMISSIVE, PERMISSIVE, DIRECT,      DIRECT};
	t_TPEApplication defaultApplication_Mode99 = {COMMAND_OFF, PERMISSIVE, PERMISSIVE, PERMISSIVE, COMMAND_OFF, DIRECT}; //A and E and off

	memset(pNSDExtCustomTableFile_ForTest, 0, sizeof(*pNSDExtCustomTableFile_ForTest));

	pNSDExtCustomTableFile_ForTest->version = NSD_EXTENDED_FILE_VERSION;

	pNSDExtCustomTableFile_ForTest->nrOfEntries = 4;
	pNSDExtCustomTableFile_ForTest->version     = 0;

	// ******************************************************************
	// Mode 1
	// ******************************************************************
	         pNSDExtCustomTableFile_ForTest->entries[0].id     = 1;
	strcpy(  pNSDExtCustomTableFile_ForTest->entries[0].name,    "Mode 1");

	memcpy (&pNSDExtCustomTableFile_ForTest->entries[0].defaultApplication, &defaultApplication_Mode1, sizeof (t_TPEApplication));


	success = nsdCustomTable_getNSDextCustomTableForTest(1, &nsdExtCustomTable);
	assert(success);
	memcpy(&pNSDExtCustomTableFile_ForTest->entries[0].customTable, &nsdExtCustomTable, sizeof(nsdExtCustomTable));

	// ******************************************************************
	// Mode 2
	// ******************************************************************
	         pNSDExtCustomTableFile_ForTest->entries[1].id     = 2;
	strcpy(  pNSDExtCustomTableFile_ForTest->entries[1].name,    "Mode 2");

	memcpy (&pNSDExtCustomTableFile_ForTest->entries[1].defaultApplication, &defaultApplication_Mode2, sizeof (t_TPEApplication));

	success = nsdCustomTable_getNSDextCustomTableForTest(2, &nsdExtCustomTable);
	assert(success);
	memcpy(&pNSDExtCustomTableFile_ForTest->entries[1].customTable, &nsdExtCustomTable, sizeof(nsdExtCustomTable));

	// ******************************************************************
	// Mode 3
	// ******************************************************************
	         pNSDExtCustomTableFile_ForTest->entries[2].id     = 3;
	strcpy(  pNSDExtCustomTableFile_ForTest->entries[2].name,    "Mode 3");

	memcpy (&pNSDExtCustomTableFile_ForTest->entries[2].defaultApplication, &defaultApplication_Mode3, sizeof (t_TPEApplication));

	success = nsdCustomTable_getNSDextCustomTableForTest(3, &nsdExtCustomTable);
	assert(success);
	memcpy(&pNSDExtCustomTableFile_ForTest->entries[2].customTable, &nsdExtCustomTable, sizeof(nsdExtCustomTable));

	// ******************************************************************
	// Mode 99
	// ******************************************************************
	         pNSDExtCustomTableFile_ForTest->entries[3].id     = 99;
	strcpy(  pNSDExtCustomTableFile_ForTest->entries[3].name,    "Mode 99");

	memcpy (&pNSDExtCustomTableFile_ForTest->entries[3].defaultApplication, &defaultApplication_Mode99, sizeof (t_TPEApplication));

	success = nsdCustomTable_getNSDextCustomTableForTest(99, &nsdExtCustomTable);
	assert(success);
	memcpy(&pNSDExtCustomTableFile_ForTest->entries[3].customTable, &nsdExtCustomTable, sizeof(nsdExtCustomTable));


#ifdef VISUALC // does not work unter Visual DSP

	//############################################################
	//  This code creates a custom table file "custom.nsd" from
	//  the previously created custom tables in memory
	// (for custom table file creation)
	//############################################################
	{
		unsigned long checksum = calculateChecksum(pNSDExtCustomTableFile_ForTest);

		FILE * stream = fopen(NSD_EXTENDED_FILENAME , "wb" ); // open file for writing in binary mode;
		
		success = (NULL != stream); 
	
		// write the version
		if (success)
		{
			success = (1 == fwrite(&pNSDExtCustomTableFile_ForTest->version, sizeof(unsigned long), 1, stream)); // write one item
		}

		// write the number of entries
		if (success)
		{
			success = (1 == fwrite(&pNSDExtCustomTableFile_ForTest->nrOfEntries, sizeof(unsigned long), 1, stream)); // write one item
		}

		// write the file
		if (success)
		{
			success = (1 == fwrite(pNSDExtCustomTableFile_ForTest->entries, 
				                    pNSDExtCustomTableFile_ForTest->nrOfEntries * sizeof(t_NSDExtCustomTableFileEntry), 
									1, 
									stream)); // write one item
		}

		// write checksum
		if (success)
		{
			success = (1 == fwrite(&checksum, sizeof(unsigned long), 1, stream)); // write one item
		}

		if (success)
		{
			success = (0 == fclose(stream));
		}
	}
#endif  // VISUALC

#endif  // DEBUG_NSD_EXTENDED_CUSTOM_TABLES

	//############################################################
	//  Read the custom table file "custom.nsd"
	//############################################################

#ifdef VISUALC // does not work unter Visual DSP
	{
		unsigned long checksum;

		// open file for read in binary mode
		FILE * stream = fopen(NSD_EXTENDED_FILENAME, "rb" );
		
		success = (NULL != stream); 

		if (success) 
		{
			memset(pNSDExtCustomTableFile, 0, sizeof(*pNSDExtCustomTableFile)); // zero out the table

			// read the file version
			success = (1 == fread(&pNSDExtCustomTableFile->version, sizeof(unsigned long), 1, stream));
		}

		if (success)
		{
			success = (NSD_EXTENDED_FILE_VERSION == pNSDExtCustomTableFile->version); // only version "NSD_EXTENDED_FILE_VERSION" supported
		}
		
		if (success)
		{
			// read the nrOfEntries
			success = (1 == fread(&pNSDExtCustomTableFile->nrOfEntries, sizeof(unsigned long), 1, stream)); // read one item
		}

		if (success)
		{
			success = (1 == fread(pNSDExtCustomTableFile->entries, 
					              pNSDExtCustomTableFile->nrOfEntries * sizeof(t_NSDExtCustomTableFileEntry), 
								  1, 
								  stream)); // read one item
		}

		if (success) // read checksum
		{
			success = (1 == fread(&checksum, 
					              sizeof(unsigned long), 
								  1, 
								  stream)); // read one item
		}

		if (success) // verify checksum
		{
			success = (checksum == calculateChecksum(pNSDExtCustomTableFile));
		}

		if (success)
		{
			success = (0 == fclose(stream));
		}
	}

#endif	// VISUALC

	return success;
}

#ifdef VISUALC // does not work unter Visual DSP

static unsigned long SEGMENT_EXT_CODE calculateChecksum(t_NSDExtCustomTableFile * pNSDExtCustomTableFile)
{

	unsigned long * ptr = (unsigned long *) pNSDExtCustomTableFile;
	unsigned long i;
	unsigned long checksum = 0;
	unsigned long sizeInLongs = ( 2 * sizeof(unsigned long) + // size of version and nrOfEntries fields
						          pNSDExtCustomTableFile->nrOfEntries * sizeof(t_NSDExtCustomTableFileEntry)
						        ) / sizeof(unsigned long);

	for (i = 0; i < sizeInLongs; i++)
	{
		checksum += ptr[i];
	}

	return checksum;
}

#endif

