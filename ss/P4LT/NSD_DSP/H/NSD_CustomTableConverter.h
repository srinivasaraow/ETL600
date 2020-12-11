                                                                             /*-------------------------------------------------------------------------*
 * $Workfile: NSD_CustomTableConverter.h $
 * Part of	: ETL600 / P4LT
 * Language	: C
 * Created by	: P. Plüer
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/NSD_DSP/H/NSD_CustomTableConverter.h $
 * 
 * 8     27.11.06 10:48 Chpeplu
 * In function CreateTransmittFreqTable() additional call parameter (for
 * delete Cmd) added
 * 
 * 7     24.11.06 11:11 Chpeplu
 * New functions CreateRxCmdTable_ext() and CreateTransmittFreqTable()
 * added
 * 
 * 3     10.03.05 17:52 Chpeplu
 * Type of parameter for the function GenerateTxFreqTable()  changed
 * 
 * 2     9.03.05 16:18 Chpeplu
 * New function GenerateTxFreqTable() added
 * 
 * 1     9.03.05 13:55 Chpeplu
 *-------------------------------------------------------------------------*/
 
#ifndef NSD_CUSTOM_TABLE_CONVERTER_H
#define NSD_CUSTOM_TABLE_CONVERTER_H

/* INCLUDE FILES **************************************************************/
#include "nsdCustomTable.h"

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

#define NO_OF_TONES_PER_COMMAND				2
// @Pl_ext
#define START_1				0
#define STOP_1				1
#define MASK_1				2
// 
#define SEND_GUARD			1

#define NOT_USED_FREQUENCY_COMBINATION		0x0000ffff

/* PUBLIC TYPE DECLARATIONS ***************************************************/
typedef unsigned long t_Freq2CustomTable[NSD_CUSTOM_TABLE_SIZE][NO_OF_TONES_PER_COMMAND];

typedef unsigned long t_Custom2CommandTable[NSD_CUSTOM_TABLE_SIZE];

typedef unsigned long t_Ternary_XBitValue[NSD_CUSTOM_TABLE_SIZE][2];
/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/
void SEGMENT_EXT_CODE ConvertCustom2CommandTable(	t_NSDCustomTable		table, 
													t_Custom2CommandTable	table_C2Cmd);

void SEGMENT_EXT_CODE GenerateDetectorCodeTable(	t_Custom2CommandTable	table_C2Cmd, 
													unsigned long			*ptr_DetectorCodeTable);

void SEGMENT_EXT_CODE GenerateTxPriorityTable (	t_NSDCustomTable		table, 
												t_Custom2CommandTable	table_C2Cmd,
												unsigned long			*ptr_Command_Table);
void SEGMENT_EXT_CODE GenerateTxFreqTable(	t_Custom2CommandTable	table_C2Cmd, 
											unsigned long			*ptr_Command_Table,
											unsigned long			*ptr_txFreq_Table);
// @Pl_ext
void SEGMENT_EXT_CODE CreateRxCmdTable_ext(unsigned long *p_RxDetectorTableExt, t_NSDExtCustomTable *p_localCustomTable);
void SEGMENT_EXT_CODE CreateTransmittFreqTable(unsigned long *p_TransmitTable, unsigned long *p_Delete_Cmd_Table, t_NSDExtCustomTable *p_localCustomTable);
#endif

