/*-------------------------------------------------------------------------*
 * $Workfile: nsdCustomTable.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2005
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source R4/HMI600/H/nsdCustomTable.H $
 * 
 * 24    24.06.11 9:37 Chthbae
 * 
 * 23    22.06.11 15:55 Chthbae
 * 
 * 19    27.11.06 19:20 Chchlee
 * Checksum added. Version in unpacked custom table removed
 * 
 * 18    16.11.06 16:22 Chchlee
 * 
 * 17    15.11.06 15:49 Chchlee
 * 
 * 16    14.11.06 16:16 Chchlee
 * code for generation of custom.nsd added
 * 
 * 15    14.11.06 10:50 Chmaflu
 * make C Functions to Extern "C" functions
 * wrap ifdef arround these functions
 * 
 * 14    14.11.06 9:57 Chchlee
 * return value "BOOL" added to "nsdCustomTable_readCustomTableFile"
 * 
 * 13    9.11.06 7:54 Chchlee
 * packer of nsdCustomTable_packEx implemented
 * 
 * 12    8.11.06 9:26 Chchlee
 * First test version with NSD extended custom table
 * 
 * 8     11.03.05 16:25 Chpeplu
 * new function void testcustom(void) added for tests only
 * 
 * 7     9.03.05 17:19 Chpeplu
 * define MAX_COMMAND_COMBINATION changed
 * 
 * 6     9.03.05 16:17 Chpeplu
 * New define MAX_COMMAND_COMBINATION added
 * 
 * 5     9.03.05 14:02 Chpeplu
 * Some typdefs and function declarations moved to
 * NSD_CustomTabelConverter.h
 * 
 * 4     8.03.05 17:30 Chpeplu
 * typedef for t_Ternary_XBitValue added,
 * function GenerateTxPriorityTable added
 * 
 * 3     8.03.05 7:21 Chpeplu
 * Some defines and to functions ConvertCustom2CommandTable() and
 * GenerateDetectorCodeTable() added
 * 
 * 2     21.02.05 19:23 Chchlee
 * nsdCustomTable_getNCommands added
 * 
 * 1     21.02.05 9:39 Chchlee
 *-------------------------------------------------------------------------*/

#ifndef NSD_CUSTOM_TABLE_H
#define NSD_CUSTOM_TABLE_H

/* INCLUDE FILES **************************************************************/
#include "basetype.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/
#define COMMAND_SIZE				6			// number of bits needed for commands A-F
// MAX_COMMAND_CO5MBINATION = pow(2,COMMAND_SIZE)
#define MAX_COMMAND_COMBINATION		(64)



#define TERNARY_0            0
#define TERNARY_1            1
#define TERNARY_X            2

#define TERNARY_SYMBOL_SIZE  2  // number of bits needed for a ternary symbol

#define  NR_DUALTONES        21      // for 7 command frequencies  
#define  NSD_CUSTOM_TABLE_SIZE NR_DUALTONES

// Yields a size of 8 longs
#define PACKED_NSD_CUSTOM_TABLE_SIZE \
	  (( (NSD_CUSTOM_TABLE_SIZE * TERNARY_SYMBOL_SIZE * COMMAND_SIZE) + (NR_OF_BITS_IN_LONG-1) ) / NR_OF_BITS_IN_LONG)

#define NR_OF_CUSTOM_TABLES       50

/* PUBLIC TYPE DECLARATIONS ***************************************************/

// packed custom table
typedef unsigned long t_PackedNSDCustomTable[PACKED_NSD_CUSTOM_TABLE_SIZE];

// #########################################
// ##          R1.x custom table          ##
// #########################################

// each command has ternary symbol value
typedef unsigned long t_NSDCustomTable[NSD_CUSTOM_TABLE_SIZE][COMMAND_SIZE];

// #########################################
// ##    NSD extended  custom table       ##
// #########################################

#define NR_GUARD_COMMANDS          32 // maximum that will ever be needed

typedef unsigned long t_RxTxTable    [NR_DUALTONES]      [COMMAND_SIZE];
typedef unsigned long t_GuardCommands[NR_GUARD_COMMANDS] [COMMAND_SIZE];

typedef struct {
		unsigned long   usedCommands;       // bit field: A-F corresponding to bit 0..5
		unsigned long   testTone;           // [1;21]
		t_RxTxTable     txTable;            // ternary
		t_RxTxTable     rxTable;            // binary 
		unsigned long   nrOfGuardCommands;  // [0; NR_GUARD_COMMANDS]
		t_GuardCommands guardCommands;      // ternary
}t_NSDExtCustomTable;


// #########################################
// ##    file format of "custom.nsd"      ##
// #########################################

typedef unsigned long t_TPEApplication[COMMAND_SIZE]; // {COMMAND_OFF|BLOCKING|PERMISSIVE|DIRECT} 

typedef struct {
		unsigned long         id;
		char                  name[40]; // space for 39 characters and 0 zero delimiter
		t_TPEApplication      defaultApplication; 
		t_NSDExtCustomTable   customTable;

}t_NSDExtCustomTableFileEntry;

typedef struct {
		unsigned long                   version;
		unsigned long                   nrOfEntries;
		t_NSDExtCustomTableFileEntry    entries[NR_OF_CUSTOM_TABLES];
}t_NSDExtCustomTableFile;
// At the end of the file there is 32-bit checksum which is the sum of all previous long words


/* PUBLIC MACRO DEFINITIONS ***************************************************/


/* PUBLIC FUNCTIONS ***********************************************************/

// #############################################
// functions for R1.X custom table
// #############################################
void testcustom(void);

BOOL nsdCustomTable_pack(BOOL                   doPack,    // TRUE: pack, FALSE: unpack
						 t_NSDCustomTable       unpacked, 
						 t_PackedNSDCustomTable packed);

unsigned long nsdCustomTable_getNCommands(t_NSDCustomTable table);

// #############################################
// functions for NSD extended custom table
// #############################################


#ifdef __cplusplus
	extern "C" BOOL nsdCustomTable_packEx(BOOL                     doPack,  // TRUE: pack, FALSE: unpack
	 		                              t_NSDExtCustomTable    * unpacked, 
							              t_PackedNSDCustomTable   packed);
#else
	BOOL nsdCustomTable_packEx(BOOL                     doPack,  // TRUE: pack, FALSE: unpack
	 						   t_NSDExtCustomTable    * unpacked, 
							   t_PackedNSDCustomTable   packed);
#endif

//###############################################
// NSD extended custom tables code for test
//###############################################

#ifdef __cplusplus
	extern "C" BOOL nsdCustomTable_readCustomTableFile (t_NSDExtCustomTableFile * pNSDExtCustomTableFile);
	extern "C" BOOL nsdCustomTable_getNSDextCustomTableForTest(unsigned long id, t_NSDExtCustomTable * tb);
#else
	BOOL nsdCustomTable_readCustomTableFile (t_NSDExtCustomTableFile * pNSDExtCustomTableFile);
	BOOL nsdCustomTable_getNSDextCustomTableForTest(unsigned long id, t_NSDExtCustomTable * tb);
#endif


#endif
