/*-------------------------------------------------------------------------*
 * $Workfile: NSD_CustomTableConverter.c $
 * Part of	: ETL600 / P4LT
 * Language	: C
 * Created by: P. Plüer
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source AES/P4LT/NSD_DSP/C/NSD_CustomTableConverter.c $
 * 
 * 12    27.11.06 14:11 Chpeplu
 * Function CreateTransmittFreqTable()  corrected!
 * 
 * 11    27.11.06 10:49 Chpeplu
 * In function CreateTransmittFreqTable() additional call parameter (for
 * delete Cmd) added
 * 
 * 10    24.11.06 12:58 Chpeplu
 * New functions to handle new custom tables for NSD600 extended  added
 * 
 * 9     14.11.06 13:30 Chpeplu
 * Use RX_COM_T anstelle von COM_T in der Funktion
 * ConvertCustom2CommandTable(), Use TX_COM_T anstelle von COM_T in der
 * Funktion GenerateTxPriorityTable(), Function Init_Cyclic_LT() changed
 * for custom mode
 * 
 * 5     15.03.05 17:38 Chpeplu
 * function GenerateTxFreqTable() corrected
 * 
 * 4     14.03.05 10:36 Chpeplu
 * function GenerateTxFreqTable() corrected for Loop test 
 * 
 * 3     10.03.05 17:52 Chpeplu
 * Type of parameter for the function GenerateTxFreqTable()  changed
 * 
 * 2     9.03.05 16:18 Chpeplu
 * New function GenerateTxFreqTable() added
 * 
 * 1     9.03.05 13:55 Chpeplu
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include "NSD_CustomTableConverter.h"
#include "NSD_DSP_Types.h"
#include "nsdCustomTable.H"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/
t_Freq2CustomTable	MapFrequency2CustomTable = 
{
//	freq_1		freq_2
	{F_1,		F_2},			// row 1
	{F_1,		F_3},			// row 2
	{F_1,		F_4},			// row 3
	{F_1,		F_5},			// row 4
	{F_1,		F_6},			// row 5
	{F_1,		F_7},			// row 6
	{F_2,		F_3},			// row 7
	{F_2,		F_4},			// row 8
	{F_2,		F_5},			// row 9
	{F_2,		F_6},			// row 10
	{F_2,		F_7},			// row 11
	{F_3,		F_4},			// row 12
	{F_3,		F_5},			// row 13
	{F_3,		F_6},			// row 14
	{F_3,		F_7},			// row 15
	{F_4,		F_5},			// row 16
	{F_4,		F_6},			// row 17
	{F_4,		F_7},			// row 18
	{F_5,		F_6},			// row 19
	{F_5,		F_7},			// row 20
	{F_6,		F_7}			// row 21
};
/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

t_Ternary_XBitValue Ternary_XBit;
// @Pl_ext
SEGMENT_EXT_DATA t_NSDExtCustomTable localCustomTable;

/* LOCAL FUNCTIONS ***********************************************************/
// @Pl_ext
void SEGMENT_EXT_CODE createTernary_x_Ternary_0_CountTable(unsigned long		*p_Ternary_0_Table, 
														   unsigned long		*p_Ternary_x_Table, 
														   t_NSDExtCustomTable	*localCustomTable);
unsigned long SEGMENT_EXT_CODE count_nr_of_Commands(unsigned long usedCommands);
void SEGMENT_EXT_CODE convert_rxTable(unsigned long *p_RxCmdTable, t_NSDExtCustomTable *localCustomTable);

/* IMPLEMENTATION ************************************************************/
 
//
/*****************************************************************************
  Function:		ConvertCustom2CommandTable(t_NSDCustomTable table)
  Description:	Map costum table to command table
  Inputs:		None
  Outputs:		None
  Return:		
  Globals:		
  Important:	

******************************************************************************/
//
void SEGMENT_EXT_CODE ConvertCustom2CommandTable(t_NSDCustomTable		table, 
												 t_Custom2CommandTable	table_C2Cmd)
{
	int rowIndex, commandIndex;
	int Ternary_X_counter = 0;

	unsigned long Command = 0, N_Commands = 0;

	N_Commands = nsdCustomTable_getNCommands(table);
	
	for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++) // for all rows
	//for (commandIndex = 0; commandIndex < COMMAND_SIZE; commandIndex++) // for all commands
	{
		Command = 0;
		Ternary_X_counter = 0;

		//for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++)	// for all rows
		for (commandIndex = 0; commandIndex < N_Commands; commandIndex++)	// for all used commands
		{

			if (table[rowIndex][commandIndex] != TERNARY_X) // "X" found means that frequency combination is not used
			{
				// frequency combination is used
				if(table[rowIndex][commandIndex] == TERNARY_1) // command is used
				{
					// command is used
					Command |= (BIT0<<commandIndex); 
				}
				else
				{
					// 
				}
			}
			else
			{
				// frequency combination not used
				//Command = NOT_USED_FREQUENCY_COMBINATION;
				//break;	// go to next row
				Ternary_X_counter++;
			}
			// go to next command
		}
		// test for loop test command
		// @Pl_ext
		if(Command == 0) Command = RX_COM_T;
		//
		if(Ternary_X_counter == N_Commands) Command = NOT_USED_FREQUENCY_COMBINATION;
		//
		table_C2Cmd[rowIndex] = Command;
		// go to next row
	}	
}
//
/*****************************************************************************
  Function:		GenerateDetectorCodeTable(t_Custom2CommandTable	table_C2Cmd,
											unsigned long	*ptr_DetectorCodeTable	)
  Description:	Map custom table to Ch1_COMMAND_Table_NSD format (see NSD_Detector.c)
  Inputs:		None
  Outputs:		None
  Return:		
  Globals:		
  Important:	

******************************************************************************/
//
void SEGMENT_EXT_CODE GenerateDetectorCodeTable(t_Custom2CommandTable	table_C2Cmd, 
												unsigned long			*ptr_DetectorCodeTable)
{

	unsigned long *ptr_temp;
	int rowIndex, temp;
	unsigned long command;
	unsigned long freq1, freq2;
	int AppIndex;


	// **************************************************************
	// *** Reset Detector code table before write the custom data ***
	// **************************************************************
	//
	ptr_temp = ptr_DetectorCodeTable;	// save pointer to Detector Code table

	for (temp=0; temp < NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2*NUMBER_OF_RX_DET_TONES*2; temp++)
	{
		*ptr_temp++ = 0x00000000;
	}
	//
	//
	for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++)
	{
		// get command and frequencies for rowIndex
		command = table_C2Cmd[rowIndex];				// get command from table
		// 
		if(command == NOT_USED_FREQUENCY_COMBINATION) command = 0x00000000;
		//
		freq1 =	MapFrequency2CustomTable[rowIndex][0];	// get freq 1
		freq2 = MapFrequency2CustomTable[rowIndex][1];	// get freq 2
		//
		for(AppIndex = 0; AppIndex < NUMBER_OF_APPLICATION_MODES; AppIndex++)
		{
			*(ptr_DetectorCodeTable + freq1*NUMBER_OF_APPLICATION_MODES*NUMBER_OF_RX_DET_TONES*2 + freq2*NUMBER_OF_APPLICATION_MODES + AppIndex) = command;	
			*(ptr_DetectorCodeTable + freq2*NUMBER_OF_APPLICATION_MODES*NUMBER_OF_RX_DET_TONES*2 + freq1*NUMBER_OF_APPLICATION_MODES + AppIndex) = command;
		}

	}

}

/*****************************************************************************
  Function:		GenerateTxPriorityTable(t_NSDCustomTable		table, 
										t_Custom2CommandTable	table_C2Cmd,
										unsigned long			*ptr_Command_Table)
  Description:	
  Inputs:		None
  Outputs:		None
  Return:		
  Globals:		
  Important:	

******************************************************************************/
//
void SEGMENT_EXT_CODE GenerateTxPriorityTable(	t_NSDCustomTable		table, 
												t_Custom2CommandTable	table_C2Cmd,
												unsigned long			*ptr_Command_Table)
{
	unsigned long *ptr_temp;
	int rowIndex, commandIndex;
	unsigned long command, N_Commands = 0;
	unsigned long freq1, freq2;
	unsigned long Ternary_XBitValue = 0;
	unsigned long Ternary_X_counter = 0;
	int Address = 0;


	N_Commands = nsdCustomTable_getNCommands(table);

	for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++)
	{
		Ternary_X_counter = 0;
		Ternary_XBitValue = 0;
		command = 0;

		for (commandIndex = 0; commandIndex < N_Commands; commandIndex++)	// for all used commands
		{
			if (table[rowIndex][commandIndex] != TERNARY_X) // "X" found means that frequency combination is not used
			{
				// frequency combination is used
				if(table[rowIndex][commandIndex] == TERNARY_1) // command is used
				{
					// command is used
					command |= (BIT0<<commandIndex); 
				}
				else
				{
					// 
				}
			}
			else
			{
				// command bit don't care
				Ternary_X_counter++;
				Ternary_XBitValue |= (BIT0<<commandIndex);
			}
		}
		// check for Loop test
		// @Pl_ext
		if(command == 0) command = TX_COM_T;
		//
		if(Ternary_X_counter == N_Commands) command = NOT_USED_FREQUENCY_COMBINATION;
		// write command in table
		table_C2Cmd[rowIndex] = command;
		//
		Ternary_XBit[rowIndex][0] = Ternary_XBitValue;
		Ternary_XBit[rowIndex][1] = Ternary_X_counter;
		// go to next row
	}
	// *******************************************************************
	// *** Write tx command priority table      **************************
	// *******************************************************************
	//
	for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++)
	{
		// get command 
		command = table_C2Cmd[rowIndex];
		Ternary_XBitValue = Ternary_XBit[rowIndex][0]; 
		Ternary_X_counter = Ternary_XBit[rowIndex][1]; 
		//
		// @Pl_ext
		if((command == NOT_USED_FREQUENCY_COMBINATION) || (command == TX_COM_T))
		{
			// 
		}
		else
		{
			for(Address = 0; Address < MAX_COMMAND_COMBINATION; Address++)
			{
				*(ptr_Command_Table + command + (Address & Ternary_XBitValue)) = command;
			}
		}
		// set loop test command
		// @Pl_ext
		if(command == TX_COM_T)
		{
			// @Pl_ext
			*(ptr_Command_Table + MAX_COMMAND_COMBINATION) = command;
			//*(ptr_Command_Table + command) = command;
		}

	}

}

/*****************************************************************************
  Function:		GenerateTxFreqTable(t_Custom2CommandTable	table_C2Cmd, 
									unsigned long			*ptr_Command_Table,
									float					*ptr_txFreq_Table)
  Description:	
  Inputs:		None
  Outputs:		None
  Return:		
  Globals:		
  Important:	

******************************************************************************/
//
void SEGMENT_EXT_CODE GenerateTxFreqTable(	t_Custom2CommandTable	table_C2Cmd, 
											unsigned long			*ptr_Command_Table,
											unsigned long 			*ptr_txFreq_Table)
{
	int rowIndex;
	unsigned long command, TxCommand;
	int Address = 0;
	int freq1, freq2;

	for (rowIndex = 0; rowIndex < NSD_CUSTOM_TABLE_SIZE; rowIndex++)
	{
		command = table_C2Cmd[rowIndex];
		freq1 =	MapFrequency2CustomTable[rowIndex][0];	// get freq 1
		freq2 = MapFrequency2CustomTable[rowIndex][1];	// get freq 2
		if(command != NOT_USED_FREQUENCY_COMBINATION)
		{
			// searching for command in tx priority table
			for(Address = 0; Address < MAX_COMMAND_COMBINATION+1; Address++)
			{
				TxCommand = *(ptr_Command_Table + Address);
				if(TxCommand == command)
				{
					//
					*(ptr_txFreq_Table + 2*Address)		= freq1;
					*(ptr_txFreq_Table + 2*Address + 1)	= freq2;
				}
				else
				{

				}
			}
		}
		else
		{
			// not used frequency combination do not write any frequency
		}
		//
	}

}
//
// @Pl_ext
/*****************************************************************************
  Function:		createTernary_x_Ternary_0_CountTable(unsigned long *p_Ternary_0_Table, unsigned long *p_Ternary_x_Table, t_NSDExtCustomTable *localCustomTable)
  Description:	Filled in the two tables (p_Ternary_0_Table, p_Ternary_x_Table)
				with the count value of ternary_x and ternary_0 symbols per rows.
				Only counts for used commands are done!
  Inputs:		pointer to the two tables p_Ternary_0_Table, p_Ternary_x_Table, 
				pointer to the unpacked custom table of type t_NSDExtCustomTable; 
  Outputs:		Values in the two tables (p_Ternary_0_Table, p_Ternary_x_Table)
  Return:		None	
  Globals:		
  Important:	Counts the ternary_0 and ternary_x symbols only if command is used!

******************************************************************************/
void SEGMENT_EXT_CODE createTernary_x_Ternary_0_CountTable(unsigned long *p_Ternary_0_Table, unsigned long *p_Ternary_x_Table, t_NSDExtCustomTable *localCustomTable)
{
	int i, Cmd_Index;
	unsigned long ternary_x_Counter, ternary_0_Counter;

	// Reset ternary_x, ternary_0 table
	for(i=0;i<NR_DUALTONES;i++)
	{
		*(p_Ternary_0_Table + i) = 0;	
		*(p_Ternary_x_Table + i) = 0;
	}
	//
	// *********************************************************
	// *** create ternary_x and ternary_0 count table **********
	// *********************************************************
	//
	// !!! -> counts the ternary_x and ternary_0 only for used commands !!!
	//
	for(i=0; i<NR_DUALTONES;i++)
	{
		ternary_x_Counter = 0;
		ternary_0_Counter = 0;
		for(Cmd_Index=0; Cmd_Index<COMMAND_SIZE;Cmd_Index++)
		{
			// count ternary_x
			if((localCustomTable->txTable[i][Cmd_Index] == TERNARY_X)&&((localCustomTable->usedCommands &(BIT0<<Cmd_Index)) != 0))
			{
				ternary_x_Counter +=1;
			}
			else
			{

			}
			// count ternary_0 
			if(localCustomTable->txTable[i][Cmd_Index] == TERNARY_0)
			{
				ternary_0_Counter +=1;
			}
			else
			{

			}
		}
		// write count values to table
		*(p_Ternary_x_Table + i) = ternary_x_Counter;
		*(p_Ternary_0_Table + i) = ternary_0_Counter;
	}
}
//
/*****************************************************************************
  Function:		count_nr_of_Commands(unsigned long usedCommands)
  Description:	Counts the number of used commands 
  Inputs:		usedCommands
  Outputs:		None
  Return:		Number of used commands	
  Globals:		
  Important:	

******************************************************************************/
unsigned long SEGMENT_EXT_CODE count_nr_of_Commands(unsigned long usedCommands)
{
	unsigned long Nr_of_Commands = 0;
	int i;

	for (i=0;i<COMMAND_SIZE;i++)
	{
		if((usedCommands &(BIT0<<i)) == 0)
		{

		}
		else
		{
			Nr_of_Commands +=1;	
		}
	}
	return(Nr_of_Commands);
}
//
/*****************************************************************************
  Function:		convert_rxTable(unsigned long *p_RxCmdTable, t_NSDExtCustomTable *localCustomTable)
  Description:	Writes the used command for the dual tone combination in a new table
  Inputs:		pointer to the rxCmdTable[]
				pointer to the struct t_NSDExtCustomTable, where the rxTable[] is included
  Outputs:		Values in the rxCmdTable[]
  Return:		None	
  Globals:		None
  Important:	-

******************************************************************************/
void SEGMENT_EXT_CODE convert_rxTable(unsigned long *p_RxCmdTable, t_NSDExtCustomTable *localCustomTable)
{
	int i, Cmd_Index;
	unsigned long Command;
	// 
	// Reset RxCmdTable array before write to it!
	for(i=0;i<NR_DUALTONES;i++)
	{
		*(p_RxCmdTable + i) = 0;
	}
	//
	//
	for(i=0;i<NR_DUALTONES;i++)
	{
		//
		Command = 0;
		//
		for (Cmd_Index=0; Cmd_Index<COMMAND_SIZE;Cmd_Index++)
		{
			// read command from custom table
			if((localCustomTable->rxTable[i][Cmd_Index]) == 1)
			{
				Command |=(BIT0<<Cmd_Index);
			}
			else
			{
				
			}
		}
		//write command to RxCmdTable
		*(p_RxCmdTable + i) = Command;
	}
}
/*****************************************************************************
  Function:		CreateRxCmdTable_ext(unsigned long *p_RxDetectorTableExt, t_NSDExtCustomTable *p_localCustomTable)
  Description:	Fill in the table described in the file NSD_Detector.c
  Inputs:		Pointer to the Detector command table p_RxDetectorTableExt
				Pointer to the unpacked custom mode table p_localCustomTable
				
  Outputs:		Values in the Detecor command table 
  Return:		None	
  Globals:		None
  Important:	-

******************************************************************************/
void SEGMENT_EXT_CODE CreateRxCmdTable_ext(unsigned long *p_RxDetectorTableExt, t_NSDExtCustomTable *p_localCustomTable)
{
	int i=0, indx = 0;
	unsigned long *p_Ch1_CMD_Table_Ext;
	unsigned long test_1 = 0;
	unsigned long ternary_x_Table[NR_DUALTONES];
	unsigned long ternary_0_Table[NR_DUALTONES];
	unsigned long rxCmdTable[NR_DUALTONES];
	unsigned long Cmd_Index;
	unsigned long Command, Nr_of_Commands;
	unsigned long Freq_1, Freq_2;

	//
	//init customTable
	//

	//success = nsdCustomTable_getNSDextCustomTableForTest(2, &localCustomTable);

	p_Ch1_CMD_Table_Ext = p_RxDetectorTableExt;
	//
	Freq_1 = 0;
	Freq_2 = 0;
	//
	// Reset Detector table before write to table
	for (i=0; i< NUMBER_OF_APPLICATION_MODES * NUMBER_OF_RX_DET_TONES*2 * NUMBER_OF_RX_DET_TONES*2; i++)
	{
		*p_Ch1_CMD_Table_Ext = 0;
		*p_Ch1_CMD_Table_Ext++;
	}
	//
	// *********************************************************
	// *** create ternary_x and ternary_0 count table **********
	// *********************************************************
	//
	createTernary_x_Ternary_0_CountTable(&ternary_0_Table[0], &ternary_x_Table[0], p_localCustomTable);
	//
	// **********************************************************
	// *** Count used commands **********************************
	// **********************************************************
	//
	Nr_of_Commands = count_nr_of_Commands(p_localCustomTable->usedCommands);
	//
	// **********************************************************
	// *** Convert rxTable **************************************
	// **********************************************************
	// 
	convert_rxTable(&rxCmdTable[0], p_localCustomTable);
	//
	// **********************************************************
	// *** Fill in detector command table ***********************
	// **********************************************************
	//
	for(i=0;i<NR_DUALTONES;i++)
	{
		Command = 0;
		for(Cmd_Index=0; Cmd_Index < COMMAND_SIZE; Cmd_Index++)
		{
			if(p_localCustomTable->txTable[i][Cmd_Index] == TERNARY_1)
			{
				Command |= (BIT0<<Cmd_Index);
			}
			else
			{

			}
		}
		//
		Command = (Command & p_localCustomTable->usedCommands);
		// write command to detector command table
		//
		// check ternary_x counter
		if(ternary_x_Table[i] == Nr_of_Commands)
		{
			// command combination not used
			// -> Command = 0 still
		}
		else
		{
			// *************************************************
			// *** Overwrite command with command in rxTable ***
			// *************************************************
			if(rxCmdTable[i] != 0) 
			{
				Command = rxCmdTable[i];
			}
			else
			{
				// no command in rxTable -> do not overwrite
			}
		}
		//
		//
		if(Command != 0)
		{
			// write command to the detector command table -> Ch1_CMD_Table_NSD_Ext[]
			// read frequency numbers
			Freq_1 = MapFrequency2CustomTable[i][0];
			Freq_2 = MapFrequency2CustomTable[i][1];
			//
			for(indx=0;indx<NUMBER_OF_APPLICATION_MODES;indx++)
			{
				*(p_RxDetectorTableExt + Freq_1*NUMBER_OF_APPLICATION_MODES*NUMBER_OF_RX_DET_TONES*2 + Freq_2*NUMBER_OF_APPLICATION_MODES + indx) = Command;			
				*(p_RxDetectorTableExt + Freq_2*NUMBER_OF_APPLICATION_MODES*NUMBER_OF_RX_DET_TONES*2 + Freq_1*NUMBER_OF_APPLICATION_MODES + indx) = Command;
			}

		}
		else
		{
			// Command is 0 -> no table entry into the detector command table for this frequency combination
		}
	}
	// *********************************************
	// *** Write Loop test to detector command table
	// *********************************************
	//
	Command = RX_COM_T;	// set Loop Test mask
	// read freq 1 and freq 2 for loop test
	Freq_1 = MapFrequency2CustomTable[p_localCustomTable->testTone - 1][0];
	Freq_2 = MapFrequency2CustomTable[p_localCustomTable->testTone - 1][1];
	// write loop test mask to detector command table
	for(indx=0;indx<NUMBER_OF_APPLICATION_MODES;indx++)
	{
		*(p_RxDetectorTableExt + Freq_1*NUMBER_OF_APPLICATION_MODES*NUMBER_OF_RX_DET_TONES*2 + Freq_2*NUMBER_OF_APPLICATION_MODES + indx) = Command;			
		*(p_RxDetectorTableExt + Freq_2*NUMBER_OF_APPLICATION_MODES*NUMBER_OF_RX_DET_TONES*2 + Freq_1*NUMBER_OF_APPLICATION_MODES + indx) = Command;
	}
}
//
/*****************************************************************************
  Function:		void SEGMENT_EXT_CODE CreateTransmittFreqTable(unsigned long *p_TransmitTable, unsigned long *p_Delete_Cmd_Table, t_NSDExtCustomTable *p_localCustomTable)
  Description:	Writes the frequency Number to the transmit freq table in accordance to 
				the custom table input. For command A assuming used freq F1 and F2, 
				this functions writes 1 into the array[2*1] and 2  into the arry [2*1+1]
  Inputs:		pointer to the transmit freq. table of array type unsigned long size [(MAX_COMMAND_COMBINATION +1)*NUMBER_OF_TONE_PER_CMD]
				pointer to the unpacked custom Table of type t_NSDExtCustomTable
				
  Outputs:		write the frequency numbers to the transmit freq table 
  Return:		None	
  Globals:		None
  Important:	-

******************************************************************************/
void SEGMENT_EXT_CODE CreateTransmittFreqTable(unsigned long *p_TransmitTable, unsigned long *p_Delete_Cmd_Table, t_NSDExtCustomTable *p_localCustomTable)
{
	//
	unsigned long *p_Temp_Table;
	unsigned long i_dx;
	unsigned long i, i_score;
	unsigned long ternary_x_Table[NR_DUALTONES];
	unsigned long ternary_0_Table[NR_DUALTONES];
	unsigned long ternary_x_Members[NR_DUALTONES];
	unsigned long Freq_1, Freq_2;
	unsigned long Count_ternary_x, Max_Count_x;
	unsigned long temp1, temp2, ul_Adress;
	unsigned long Adress_Table[NR_DUALTONES][3];
	unsigned long Nr_of_Commands;
	//
	// *********************************************************
	// *** init used parameters ********************************
	// *********************************************************
	//
	Freq_1 = MapFrequency2CustomTable[0][0];
	Freq_2 = MapFrequency2CustomTable[0][1];
	Max_Count_x = 0;
	Count_ternary_x = 0;
	temp1 = 0;
	temp2 = 0;
	ul_Adress = 0;
	// reset table ternary_x_MemberTable[]
	for(i=0;i<NR_DUALTONES;i++)
	{
		ternary_x_Members[i] = 0;
		Adress_Table[i][0] = 0;
		Adress_Table[i][1] = 0;
		Adress_Table[i][2] = 0;
	}
	//
	p_Temp_Table = p_TransmitTable;

	for(i=0; i<(MAX_COMMAND_COMBINATION +1)*NO_OF_TONES_PER_COMMAND; i++)
	{
		// Reset Tx Command frequency table before write to it
		*p_Temp_Table++ = 0;
	}
	//
	p_Temp_Table = p_Delete_Cmd_Table;
	//
	for(i=0; i<(MAX_COMMAND_COMBINATION +1); i++)
	{
		// Reset Delete_Cmd_Table before write to it
		*p_Temp_Table++ = 0;
	}
	//
	p_Temp_Table = p_TransmitTable;
	//
	// *********************************************************
	// *** create ternary_x and ternary_0 count table **********
	// *********************************************************
	//
	createTernary_x_Ternary_0_CountTable(&ternary_0_Table[0], &ternary_x_Table[0], p_localCustomTable);
	//
	// **********************************************************
	// *** Count used commands **********************************
	// **********************************************************
	//
	Nr_of_Commands = count_nr_of_Commands(p_localCustomTable->usedCommands);
	//
	// *********************************************************
	// *** count rows with ternary_x symbols *******************
	// *********************************************************
	//
	for(i=0; i<NR_DUALTONES; i++)
	{
		if((ternary_x_Table[i] > 0) && (ternary_x_Table[i] < Nr_of_Commands)) 
		{
			ternary_x_Members[Count_ternary_x] = i;
			Count_ternary_x +=1;
		}
		else
		{

		}
	}
	//
	Max_Count_x = Count_ternary_x;	// save count number of rows with ternary_x symbols
	//
	if( Max_Count_x > 0)
	{
		// *************************************************************************
		// *** write send freq first for combinations with ternary_x symbols  ******
		// *************************************************************************
		//
		// ***********************************************
		// *** Create table with sorted high score ternary_x count
		// ***********************************************
		//
		for(i=0; i< Max_Count_x; i++)
		{
			//
			for (i_score = i; i_score < Max_Count_x; i_score++) 
			{
				if(ternary_x_Table[ternary_x_Members[i]] < ternary_x_Table[ternary_x_Members[i_score]])
				{
					// change score
					temp1 = ternary_x_Members[i];
					temp2 = ternary_x_Members[i_score];
					ternary_x_Members[i] = temp2;
					ternary_x_Members[i_score] = temp1;
				}
				else
				{
					// do not change score
				}
			}
		}
		// *****************************************************
		// *** write freq to table with ternary x symbols first*
		// *****************************************************
		//
		// Set start and end address for ternary x rows
		//
		for(i=0; i< Max_Count_x; i++)
		{
			for(i_dx=0; i_dx<COMMAND_SIZE; i_dx++)
			{
				// set start adress
				if((p_localCustomTable->txTable[ternary_x_Members[i]][i_dx] == TERNARY_X)&&((p_localCustomTable->usedCommands&(BIT0<<i_dx)) != 0))
				{
					//Adress_Table[i][START_1] 
					Adress_Table[i][STOP_1] |= (BIT0<<i_dx);
					Adress_Table[i][MASK_1] |= (BIT0<<i_dx);
				}
				if((p_localCustomTable->txTable[ternary_x_Members[i]][i_dx] == TERNARY_1)&&((p_localCustomTable->usedCommands&(BIT0<<i_dx)) != 0))
				{
					Adress_Table[i][START_1] |= (BIT0<<i_dx);
					Adress_Table[i][STOP_1] |= (BIT0<<i_dx);
					Adress_Table[i][MASK_1] |= (BIT0<<i_dx);
				}
			}
		}
		//
		// write freq to transmit table
		//
		p_Temp_Table = p_TransmitTable;
		//
		for(i=0; i< Max_Count_x; i++)
		{
			Freq_1 = MapFrequency2CustomTable[ternary_x_Members[i]][0];
			Freq_2 = MapFrequency2CustomTable[ternary_x_Members[i]][1];
			for(i_dx = Adress_Table[i][START_1]; i_dx <= Adress_Table[i][STOP_1]; i_dx++)
			{
				ul_Adress = ((i_dx | Adress_Table[i][START_1])&(Adress_Table[i][MASK_1]));
				if(ul_Adress == 0)
				{
					// do not write freq to address 0 and 1 -> guard
				}
				else
				{
					*(p_Temp_Table + 2*ul_Adress) = Freq_1;
					*(p_Temp_Table + 2*ul_Adress+1) = Freq_2;
				}
			}
		}
	}
	else
	{
		// Max Count x  = 0 -> no row entries with ternary_x symbols
	}
	//
	// *****************************************************************************
	// *** write the other freq without ternary x symbols to transmit freq table ***
	// *****************************************************************************
	//
	for(i=0; i<NR_DUALTONES; i++)
	{	
		Freq_1 = MapFrequency2CustomTable[i][0];
		Freq_2 = MapFrequency2CustomTable[i][1];
		temp1 = 0;
		//
		for(i_dx=0;i_dx < COMMAND_SIZE; i_dx++)
		{
			if((p_localCustomTable->txTable[i][i_dx] == TERNARY_1)&&((p_localCustomTable->usedCommands&(BIT0<<i_dx)) != 0))
			{
				temp1 |= (BIT0<<i_dx);	
			}
			else
			{

			}
		}
		if((temp1 == 0)||(ternary_x_Table[i] != 0))
		{
			// address = 0 -> do not write
		}
		else
		{
			*(p_Temp_Table + 2*temp1) = Freq_1;
			*(p_Temp_Table + 2*temp1+1) = Freq_2;
		}
	}
	// 
	// *****************************************************************************
	// *** write loop test freq to table *******************************************
	// *****************************************************************************
	//
		Freq_1 = MapFrequency2CustomTable[p_localCustomTable->testTone - 1][0];
		Freq_2 = MapFrequency2CustomTable[p_localCustomTable->testTone - 1][1];
		*(p_Temp_Table + 2*MAX_COMMAND_COMBINATION) = Freq_1;
		*(p_Temp_Table + 2*MAX_COMMAND_COMBINATION+1) = Freq_2;

	//
	// *****************************************************************************
	// *** delete Freq in table for guard commands *********************************
	// *****************************************************************************
	//
	if(p_localCustomTable->nrOfGuardCommands == 0)
	{
		// -> no guard command used for the actual mode
	}
	else
	{
		//
		// *****************************************************
		// *** guard commands used -> reset commands according guard table
		// *****************************************************
		// read number of guard commands 
		i_score = p_localCustomTable->nrOfGuardCommands;
		// 
		for(i=0; i < i_score; i++)
		{
			temp1 = 0;
			temp2 = 0;
			//
			// *************************************************
			// *** generate start and end address **************
			// *************************************************
			//
			for(i_dx=0;i_dx < COMMAND_SIZE; i_dx++)
			{
				// generate start adress 
				if((p_localCustomTable->guardCommands[i][i_dx] == TERNARY_1)&&((p_localCustomTable->usedCommands&(BIT0<<i_dx)) != 0))
				{
					temp1 |= (BIT0<<i_dx);	// set start adress
					temp2 |= (BIT0<<i_dx);	// set bit for end adress = mask
				}
				else
				{

				}
				// generate end adress
				if((p_localCustomTable->guardCommands[i][i_dx] == TERNARY_X)&&((p_localCustomTable->usedCommands&(BIT0<<i_dx)) != 0))
				{
					temp2 |= (BIT0<<i_dx);	// set end adress = mask
				}
				else
				{

				}
			}
			//
			// *************************************************
			// *** Reset freq in transmit freq table ***********
			// *************************************************
			//
			for(i_dx = temp1; i_dx <= temp2; i_dx++)
			{
				ul_Adress = ((i_dx|temp1)&temp2);
				*(p_Temp_Table + 2*ul_Adress) = 0;
				*(p_Temp_Table + 2*ul_Adress + 1) = 0;
				*(p_Delete_Cmd_Table + ul_Adress) = SEND_GUARD;
			}
		}
	}
}




