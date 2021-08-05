/*-------------------------------------------------------------------------*
 * $Workfile: Event_Defs.H $
 * Part of	: ETL600 
 * Language	: C
 * Created by	:  Christian Leeb 
 * Remarks	: Category defines for eventrecorder                     
 * Purpose	: 
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*    
 * $Log: /branches/Source R4/P4LT/Master_DSP/H/Event_Defs.H $                                  
 * 
 * 70    10-11-24 16:12 Chhamaa
 * New alarms and warning for rel. 4 added.
 * 
 * 69    10-10-28 11:22 Chhamaa
 * New alarms added: ALM_TRANSIT_NO_SYNC, ALM_TRANSIT_MISSING_RX_DATA
 * 
 * 66    08-02-25 10:18 Chtognu
 * Merged changes from ad.equ.branch:
 * ALM_DCD_NSK1..4 added
 * 
 * 65    10.01.07 16:25 Chhamaa
 * Defines for O4CV alarms and warning added
 * 
 * 63    19.09.06 10:53 Chhamaa
 * ALM_NSD_Command_Outputs_Blocked added to NSD_IF_ALM_MASK
 * 
 * 62    18.09.06 17:27 Chhamaa
 * define ALM_NSD_Command_Outputs_Blocked added.
 * 
 * 61    18.09.06 16:54 Chhamaa
 * Defines AES_MANUAL_RESET_ALM_MASK and NSD_LEVEL_ALM_MASK added.
 * 
 * 60    7.08.06 14:05 Chchlee
 * 
 * 57    21.07.06 10:15 Chchlee
 * Comments added
 * 
 * 56    18.07.06 16:21 Chchlee
 * ALM_R1LA_Life_Sign added
 * 
 * 55    6.06.06 9:38 Chalgyg
 * G.703 LOS alarm added to define DATA_LINK_ALM_MASK.
 * 
 * 52    14.10.05 11:30 Chchlee
 * Comment added
 * 
 * 51    13.10.05 10:44 Chchlee
 * Comments added
 * 
 * 50    12.10.05 14:14 Chchlee
 * 
 * 49    18.09.05 15:42 Chchlee
 * additional debug events added
 * 
 * 48    1.09.05 11:34 Chchlee
 * ALARM_EVENT_DEBUG1-4 added
 * 
 * 47    21.07.05 13:31 Chchlee
 * Cleaned up
 * 
 * 46    30.06.05 8:13 Chchlee
 * #define NUMBER_OF_COMMANDS 4  removed
 * 
 * 45    29.06.05 8:27 Chchlee
 * cleaned up
 * 
 * 44    14.06.05 17:12 Chpeplu
 * define NSD_IF_ALM_MASK changed -> rx continuous command added
 * 
 * 43    9.06.05 19:51 Chchlee
 * WARN_Event_Recorder_block_rotation added
 * 
 * 42    6.06.05 10:29 Chpeplu
 * New Alarm Events added
 * 
 * 41    3.06.05 10:14 Chmibuh
 * 
 * 40    27.05.05 8:37 Chchlee
 * ALM_UART_not_operational removed
 * 
 * 39    27.05.05 8:32 Chchlee
 * 
 * 38    23.05.05 8:03 Chchlee
 * 
 * 37    23.05.05 7:04 Chchlee
 * 
 * 36    17.05.05 16:57 Chchlee
 * ofdmAlarms.configAlm
 * 
 * 35    22.03.05 16:55 Chutpra
 * Clean old status structure
 * 
 * 34    1.03.05 13:23 Chchlee
 * DATA_HW_ALM_MASK alarm mask fixed
 * 
 * 33    7.02.05 18:10 Chchlee
 * command event names modified
 * 
 * 32    3.02.05 10:43 Chhamaa
 * WARN_Equalizer_action added
 * 
 * 31    28.01.05 9:25 Chchlee
 * WARN_Slave_board changed to WARN_O4LE_board
 * 
 * 30    14.01.05 14:18 Chsanoh
 * Low level alarm defines adapted to "ETL600 Alarms&Warnings B.xls".
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef EVENT_DEFS_H
#define EVENT_DEFS_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

// Bit-Maske fuer Event Recorder 
#define		ERPOM_NOT_BUSY_FLAG					    BIT31 // is 0 if busy!!

/* PUBLIC TYPE DECLARATIONS ***************************************************/


typedef struct
{
	unsigned long date;
	unsigned long time;
	unsigned long counter;

}t_EventCounter;

typedef struct
{
	unsigned long categoryAndDate;
	unsigned long time;
	unsigned long data;

}t_EventData;

// Type of message response returned on READ_EVENT request
typedef struct
{
	t_EventData   eventData;
	unsigned long controlInformation;

}t_HMIEventData;


/* PUBLIC MACRO DEFINITIONS ***************************************************/


#define BYTES_PER_EVENT    (sizeof(t_EventData) * NR_OF_BYTES_IN_LONG / sizeof(long))


// Eventrecorder

#define MINIMUM_EVENTS 1000 // Anzahl Events die minimal IMMER gespeichert sind

#define MAX_CMD_EVENTS 2600 // Anzahl Speicherplätze im ersten Block                 
#define MAX_ALM_EVENTS 2600 // Anzahl im zweiten Block.Beide zusammen 

#define EVENTDATA_OFFSET   0x10
   
#define START 	1
#define END  	0
 
		

// ****************************************************************
// Alarms and Warnings
// ****************************************************************

// DATA alarms and warnings

#define	ALM_NSK_Life_Sign				BIT0
#define	ALM_MOD_Life_Sign				BIT1
#define	ALM_NSK_Programm_Memory_CRC		BIT2
#define	ALM_MOD_Programm_Memory_CRC		BIT3
#define	ALM_FPGA_not_operational		BIT4
#define	ALM_HW_Piggyback				BIT5
#define	ALM_LINK_OFDM_PILOT				BIT6
#define	ALM_LINK_OFDM_SYNC				BIT7
#define	ALM_LINK_NSK1					BIT8
#define	ALM_LINK_NSK2					BIT9
#define	ALM_LINK_NSK3					BIT10
#define	ALM_LINK_NSK4					BIT11
#define	ALM_LINK_MUX					BIT12
#define	ALM_INTERFACE_PORT1				BIT13  // V24 Port1
#define	ALM_INTERFACE_PORT2				BIT14  // V24 Port2
#define	ALM_INTERFACE_PORT3				BIT15  // V24 Port3
#define	ALM_INTERFACE_PORT4				BIT16  // V24 Port4
#define	ALM_INTERFACE_PORT5				BIT17  // V24 Port5
#define	ALM_INTERFACE_PORT6				BIT18  // V24 Port6
#define	ALM_INTERFACE_PORT7				BIT19  // V11 Port1 (alarm not used)
#define	ALM_INTERFACE_PORT8				BIT20  // V11 Port1 (alarm not used)
#define	ALM_TRANSIT_NO_SYNC				BIT21
#define	ALM_INTERFACE_PORT10			BIT22  // G703.1 LOS
#define	ALM_TRANSIT_MISSING_RX_DATA		BIT23
#define	WARN_LAN						BIT24
#define	ALM_OFDM_Config					BIT25
#define	ALM_R1LA_Life_Sign				BIT26
#define	ALM_DCD_NSK1					BIT27
#define	ALM_DCD_NSK2					BIT28
#define	ALM_DCD_NSK3					BIT29
#define	ALM_DCD_NSK4					BIT30
#define ALM_LAN							BIT31

// PLC HW alarms
#define	ALM_Master_Life_Sign		    BIT0
#define	ALM_FE_Life_Sign		        BIT1
#define	ALM_UC_Life_Sign		        BIT2
#define	ALM_Master_Programm_Memory_CRC	BIT3
#define	ALM_FE_Programm_Memory_CRC		BIT4
#define	ALM_UC_Programm_Memory_CRC		BIT5
#define	ALM_M12V_Underv		            BIT6
#define	ALM_M12V_Overv		            BIT7
#define	ALM_P12V_Underv		            BIT8
#define	ALM_P12V_Overv		            BIT9
#define	ALM_P5V_Underv		            BIT10
#define	ALM_P5V_Overv		            BIT11
#define	ALM_P3_3V_Underv		        BIT12
#define	ALM_P3_3V_Overv					BIT13
#define	ALM_P1_8V_Underv				BIT14
#define	ALM_P1_8V_Overv					BIT15
#define	ALM_Event_Recorder_disabled		BIT16
#define	ALM_RF_Rx_overflow				BIT18
#define	ALM_License						BIT19
#define	ALM_Plut_out_3_RF_Hybrid		BIT21
#define	ALM_Plut_out_4_Rx_Filter		BIT22
#define	ALM_HW_O4LE_board				BIT23
#define	ALM_Lifesign_O4LE_board			BIT24
#define	ALM_Tx_RF						BIT25
#define ALM_HW_O4CV_board				BIT26
#define	ALM_Lifesign_O4CV_board			BIT27
#define ALM_PSU1_failed					BIT28
#define ALM_PSU2_failed					BIT29

// PLC Link alarms
#define	ALM_Slow_Mute_Ch1				BIT0
#define	ALM_Slow_Mute_Ch2				BIT1
#define	ALM_Slow_Mute_Ch3				BIT2
#define	ALM_Rx_pilot_fail_Ch1			BIT3
#define	ALM_Rx_pilot_fail_Ch2			BIT4
#define	ALM_Rx_pilot_fail_Ch3			BIT5
#define	ALM_Pilot_level_Ch1				BIT6
#define	ALM_Pilot_level_Ch2				BIT7
#define	ALM_Pilot_level_Ch3				BIT8
#define	ALM_AGC_fail_Ch1				BIT9
#define	ALM_AGC_fail_Ch2				BIT10
#define	ALM_AGC_fail_Ch3				BIT11
#define	ALM_AFC_fail					BIT12

// PLC interface alarms and warnings

	//PLC interface alarms
#define	ALM_Incorrect_RTC_date					BIT0
#define	ALM_IRIG_signal_lost					BIT1

	//PLC warnings
#define	WARN_EOC_Ch1							BIT2
#define	WARN_No_EOC_data_tx_Ch1					BIT5
#define	WARN_No_EOC_data_rx_Ch1					BIT8
#define WARN_Local_RF_loopback_on				BIT10
#define	WARN_Any_test_switch_active				BIT11
#define	WARN_Not_in_normal_operational_mode		BIT12
#define	WARN_Different_Fw_revision				BIT13
#define	WARN_Temporary_configuration_active		BIT14
#define	WARN_Dummyload_plugged					BIT15
#define	WARN_O4LE_board							BIT16
#define	WARN_Ext_in								BIT17
#define	WARN_Event_Recorder_block_rotation		BIT18
#define WARN_Equalizer_action					BIT19
#define	WARN_O4CV_board							BIT20	

// NSD HW Alarms
#define	ALM_NSD_Life_Sign					    BIT0
#define	ALM_NSD_Programm_Memory_CRC			    BIT1
#define	ALM_NSD_Data_Memory					    BIT2
#define	ALM_NSD_Extern_Extended_TDM_Bus		    BIT3
#define	ALM_NSD_Extern_Basic_TDM_Bus		    BIT4
#define	ALM_Lifesign_G4AI					    BIT5
#define	ALM_NSD_G4AI_board					    BIT19

// NSD Link Alarms
#define	ALM_NSD_Rx_Low_Level				    BIT6
#define	ALM_NSD_Rx_High_Level				    BIT7
#define	ALM_NSD_Rx_SNR						    BIT8
#define	ALM_NSD_No_Trip_and_No_Guard		    BIT9
#define	ALM_NSD_Trip_and_Guard				    BIT10
#define	ALM_NSD_Loop_Test					    BIT11

// NSD Interface Alarms
#define	ALM_NSD_Output_Set_To_Predefined_State	BIT12
#define ALM_NSD_G4AI_Rx_Continuous				BIT13
#define ALM_NSD_G4AI_Interface					BIT20
#define ALM_NSD_Command_Outputs_Blocked			BIT22
			
// NSD Warnings
#define	WARN_NSD_Test_Config_Active				BIT14
#define	WARN_NSD_Local_Loop_Active				BIT15
#define	WARN_NSD_Loop_Test						BIT16
#define	WARN_NSD_Test_Signal_Active				BIT18
#define	WARN_NSD_G4AI_board						BIT21


// Masks for reset condition -> CRC errors
#define CRC_RESET_NSD_HW_ALM_MASK			(ALM_NSD_Programm_Memory_CRC)
#define	CRC_RESET_DATA_HW_ALM_MASK			(ALM_NSK_Programm_Memory_CRC | ALM_MOD_Programm_Memory_CRC)
#define CRC_RESET_PLC_HW_ALM_MASK			(ALM_Master_Programm_Memory_CRC | ALM_FE_Programm_Memory_CRC | ALM_UC_Programm_Memory_CRC)


// high level alarm masks
#define NSD_HW_ALM_MASK   (ALM_NSD_Life_Sign | ALM_NSD_Programm_Memory_CRC |\
						   ALM_NSD_Data_Memory | ALM_NSD_Extern_Extended_TDM_Bus |\
						   ALM_NSD_Extern_Basic_TDM_Bus | \
						   ALM_Lifesign_G4AI | ALM_NSD_G4AI_board)

#define NSD_LINK_ALM_MASK (ALM_NSD_Rx_Low_Level | ALM_NSD_Rx_High_Level |\
						   ALM_NSD_Rx_SNR | ALM_NSD_No_Trip_and_No_Guard |\
                           ALM_NSD_Trip_and_Guard | ALM_NSD_Loop_Test)

#define NSD_LEVEL_ALM_MASK (ALM_NSD_Rx_Low_Level | ALM_NSD_Rx_High_Level)

#define AES_MANUAL_RESET_ALM_MASK (NSD_LEVEL_ALM_MASK | ALM_NSD_Rx_SNR)

#define NSD_IF_ALM_MASK   (ALM_NSD_Output_Set_To_Predefined_State | ALM_NSD_G4AI_Rx_Continuous |\
						   ALM_NSD_Command_Outputs_Blocked)

#define NSD_WARN_MASK     (WARN_NSD_Test_Config_Active | WARN_NSD_Local_Loop_Active |\
						   WARN_NSD_Loop_Test | WARN_NSD_Test_Signal_Active |\
                           WARN_NSD_G4AI_board)

#define PLC_IF_ALM_MASK   (ALM_Incorrect_RTC_date | ALM_IRIG_signal_lost)
#define PLC_WARN_MASK     (~PLC_IF_ALM_MASK)

#define DATA_HW_ALM_MASK (ALM_NSK_Life_Sign | ALM_MOD_Life_Sign |\
						  ALM_NSK_Programm_Memory_CRC | ALM_MOD_Programm_Memory_CRC |\
	                      ALM_FPGA_not_operational | ALM_HW_Piggyback | ALM_OFDM_Config |\
						  ALM_R1LA_Life_Sign)

#define DATA_LINK_ALM_MASK (\
									ALM_LINK_OFDM_PILOT |\
									ALM_LINK_OFDM_SYNC  |\
									ALM_LINK_NSK1       |\
									ALM_LINK_NSK2       |\
									ALM_LINK_NSK3       |\
									ALM_LINK_NSK4       |\
									ALM_DCD_NSK1        |\
									ALM_DCD_NSK2        |\
									ALM_DCD_NSK3        |\
									ALM_DCD_NSK4        |\
									ALM_LINK_MUX)

#define DATA_IF_ALM_MASK (\
               						ALM_INTERFACE_PORT1 |\
               						ALM_INTERFACE_PORT2	|\
               						ALM_INTERFACE_PORT3 |\
               						ALM_INTERFACE_PORT4 |\
               						ALM_INTERFACE_PORT5 |\
               						ALM_INTERFACE_PORT6 |\
									ALM_INTERFACE_PORT10|\
									ALM_TRANSIT_NO_SYNC |\
									ALM_TRANSIT_MISSING_RX_DATA |\
									ALM_LAN)

#define DATA_WARN_MASK (WARN_LAN)	

// top level alarm masks
#define TOP_LEVEL_ALM_HW_MASK (\
						R1BCALM_LOCAL_PLC_HW_ALM  |\
						R1BCALM_LOCAL_TPE1_HW_ALM |\
						R1BCALM_LOCAL_TPE2_HW_ALM |\
						R1BCALM_LOCAL_DATA_HW_ALM)

#define TOP_LEVEL_ALM_LINK_MASK (\
						R1BCALM_LOCAL_PLC_LINK_ALM  |\
						R1BCALM_LOCAL_TPE1_LINK_ALM |\
						R1BCALM_LOCAL_TPE2_LINK_ALM |\
						R1BCALM_LOCAL_DATA_LINK_ALM)

#define TOP_LEVEL_ALM_IF_MASK (\
						R1BCALM_LOCAL_PLC_IF_ALM  |\
						R1BCALM_LOCAL_TPE1_IF_ALM |\
						R1BCALM_LOCAL_TPE2_IF_ALM |\
						R1BCALM_LOCAL_DATA_IF_ALM)

#define TOP_LEVEL_WARN_MASK (\
						R1BCALM_LOCAL_PLC_WARN  |\
						R1BCALM_LOCAL_TPE1_WARN |\
						R1BCALM_LOCAL_TPE2_WARN |\
						R1BCALM_LOCAL_DATA_WARN)

// HMI600 internal usage only
#define TOP_LEVEL_ALM_PLC_MASK (\
						R1BCALM_LOCAL_PLC_HW_ALM  |\
						R1BCALM_LOCAL_PLC_LINK_ALM  |\
						R1BCALM_LOCAL_PLC_IF_ALM  |\
						R1BCALM_LOCAL_PLC_WARN)

// HMI600 internal usage only
#define TOP_LEVEL_ALM_DATA_MASK (\
						R1BCALM_LOCAL_DATA_HW_ALM |\
						R1BCALM_LOCAL_DATA_LINK_ALM |\
						R1BCALM_LOCAL_DATA_IF_ALM |\
						R1BCALM_LOCAL_DATA_WARN)

// HMI600 internal usage only
#define TOP_LEVEL_ALM_TPE1_MASK (\
						R1BCALM_LOCAL_TPE1_HW_ALM |\
						R1BCALM_LOCAL_TPE1_LINK_ALM |\
						R1BCALM_LOCAL_TPE1_IF_ALM |\
						R1BCALM_LOCAL_TPE1_WARN)
					
// HMI600 internal usage only
#define TOP_LEVEL_ALM_TPE2_MASK (\
						R1BCALM_LOCAL_TPE2_HW_ALM |\
						R1BCALM_LOCAL_TPE2_LINK_ALM |\
						R1BCALM_LOCAL_TPE2_IF_ALM |\
						R1BCALM_LOCAL_TPE2_WARN)

// system level alarm mask
// HMI600 internal usage only
#define SYSTEM_LEVEL_ALARM_MASK (\
							TOP_LEVEL_ALM_HW_MASK	|\
							TOP_LEVEL_ALM_LINK_MASK	|\
							TOP_LEVEL_ALM_IF_MASK)


///////////////////////////////////////////////////////////////////////////
// definition of alarm event categories
///////////////////////////////////////////////////////////////////////////////////

#define FIRST_ALARM_EVENT              100

// high level alarm events
#define ALARM_EVENT_PLC_HW_ALM                      (FIRST_ALARM_EVENT + 0)
#define ALARM_EVENT_PLC_LINK_ALM                    (FIRST_ALARM_EVENT + 1)
#define ALARM_EVENT_PLC_IF_ALM                      (FIRST_ALARM_EVENT + 2)
#define ALARM_EVENT_PLC_WARN                        (FIRST_ALARM_EVENT + 3)

#define ALARM_EVENT_NSD1_HW_ALM                     (FIRST_ALARM_EVENT + 4)
#define ALARM_EVENT_NSD1_LINK_ALM                   (FIRST_ALARM_EVENT + 5)
#define ALARM_EVENT_NSD1_IF_ALM                     (FIRST_ALARM_EVENT + 6)
#define ALARM_EVENT_NSD1_WARN                       (FIRST_ALARM_EVENT + 7)

#define ALARM_EVENT_NSD2_HW_ALM                     (FIRST_ALARM_EVENT + 8)
#define ALARM_EVENT_NSD2_LINK_ALM                   (FIRST_ALARM_EVENT + 9)
#define ALARM_EVENT_NSD2_IF_ALM                     (FIRST_ALARM_EVENT + 10)
#define ALARM_EVENT_NSD2_WARN                       (FIRST_ALARM_EVENT + 11)
  
#define ALARM_EVENT_DATA_HW_ALM                     (FIRST_ALARM_EVENT + 12)
#define ALARM_EVENT_DATA_LINK_ALM                   (FIRST_ALARM_EVENT + 13)
#define ALARM_EVENT_DATA_IF_ALM                     (FIRST_ALARM_EVENT + 14)
#define ALARM_EVENT_DATA_WARN                       (FIRST_ALARM_EVENT + 15)


// Manipulation events
#define ALARM_EVENT_MACHINE_START	                (FIRST_ALARM_EVENT + 16)   

#define ALARM_EVENT_RESET_COUNTER                   (FIRST_ALARM_EVENT + 17) // event recorder internal use
#define ALARM_EVENT_EVENTS_LOST		                (FIRST_ALARM_EVENT + 18) // event recorder internal use  

#define ALARM_EVENT_DATE_TIME_SET				    (FIRST_ALARM_EVENT + 19)  

#define ALARM_EVENT_TPE1_LOOPTEST_SENT 	   	 	    (FIRST_ALARM_EVENT + 20)     
#define ALARM_EVENT_TPE1_LOOPTEST_FAILED  	 	    (FIRST_ALARM_EVENT + 21)     
#define ALARM_EVENT_TPE1_LOOPTEST_REPLIED 	 	    (FIRST_ALARM_EVENT + 22) 

#define ALARM_EVENT_TPE2_LOOPTEST_SENT 	  	 	    (FIRST_ALARM_EVENT + 23)     
#define ALARM_EVENT_TPE2_LOOPTEST_FAILED  	 	    (FIRST_ALARM_EVENT + 24)     
#define ALARM_EVENT_TPE2_LOOPTEST_REPLIED   	 	(FIRST_ALARM_EVENT + 25) 

#define ALARM_EVENT_EPROM_SAVE		 	 	 	    (FIRST_ALARM_EVENT + 26)

#define ALARM_EVENT_TDM_BUS_ALARM                   (FIRST_ALARM_EVENT + 27)

#define ALARM_EVENT_TPE1_MANUAL_LOOPTEST_SENT		(FIRST_ALARM_EVENT + 28)     
#define ALARM_EVENT_TPE1_MANUAL_LOOPTEST_FAILED		(FIRST_ALARM_EVENT + 29) 
#define ALARM_EVENT_TPE2_MANUAL_LOOPTEST_SENT 	    (FIRST_ALARM_EVENT + 30)     
#define ALARM_EVENT_TPE2_MANUAL_LOOPTEST_FAILED		(FIRST_ALARM_EVENT + 31) 

#define	ALARM_EVENT_USER_RESET						(FIRST_ALARM_EVENT + 32)
#define	ALARM_EVENT_FIRMWARE_DOWNLOAD				(FIRST_ALARM_EVENT + 33)
#define	ALARM_EVENT_PREVIOUS_CONFIGURATION			(FIRST_ALARM_EVENT + 34)
#define	ALARM_EVENT_CONFIGURATION_DOWNLOAD			(FIRST_ALARM_EVENT + 35)

#define	ALARM_EVENT_DEBUG1							(FIRST_ALARM_EVENT + 105)
#define	ALARM_EVENT_DEBUG2							(FIRST_ALARM_EVENT + 106)
#define	ALARM_EVENT_DEBUG3							(FIRST_ALARM_EVENT + 107)
#define	ALARM_EVENT_DEBUG4							(FIRST_ALARM_EVENT + 108)
#define	ALARM_EVENT_DEBUG5							(FIRST_ALARM_EVENT + 109)
#define	ALARM_EVENT_DEBUG6							(FIRST_ALARM_EVENT + 110)
#define	ALARM_EVENT_DEBUG7							(FIRST_ALARM_EVENT + 111)
#define	ALARM_EVENT_DEBUG8							(FIRST_ALARM_EVENT + 112)
#define	ALARM_EVENT_DEBUG9							(FIRST_ALARM_EVENT + 113)
#define	ALARM_EVENT_DEBUG10							(FIRST_ALARM_EVENT + 114)


///////////////////////////////////////////////////////////////////////////
// definition of command event categories
///////////////////////////////////////////////////////////////////////////////////

#define FIRST_CMD_EVENT             0


#define NSD1_RX_CMD_A               (FIRST_CMD_EVENT + 0)
#define NSD1_RX_CMD_B               (FIRST_CMD_EVENT + 1)
#define NSD1_RX_CMD_C               (FIRST_CMD_EVENT + 2)
#define NSD1_RX_CMD_D               (FIRST_CMD_EVENT + 3) 
#define NSD1_RX_CMD_E               (FIRST_CMD_EVENT + 4) // for future enhancements
#define NSD1_RX_CMD_F               (FIRST_CMD_EVENT + 5) // for future enhancements
#define NSD1_RX_CMD_P               (FIRST_CMD_EVENT + 6) 
#define NSD1_RX_CMD_U               (FIRST_CMD_EVENT + 7)

#define NSD1_TX_CMD_A               (FIRST_CMD_EVENT + 8) 
#define NSD1_TX_CMD_B               (FIRST_CMD_EVENT + 9) 
#define NSD1_TX_CMD_C               (FIRST_CMD_EVENT + 10)
#define NSD1_TX_CMD_D               (FIRST_CMD_EVENT + 11)
#define NSD1_TX_CMD_E               (FIRST_CMD_EVENT + 12) // for future enhancements
#define NSD1_TX_CMD_F               (FIRST_CMD_EVENT + 13) // for future enhancements
#define NSD1_TX_CMD_P               (FIRST_CMD_EVENT + 14) 
#define NSD1_TX_CMD_S               (FIRST_CMD_EVENT + 15)

#define NSD2_RX_CMD_A               (FIRST_CMD_EVENT + 16)
#define NSD2_RX_CMD_B               (FIRST_CMD_EVENT + 17)
#define NSD2_RX_CMD_C               (FIRST_CMD_EVENT + 18) 
#define NSD2_RX_CMD_D               (FIRST_CMD_EVENT + 19)
#define NSD2_RX_CMD_E               (FIRST_CMD_EVENT + 20) // for future enhancements
#define NSD2_RX_CMD_F               (FIRST_CMD_EVENT + 21) // for future enhancements
#define NSD2_RX_CMD_P               (FIRST_CMD_EVENT + 22) 
#define NSD2_RX_CMD_U               (FIRST_CMD_EVENT + 23)

#define NSD2_TX_CMD_A               (FIRST_CMD_EVENT + 24) 
#define NSD2_TX_CMD_B               (FIRST_CMD_EVENT + 25)
#define NSD2_TX_CMD_C               (FIRST_CMD_EVENT + 26)
#define NSD2_TX_CMD_D               (FIRST_CMD_EVENT + 27)
#define NSD2_TX_CMD_E               (FIRST_CMD_EVENT + 28) // for future enhancements
#define NSD2_TX_CMD_F               (FIRST_CMD_EVENT + 29) // for future enhancements
#define NSD2_TX_CMD_P               (FIRST_CMD_EVENT + 30) 
#define NSD2_TX_CMD_S               (FIRST_CMD_EVENT + 31)

#define AES_RX_CMD_1                (FIRST_CMD_EVENT + 32)
#define AES_RX_CMD_2                (FIRST_CMD_EVENT + 33)
#define AES_RX_CMD_3                (FIRST_CMD_EVENT + 34)
#define AES_RX_CMD_4                (FIRST_CMD_EVENT + 35)
#define AES_RX_CMD_5                (FIRST_CMD_EVENT + 36)
#define AES_RX_CMD_6                (FIRST_CMD_EVENT + 37)
#define AES_RX_CMD_7                (FIRST_CMD_EVENT + 38)
#define AES_RX_CMD_8                (FIRST_CMD_EVENT + 39)
#define AES_RX_CMD_9                (FIRST_CMD_EVENT + 40)
#define AES_RX_CMD_10               (FIRST_CMD_EVENT + 41)
#define AES_RX_CMD_11               (FIRST_CMD_EVENT + 42)
#define AES_RX_CMD_12               (FIRST_CMD_EVENT + 43)
#define AES_RX_CMD_13               (FIRST_CMD_EVENT + 44)
#define AES_RX_CMD_14               (FIRST_CMD_EVENT + 45)
#define AES_RX_CMD_15               (FIRST_CMD_EVENT + 46)
#define AES_RX_CMD_16               (FIRST_CMD_EVENT + 47)
#define AES_RX_CMD_17               (FIRST_CMD_EVENT + 48)
#define AES_RX_CMD_18               (FIRST_CMD_EVENT + 49)
#define AES_RX_CMD_19               (FIRST_CMD_EVENT + 50)
#define AES_RX_CMD_20               (FIRST_CMD_EVENT + 51) 
#define AES_RX_CMD_21               (FIRST_CMD_EVENT + 52) // for future enhancements
#define AES_RX_CMD_22               (FIRST_CMD_EVENT + 53) // for future enhancements
#define AES_RX_CMD_23               (FIRST_CMD_EVENT + 54) // for future enhancements
#define AES_RX_CMD_24               (FIRST_CMD_EVENT + 55) // for future enhancements



#define AES_TX_CMD_1                (FIRST_CMD_EVENT + 56)
#define AES_TX_CMD_2                (FIRST_CMD_EVENT + 57)
#define AES_TX_CMD_3                (FIRST_CMD_EVENT + 58)
#define AES_TX_CMD_4                (FIRST_CMD_EVENT + 59)
#define AES_TX_CMD_5                (FIRST_CMD_EVENT + 60)
#define AES_TX_CMD_6                (FIRST_CMD_EVENT + 61)
#define AES_TX_CMD_7                (FIRST_CMD_EVENT + 62)
#define AES_TX_CMD_8                (FIRST_CMD_EVENT + 63)
#define AES_TX_CMD_9                (FIRST_CMD_EVENT + 64)
#define AES_TX_CMD_10               (FIRST_CMD_EVENT + 65)
#define AES_TX_CMD_11               (FIRST_CMD_EVENT + 66)
#define AES_TX_CMD_12               (FIRST_CMD_EVENT + 67)
#define AES_TX_CMD_13               (FIRST_CMD_EVENT + 68)
#define AES_TX_CMD_14               (FIRST_CMD_EVENT + 69)
#define AES_TX_CMD_15               (FIRST_CMD_EVENT + 70)
#define AES_TX_CMD_16               (FIRST_CMD_EVENT + 71)
#define AES_TX_CMD_17               (FIRST_CMD_EVENT + 72)
#define AES_TX_CMD_18               (FIRST_CMD_EVENT + 73)
#define AES_TX_CMD_19               (FIRST_CMD_EVENT + 74)
#define AES_TX_CMD_20               (FIRST_CMD_EVENT + 75)
#define AES_TX_CMD_21               (FIRST_CMD_EVENT + 76) // for future enhancements
#define AES_TX_CMD_22               (FIRST_CMD_EVENT + 77) // for future enhancements
#define AES_TX_CMD_23               (FIRST_CMD_EVENT + 78) // for future enhancements
#define AES_TX_CMD_24               (FIRST_CMD_EVENT + 79) // for future enhancements



#define LAST_CMD_EVENT              (AES_TX_CMD_24 + 1) // 1 spare 


#define MAX_KAT_COMMAND_BLOCK      LAST_CMD_EVENT // Gibt die Grenze der Kategorien an die im 
								                  // Command Block gespeichert werden  

#define MAX_EVENT_COUNTERS         MAX_KAT_COMMAND_BLOCK		

////////////////////////////////////////////////////////////////////////////////////////////////
// ETL500 alarm definitions for HMI600 alarm polling
////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Bits for hardware alarms
///////////////////////////////////////////////////////////////////////////////////
#define 	LIMITER_ACTIVE			BIT1 
#define 	ALARM_PLUG_OUT3			BIT3
 
#define 	ALARM_PLUG_OUT4			BIT4
#define 	SLAVE_BOARD_HW_ALARM	BIT5 	   
#define 	CODEC_RESET				BIT6 	
#define 	COMMON_AL_CH1			BIT7

#define		DSPA_MAIN_ALARM			BIT8
#define		DSPA_TX_ALARM			BIT9
#define		DSPA_RX_ALARM			BIT10
#define		DSPC_MAIN_ALARM			BIT11

#define		EQUALIZER_CRC_ALARM		BIT12
#define 	UART_NOT_OPERATIONAL	BIT13
#define		COMMUNICATION_TO_SLAVE	BIT14
#define		SET_RTC_DATE			BIT15
                                                      
#define 	TXRF_ALARM				BIT16
#define		COMMON_AL_CH2			BIT17
#define 	MEMDATAERR 				BIT18  
#define 	CRC_PROGMEMERR_DSPA		BIT19     

#define 	CRC_PROGMEMERR_DSPB		BIT20      
#define 	CRC_PROGMEMERR_DSPC		BIT21     
#define		NSD550_HW_ALARM_ETL		BIT22
#define		IRIG_SIGNAL_LOST		BIT23

#define		INCORRECT_RTC_DATE		BIT24
#define 	ALARM_M12V_UNDERV		BIT25
#define 	ALARM_M12V_OVERV		BIT26
#define 	ALARM_P12V_UNDERV		BIT27

#define 	ALARM_P12V_OVERV		BIT28
#define		IDLE_TIME_ALARM			BIT29        

#define		EPROM_READ_ERROR		BIT30 
#define		EPROM_SAVE_ERROR		BIT30 
#define		RESTART_OR_CHANGE_EPROM	BIT31 


///////////////////////////////////////////////////////////////////////////
// Bits for link alarms and warnings 
///////////////////////////////////////////////////////////////////////////////////
  
#define		EXT_WARNING_IN			  BIT0
#define 	SLOW_MUTE_CH1			  BIT1	   	
#define     RX_PILOT_NOT_OK_CH1		  BIT2
#define		PILOT_LEVEL_ALARM_CH1	  BIT3       

#define		SYNC_NOT_OK_CH1			  BIT4
#define     AGC_NOT_OK_CH1			  BIT5
#define		SLAVE_BOARD_WARNING		  BIT6

#define		NSD_LINK_ALARM_ETL		  BIT8
#define		NSD_WARNING_ETL			  BIT9
#define		LINK_TEST_WARNING		  BIT10

#define 	EQUIPMENT_OUT_OF_ORDER	  BIT12                                                                          
#define		EOC_ALARM_CH1			  BIT13
#define		NO_EOC_DATA_REC_CH1		  BIT14      
#define 	EQUALIZER_NEW			  BIT15

#define		DIFFERENT_FW_REV		  BIT16
#define		NO_EOC_DATA_SEND_CH1  	  BIT17
#define		NO_EOC_DATA_SEND_CH2	  BIT19

#define		NO_EOC_DATA_REC_CH2		  BIT20
#define     EOC_ALARM_CH2			  BIT21
#define		TEST_CONFIGURATION_ACTIVE BIT22
#define		EQUALIZER_ACTION		  BIT23

#define 	DUMMYLOAD_PLUGGED		  BIT24
#define 	SLOW_MUTE_CH2			  BIT25	   	
#define     RX_PILOT_NOT_OK_CH2		  BIT26
#define		PILOT_LEVEL_ALARM_CH2	  BIT27       

#define		REVERSED_CHANNELS		  BIT28
#define     AGC_NOT_OK_CH2			  BIT29 
#define		ACT_ON_RT_PENDING_CH1 	  BIT30
#define		ACT_ON_RT_PENDING_CH2 	  BIT31


///////////////////////////////////////////////////////////////
/// Masks
#define		LINK_ALA1				(SLOW_MUTE_CH1 | RX_PILOT_NOT_OK_CH1 | PILOT_LEVEL_ALARM_CH1 | SYNC_NOT_OK_CH1 )
#define		LINK_ALA2				(AGC_NOT_OK_CH1 | NSD_LINK_ALARM_ETL)

#define		LINK_ALA3				(SLOW_MUTE_CH2 | RX_PILOT_NOT_OK_CH2 | PILOT_LEVEL_ALARM_CH2 | REVERSED_CHANNELS )
#define 	LINK_ALA4				(AGC_NOT_OK_CH2)

#define 	LINK_ALARMS				(LINK_ALA1 | LINK_ALA2 | LINK_ALA3 | LINK_ALA4) 

#define		W1						(EXT_WARNING_IN | EQUIPMENT_OUT_OF_ORDER | EQUALIZER_NEW | DIFFERENT_FW_REV)
#define		W2						(SLAVE_BOARD_WARNING | DUMMYLOAD_PLUGGED | NSD_WARNING_ETL | LINK_TEST_WARNING)
#define		W3						(ACT_ON_RT_PENDING_CH1 | ACT_ON_RT_PENDING_CH2 | TEST_CONFIGURATION_ACTIVE | EQUALIZER_ACTION)
#define		W4						(EOC_ALARM_CH1 | NO_EOC_DATA_REC_CH1 | EOC_ALARM_CH2 | NO_EOC_DATA_REC_CH2)
#define		W5						(NO_EOC_DATA_SEND_CH1 | NO_EOC_DATA_SEND_CH2)
#define 	WARNINGS				(W1 | W2 | W3 | W4 | W5)

/* PUBLIC FUNCTIONS ***********************************************************/

#endif

