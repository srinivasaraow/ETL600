/*-------------------------------------------------------------------------*
 * $Workfile: EventRec.c $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Christian Leeb
 * Remarks      :  
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /branches/Source O4CV/P4LT/Master_DSP/C/EventRec.c $ 
 * 
 * 21    2.11.07 16:03 Chhamaa
 * MAX_EVENTS_PER_INVOCATION reduced from 5 to 4
 * 
 * 16    12.10.05 14:35 Chchlee
 * MAX_COUNTERS --> MAX_EVENT_COUNTERS
 * 
 * 15    4.10.05 14:58 Chchlee
 * Ignore events if event recorder is disabled
 * 
 * 14    4.10.05 10:52 Chchlee
 * comments added
 * 
 * 13    17.08.05 17:06 Chchlee
 * Issue 149 fixed (lost command events were counted by mistake)
 * 
 * 12    10.08.05 9:52 Chchlee
 * type names cleaned up
 * 
 * 11    21.07.05 13:49 Chchlee
 * serve watchdog now in eventRec_EmptyStack()
 * 
 * 10    13.07.05 10:55 Chchlee
 * EVENTSTACK_SIZE and PRE_EVENTSTACK_SIZE increased to 1024
 * 
 * 9     30.06.05 8:15 Chchlee
 * cleaned up
 * 
 * 8     29.06.05 13:44 Chmibuh
 * same STANDARD_DATE & TIME as for RTC
 * 
 * 7     28.06.05 13:53 Chchlee
 * ASSERTs consolidated
 * 
 * 6     13.06.05 17:23 Chchlee
 * 
 * 5     13.06.05 7:33 Chchlee
 * additional manipulation events added
 * 
 * 4     9.06.05 20:00 Chchlee
 * WARN_Event_Recorder_block_rotation added
 * 
 * 3     11.04.05 18:32 Chchlee
 * Concurrent flash access bug fixed
 * 
 * 2     16.02.05 10:36 Chchlee
 * Upgrade to Ptty2
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 17    5.01.05 9:31 Chchlee
 * Now supporting up to 260 counters
 * 
 * 16    4.01.05 15:58 Chchlee
 * eventRec_readRAMCounters now returns unsigned long *
 * 
 * 15    4.01.05 14:20 Chchlee
 * #if MAX_EVENT_COUNTERS > 64 
 * 
 * 14    3.01.05 9:28 Chchlee
 * 
 * 13    30.12.04 13:16 Chchlee
 * monitoring added
 * 
 * 12    27.12.04 10:56 Chchlee
 * 
 * 11    23.12.04 9:21 Chchlee
 * 
 * 10    15.12.04 10:41 Chchlee
 * 
 * 9     15.12.04 10:26 Chchlee
 * 
 * 8     15.12.04 10:06 Chchlee
 * First tested version
 * 
 * 7     13.12.04 9:57 Chchlee
 * 
 * 6     10.12.04 13:05 Chchlee
 * Time & Date fixed
 * 
 * 5     10.12.04 8:42 Leeb02
 * 
 * 4     29.11.04 7:59 Leeb02
 * 
 * 3     26.11.04 18:19 Leeb02
 * 
 * 2     26.11.04 7:32 Leeb02
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/
#include "eventRec.h"    

#include "epromlow.h" 

#include "rtc.h"
#include "diag.h"

#include "IO_HW_Buffer.h"  // for watchdog

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// the 2 event sectors
#define EVENT_SECTOR1           SECTOR_SA69
#define EVENT_SECTOR2           SECTOR_SA70

// sector marks for event recorder
#define UNMARKED_SECTOR_MARK    0xFF 
#define EVENT_SECTOR_MARK		0xF0 
#define EMPTY_SECTOR_MARK		0x10 

#define		BR_START_MARK_OFFSET	0x08    // Offset of block rotation mark
#define		BR_START_MARK			0x33	// block rotation mark 

#define CMD_EVENT_DATA_OFFSET    EVENTDATA_OFFSET // begin of command events
#define ALM_EVENT_DATA_OFFSET   (EVENTDATA_OFFSET + (MAX_CMD_EVENTS * BYTES_PER_EVENT))// begin of command events

#define COUNTER_OFFSET           0x0000F3D0   // begin of counters, number of counters must be <= 260

#if MAX_EVENT_COUNTERS > 260 
	#error MAX_EVENT_COUNTERS > 260 
#endif

//******** VORSICHT ***************
// Aus Rechenzeitgründen dürfen die unteren 2 Konstanten nur
// den Wert einer 2er Potenz annehmen
#define EVENTSTACK_SIZE       (1<<10)  // 1024
#define PRE_EVENTSTACK_SIZE   (1<<10)  // 1024  
//******** VORSICHT ***************

#define STANDARD_DATE 0x01120900;	// 09.12.2001 - same init date as for RTC -> initInternalTime() rtc.c
#define STANDARD_TIME 0x0B550000;	// 11:55		-""-
	
#define DO_NOT_RESET  (-1) // for counter reset

// Speicherblöcke
#define CMD_EVENTS_BLOCK 0
#define ALM_EVENTS_BLOCK 1 

#define N_EVENT_BLOCK_TYPES     (ALM_EVENTS_BLOCK + 1)

/* LOCAL TYPE DECLARATIONS ***************************************************/

// Block rotation state
typedef enum 
{
	e_BlockRotationState_Idle,
	e_BlockRotationState_WriteStartMark,
	e_BlockRotationState_StartBlockErase,
	e_BlockRotationState_TestEndOfErase,
	e_BlockRotationState_MarkBlock,
	e_BlockRotationState_CopyCmdEvents,
	e_BlockRotationState_CopyAlmEvents,
	e_BlockRotationState_DoCmdCounters,
	e_BlockRotationState_DoAlmCounters,
	e_BlockRotationState_CopyCounters,
	e_BlockRotationState_BlockRotationEnd
}
e_EventRecState;

/* LOCAL MACRO DEFINITIONS ***************************************************/

#define SECTOR_MARKS_ARE(mark_a, mark_b)\
	   ((sectorMark[0] == (mark_a))   && (sectorMark[1] == (mark_b))       ||\
	    (sectorMark[0] == (mark_b))   && (sectorMark[1] == (mark_a))          )

#define GET_TIME()\
				   (  g_pInternalTime_Decimal->milisecond +                \
				      g_pInternalTime_Decimal->second           * 1000   | \
					((g_pInternalTime_BCD->minute & MASK_7LSB)  << 16)   | \
					((g_pInternalTime_BCD->hour	  & MASK_6LSB)  << 24))

#define GET_DATE()\
				   (((g_pInternalTime_BCD->year   & MASK_8LSB)  << 24)   | \
					((g_pInternalTime_BCD->month  & MASK_5LSB)  << 16)   | \
					((g_pInternalTime_BCD->date	  & MASK_6LSB)  <<  8))

/* LOCAL VARIABLES ***********************************************************/

static struct 
{   
	BOOL eventRecorderIsEnabled;                   // Aktiviert/Deaktiviert den Eventrecorder 
	BOOL isWriting;                                // Zur Kommunikation mit der Schreibroutine
	BOOL writeEnable;  
	long ResetCounter;                             // Nummer des Zähler, der zurückgesetzt werden soll

	long BlockNextPlace[N_EVENT_BLOCK_TYPES];      // Gibt den nächsten freien Platz im Block an.
	long OldBlockNextPlace[N_EVENT_BLOCK_TYPES];   // Gibt den nächsten freien Platz im Altblock

	long BlockOffset[N_EVENT_BLOCK_TYPES];		   // Offsets der Speicherblöcke 
	long MaxEvents[N_EVENT_BLOCK_TYPES];           // Grösse der Blöcke

	long OldSectorAddress;                         // Addresse des Kopierblocks 
	long CurrentSectorAddress;                     // Addresse des aktuellen (neuen) Blocks 

	long EventsToCopy[N_EVENT_BLOCK_TYPES];        // Anzahl zu kopierender Events während BR        
	long ReadStartAddress[N_EVENT_BLOCK_TYPES];    // Anfangsaddresse der zu kopierenden Events in der BR

	long numberOfLostEvents;    		           // gibt an ob Eventpuffer voll ist    

	BOOL blockRotationIsOngoing;                   // Gibt an ob BR im Gange
    e_EventRecState state;	                       // Steuervariable für BR
}
g_ERStatus =    
{
	TRUE,                      // Eventrecorder ist aktiv
	FALSE,                     // Anfangszustand nicht schreibend                                     
	TRUE,                      // Schreiben freigeben                                     
	DO_NOT_RESET,              // Es wird kein Zähler zurückgesetzt

	{0,             0},        // Muss bei Initialisierung korrekt gesetzt werden
	{MINIMUM_EVENTS,MINIMUM_EVENTS},        

	{CMD_EVENT_DATA_OFFSET,
	 ALM_EVENT_DATA_OFFSET},   // Blockoffset

	{MAX_CMD_EVENTS, 
	 MAX_ALM_EVENTS},          // Grösse der Blöcke
	
	0,                         // Epromadresse des aktuellen Ausweichblocks
	0,                         // Epromadresse des aktuellen Blocks

	{0,0},
	{0,0},

	0,                         // Keine Events verloren
	FALSE,                     // no ongoing blockrotation 
	e_BlockRotationState_Idle
};              


static SEGMENT_EXT_DATA struct  
{
   	unsigned long  category;
	unsigned long  data;
	BOOL           doCount;

}g_PreEventStack[PRE_EVENTSTACK_SIZE];        


static SEGMENT_EXT_DATA struct         
{                                       
	long        address; 
	t_EventData eventData;

}g_EventStack[EVENTSTACK_SIZE];    
      
static SEGMENT_EXT_DATA t_EventCounter g_ROMCounters[MAX_EVENT_COUNTERS];
static unsigned long                   g_Counters   [MAX_EVENT_COUNTERS]; 

static long g_NewestEventStackMarker;     // Zeigt auf den leeren Platz für das nächste Event
static long g_OldestEventStackMarker;     // Zeigt auf das älteste Event  

static long g_NewestPreEventStackMarker;   // Zeigt auf den leeren Platz für das nächste Event im Pre-Eventstack
static long g_OldestPreEventStackMarker;   // Zeigt auf das älteste Event im PreEventstack

static long g_CurrentEventSector;
static long g_EmptySector;
static long g_TempSectorForBR;

static BOOL g_Mutex; // für Eprom Arbitrierung

static t_internalTime * g_pInternalTime_Decimal;
static t_internalTime * g_pInternalTime_BCD;

static long g_copyCounter;  

static BOOL g_ongoingBlockRotationWarning;

/* LOCAL FUNCTIONS ***********************************************************/


static void pushEvent(long address, long *dataPointer);
static inline void shrink12CharsTo3Longs(long *data); // in/out
static void initEventSectors(void);
static BOOL createEvent(unsigned long category, unsigned long data, long block);
static void countEvents(long            startAddr, 
						long           *total, 
						long           *singleCount, 
						t_EventCounter *romCounter, 
						long            startOnly, 
						long            max);

static void loadROMCounters(long blockAddress, t_EventCounter *romCounter);

static BOOL copyEvents(unsigned long eventBlockType);
static BOOL updateCounters(unsigned long eventBlockType);
static  void epromTimeout(void);


/* IMPLEMENTATION ************************************************************/
 
/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* RecordEvent      			 											*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Zeichnet ein Event auf (Schreibt es in den Pre-EventStack)
 *
 * Input
 * -----   
 * kat - Kategorienummer 
 * data - Eventdaten
 * count - Bei TRUE wird der Kategoriezähler um eins erhöht
 * 
 * Change History
 * --------------
 * 01.12.1997 Clemens Bürgi  Erste Version
 *
 */    
//
// BEWARE !!!!! 
//----------------
// DO NOT COUNT EVENTS WITH KAT > 20!! YOU WILL WRITE TO MEMORY ABOVE THE ARRAY BORDERS.
// YOU CAN CHANGE THE #DEFINE MAX_EVENT_COUNTERS to add more counters
// Best regards C.Sidler

void eventRec_recordEvent(unsigned long category, 
						  unsigned long data, 
						  BOOL doCount)
{
	if(g_ERStatus.eventRecorderIsEnabled) // ignore events if event recorder is disabled
	{
		if (((g_NewestPreEventStackMarker + 1) & (PRE_EVENTSTACK_SIZE-1)) == g_OldestPreEventStackMarker)   // Bei Ueberlauf.. 
		{
			g_ERStatus.numberOfLostEvents++;	// Eventslost erzeugen
		}
		else
		{
			g_PreEventStack[g_NewestPreEventStackMarker].category   = category;
			g_PreEventStack[g_NewestPreEventStackMarker].data       = data;
			g_PreEventStack[g_NewestPreEventStackMarker].doCount    = doCount; 

			g_NewestPreEventStackMarker = ++g_NewestPreEventStackMarker & (PRE_EVENTSTACK_SIZE-1);
		}
	}
} 
 

t_HMIEventData SEGMENT_EXT_CODE * eventRec_readEvent (long *p)
{
	static t_HMIEventData hmiEventData; // for HMI read event

	if (g_Mutex) // eprom busy, avoid corrupt read operation during event writing or block rotation
	{ 
 		hmiEventData.controlInformation =  g_ERStatus.BlockNextPlace[CMD_EVENTS_BLOCK]; 	
 		hmiEventData.controlInformation |= g_ERStatus.BlockNextPlace[ALM_EVENTS_BLOCK] << 16;  

		hmiEventData.controlInformation &= ~ERPOM_NOT_BUSY_FLAG; // Meldet dem MMI Eprom Busy 
	} 
	else 
	{ 
		// Event lesen 
 		//g_Mutex = TRUE; // Eprom für andere Zugriffe sperren. Not needed since we do not have a preemptive scheduler     
		BOOL bSuccess = readLongFromEprom(g_CurrentEventSector + p, (long*)&hmiEventData.eventData.categoryAndDate);

		if (bSuccess)
		{
			bSuccess = readLongFromEprom(g_CurrentEventSector + p +  1 * NR_OF_BYTES_IN_LONG, (long*)&hmiEventData.eventData.time);
			ASSERT(bSuccess); // must not be busy now

			bSuccess = readLongFromEprom(g_CurrentEventSector + p +  2 * NR_OF_BYTES_IN_LONG, (long*)&hmiEventData.eventData.data);
			ASSERT(bSuccess); // must not be busy now

	        //g_Mutex = FALSE;  // Eprom wieder freigeben. Not needed since we do not have a preemptive scheduler  
 			
 			// Der 4. long setzt sich wie folgt zusammen : (LSByte = Byte 0) 
 			// Byte 0,1 : BlockNextPlace[COMMAND_BLOCK]
 			// Byte 2,3 :(ohne Bit 31) BlockNextPlace[OTHER_EVENTS_BLOCK]
 			// Bit 31 : 1 = Eprom not busy, 0 = Eprom busy

 			hmiEventData.controlInformation =  g_ERStatus.BlockNextPlace[CMD_EVENTS_BLOCK]; 	
 			hmiEventData.controlInformation |= g_ERStatus.BlockNextPlace[ALM_EVENTS_BLOCK] << 16;  

 			hmiEventData.controlInformation |= ERPOM_NOT_BUSY_FLAG; // Meldet dem MMI Eprom Not Busy 
		}
		else
		{	// flash busy
 			hmiEventData.controlInformation =  g_ERStatus.BlockNextPlace[CMD_EVENTS_BLOCK]; 	
 			hmiEventData.controlInformation |= g_ERStatus.BlockNextPlace[ALM_EVENTS_BLOCK] << 16;  

			hmiEventData.controlInformation &= ~ERPOM_NOT_BUSY_FLAG; // Meldet dem MMI Eprom Busy 
		}
	}

	return &hmiEventData;
}



BOOL SEGMENT_EXT_CODE eventRec_resetCounter(unsigned long counterNumber)
{
	if (counterNumber < MAX_EVENT_COUNTERS)	
	{
		g_ERStatus.ResetCounter = (long)counterNumber;
		return TRUE;
	}
	else
	{
		return FALSE; // counter number not supported
	}
}

t_EventCounter SEGMENT_EXT_CODE * eventRec_readROMCounters(void)
{
	return g_ROMCounters;
}

unsigned long SEGMENT_EXT_CODE * eventRec_readRAMCounters(void)
{
	return g_Counters;
}

 
/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* WriteEventsToEprom 			 											*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Holt die Events aus dem Puffer und schreibt sie ins Eprom
 *
 * Change History
 * --------------
 * 29.07.1997 Clemens Bürgi  Erste Version
 *
 * Calls to other functions
 * ------------------------
 * writeByteToEprom         
 * epromWriteStatus
 */   
void eventRec_writeEventsToEprom(void)    
{
	static enum 
	{
		stateReadEventStack,
		stateWriteByte,
		stateDoWait

	} state = stateReadEventStack;

    switch (state)
    {   
    	static unsigned long byteCounter;          
	    static long events[BYTES_PER_EVENT];    

		static long *addressInEprom;
		static BOOL blockRotationIsOngoing;  // BlockChange zu Begin eines WriteZyklus abspeichern, pro Zyklus ist diese Wert massgebend
								             // Andernfalls kommt es zu Deadlocks beim Epromzugriff

    	case stateReadEventStack: // Eventstack lesen
    	{   
    		if (g_ERStatus.writeEnable)
    		{
    			g_ERStatus.isWriting = TRUE;

    			blockRotationIsOngoing = g_ERStatus.blockRotationIsOngoing;

    			if ((!g_Mutex) || blockRotationIsOngoing)
    	   		{   
    				if (g_NewestEventStackMarker - g_OldestEventStackMarker) // if any event in event stack
    				{   
    					g_Mutex = TRUE;  // Eprom reservieren
    					addressInEprom = (long*)(g_EventStack[g_OldestEventStackMarker].address);

    					events[0]  = (g_EventStack[g_OldestEventStackMarker].eventData.categoryAndDate     )  & MASK_8LSB; 
    					events[1]  = (g_EventStack[g_OldestEventStackMarker].eventData.categoryAndDate >> 8)  & MASK_8LSB;
    					events[2]  = (g_EventStack[g_OldestEventStackMarker].eventData.categoryAndDate >> 16) & MASK_8LSB;
    					events[3]  = (g_EventStack[g_OldestEventStackMarker].eventData.categoryAndDate >> 24) & MASK_8LSB;
    				
    					events[4]  = (g_EventStack[g_OldestEventStackMarker].eventData.time           )  & MASK_8LSB;
    					events[5]  = (g_EventStack[g_OldestEventStackMarker].eventData.time       >> 8)  & MASK_8LSB;
    					events[6]  = (g_EventStack[g_OldestEventStackMarker].eventData.time       >> 16) & MASK_8LSB;
    					events[7]  = (g_EventStack[g_OldestEventStackMarker].eventData.time       >> 24) & MASK_8LSB;
    					 
    					events[8]  = (g_EventStack[g_OldestEventStackMarker].eventData.data           )  & MASK_8LSB; 
    					events[9]  = (g_EventStack[g_OldestEventStackMarker].eventData.data       >> 8)  & MASK_8LSB;
    					events[10] = (g_EventStack[g_OldestEventStackMarker].eventData.data       >> 16) & MASK_8LSB;
    					events[11] = (g_EventStack[g_OldestEventStackMarker].eventData.data       >> 24) & MASK_8LSB;

    					g_OldestEventStackMarker = ++g_OldestEventStackMarker & (EVENTSTACK_SIZE-1);       // pop event stack
    					
						state = stateWriteByte;    // zum Schreiben weitergehen
    				} // end of if (g_NewestEventStackMarker-g_OldestEventStackMarker)   
    				else
    				{ 
    					g_ERStatus.isWriting = FALSE; 
    				}
    			} // end of if (EpromBusy == NOT_BUSY)
    		} 
    		else
    		{
    			g_ERStatus.isWriting = FALSE;
    		}
 	  		break; 
    	}

    	case stateWriteByte:  // Byte schreiben
    	{   
	   		if (writeByteToEprom(events[byteCounter],
								 addressInEprom,
								 FALSE)) // non blocking write operation
			{
		   		state = stateDoWait; 	 
			}
	   		break;
    	}

    	case stateDoWait:  // Testen ob Byte schon fertig geschrieben     
    	{   
    		e_EpromWriteStatus epromWriteStatus = writeByteToEpromStatus(events[byteCounter],
				                                                         addressInEprom);

    		if (e_EpromWriteStatus_Ok == epromWriteStatus) 	// überprüfe ob Schreiben beendet
			{
				if (++byteCounter > sizeof(events) - 1)  // Zähler erhöhen, wenn Zähler grösser als length-11 -> fertig!  
				{                         
					byteCounter = 0;
					state = stateReadEventStack;

					if (!blockRotationIsOngoing) 
					{ 
						g_Mutex = FALSE; // Eprom wieder freigeben, if not done by BR handler
					}   
				}
				else
			    { 
					addressInEprom++;               // Addresse erhöhen
   					state = stateWriteByte; 		// beim nächsten Durchgang wieder Schreiben 
   			    }
   			}    
			else 
			{  
				if(e_EpromWriteStatus_Timeout == epromWriteStatus) // Timeout ? 
				{                                          
					epromTimeout();
					byteCounter = 0;
					state = stateReadEventStack;                                     
				}
			}      
			break;
    	}         
    	
		default: ASSERT (FALSE);

    }   // Ende switch(state)                 
                              
} // end of WriteEventToEprom

                                      

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* shrink12CharsTo3Longs															*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Subroutine for use in CreateEvent and MainEventrecorder
 */                                           
inline void shrink12CharsTo3Longs(long *data) // in/out
{
    register int index;

	for (index = 0; index < NR_OF_BYTES_IN_LONG-1; index++) // longs zusammensetzen
	{
		data[index] = (data[NR_OF_BYTES_IN_LONG*index]       )  + 
			          (data[NR_OF_BYTES_IN_LONG*index+1] << 8)  +
				   	  (data[NR_OF_BYTES_IN_LONG*index+2] << 16) + 
					  (data[NR_OF_BYTES_IN_LONG*index+3] << 24);  
	}	
} // end of shrink12CharsTo3Longs 
       

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* CreateEvent		 			 											*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Generiert ein Event. 
 *
 * Input
 * -----   
 * Kategorie   
 * data : Eventdaten
 * Block : Block 0 oder 1. Block 0 normalerweise nur für NSD commands
 *
 * Output                                                     
 * ------              
 * legt das Event in den Eventpuffer        
 *    
 * Return 
 * ------              
 * TRUE if event is in stack, FALSE if event lost
 * 
 * Change History
 * --------------
 * 29.07.1997 Clemens Bürgi  Erste Version
 */                                                              
static BOOL SEGMENT_EXT_CODE createEvent(unsigned long category, unsigned long data, long block)
{               
	
	if (((g_NewestEventStackMarker + 1) & (EVENTSTACK_SIZE-1)) == g_OldestEventStackMarker) // Eventpuffer voll ?
	{     // Der Puffer ist voll, falls der NeustesEventZeiger+1 mod die Grösse gleich dem ÄltestEventZeiger ist.
		g_ERStatus.numberOfLostEvents++;	// wenn ja, ist das Event verloren, zählen;
		return FALSE;
	}
	else    // sonst Event in den Puffer legen
	{
		if(g_ERStatus.BlockNextPlace[block] + 1 >= g_ERStatus.MaxEvents[block]) // if block is full
		{   
			if(g_EmptySector) //@CL@ BR ongoing or no empty block available??
			{  
				long otherBlock;   
				
				// initiate block rotation
				g_ERStatus.blockRotationIsOngoing = g_ongoingBlockRotationWarning = TRUE;

				g_ERStatus.state = e_BlockRotationState_WriteStartMark;  

				// otherBlock ist der nicht Übergebene
				otherBlock = (block == CMD_EVENTS_BLOCK) ? ALM_EVENTS_BLOCK : CMD_EVENTS_BLOCK; 
				
			    g_ERStatus.OldBlockNextPlace[block]      = g_ERStatus.BlockNextPlace[block] + 1; // Nur der auslösende Block
		        g_ERStatus.OldBlockNextPlace[otherBlock] = g_ERStatus.BlockNextPlace[otherBlock]; 
            
				if (g_ERStatus.BlockNextPlace[otherBlock] < MINIMUM_EVENTS)  // Sind im otherBlock weniger als Minimum_Events ?
				{
	           	  	g_ERStatus.EventsToCopy[otherBlock] = g_ERStatus.BlockNextPlace[otherBlock];   
					g_ERStatus.ReadStartAddress[otherBlock] = g_ERStatus.CurrentSectorAddress + 
						                                      g_ERStatus.BlockOffset[otherBlock];    	
				}	  
				else
				{
				 	g_ERStatus.EventsToCopy[otherBlock] = MINIMUM_EVENTS; 
					g_ERStatus.ReadStartAddress[otherBlock] = g_ERStatus.CurrentSectorAddress +  
						                                      g_ERStatus.BlockOffset[otherBlock] + 
															  BYTES_PER_EVENT*(g_ERStatus.BlockNextPlace[otherBlock] - 
															  MINIMUM_EVENTS);        
				}   

		     	g_ERStatus.EventsToCopy[block] = MINIMUM_EVENTS;
	        	g_ERStatus.ReadStartAddress[block] = g_ERStatus.CurrentSectorAddress +  
					                                 g_ERStatus.BlockOffset[block] + 
													 BYTES_PER_EVENT*(g_ERStatus.MaxEvents[block] - MINIMUM_EVENTS);

				g_EventStack[g_NewestEventStackMarker].address = g_ERStatus.CurrentSectorAddress + 
					                                        g_ERStatus.BlockOffset[block] + 
															BYTES_PER_EVENT*g_ERStatus.BlockNextPlace[block]; // Normale Addresse zuweisen
				g_ERStatus.writeEnable = FALSE; // Schreiben anhalten

				g_TempSectorForBR = g_EmptySector; 
				g_EmptySector = 0; 

				g_ERStatus.OldSectorAddress = g_ERStatus.CurrentSectorAddress;
				g_ERStatus.CurrentSectorAddress = g_TempSectorForBR;

				g_ERStatus.BlockNextPlace[block]      = MINIMUM_EVENTS;
				g_ERStatus.BlockNextPlace[otherBlock] = g_ERStatus.EventsToCopy[otherBlock];

			   	g_EventStack[g_NewestEventStackMarker].eventData.time            = GET_TIME();
				g_EventStack[g_NewestEventStackMarker].eventData.data            = data;
				g_EventStack[g_NewestEventStackMarker].eventData.categoryAndDate = category + GET_DATE();   
				
				g_NewestEventStackMarker = ++g_NewestEventStackMarker & (EVENTSTACK_SIZE-1); // Puffer Zeiger nachführen

				return TRUE;
			}  
			else 
			{   
				if(g_ERStatus.blockRotationIsOngoing)
				{
					g_ERStatus.numberOfLostEvents++; // Event verlieren, warten bis BR zu Ende
				}
				else // BR nicht möglich. Disable event recorder.
				{
					g_ERStatus.eventRecorderIsEnabled = g_ongoingBlockRotationWarning = FALSE;  
				}	

				return FALSE;
			}	// if (g_EmptySector)			   
		}	 
		else // normal path
		{	 	
			g_EventStack[g_NewestEventStackMarker].address = g_ERStatus.CurrentSectorAddress + 
				                                        g_ERStatus.BlockOffset[block] + 
														BYTES_PER_EVENT*g_ERStatus.BlockNextPlace[block]; // Normale Addresse zuweisen
			g_ERStatus.BlockNextPlace[block]++; 

			g_EventStack[g_NewestEventStackMarker].eventData.time            = GET_TIME();
			g_EventStack[g_NewestEventStackMarker].eventData.data            = data;
			g_EventStack[g_NewestEventStackMarker].eventData.categoryAndDate = category + GET_DATE();   

			g_NewestEventStackMarker = ++g_NewestEventStackMarker & (EVENTSTACK_SIZE-1); // pop event stack

			return TRUE;
		}	
	}
} // end of CreateEvent           
     

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* pushEvent		 			 											*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * legt ein Event in den Puffer.Wird für die BlockChange gebraucht.   
 * Überprüft NICHT ob der Puffer voll ist.
 *
 * Input
 * -----  
 * Addresse für Eprom 
 * data : Eventdaten
 *
 * Output                                                     
 * ------              
 * legt das Event in den Eventpuffer
 *                                                                                       
 * Change History
 * --------------
 * 06.08.1997 Clemens Bürgi  Erste Version
 */                                                              
static void SEGMENT_EXT_CODE pushEvent(long address, long *dataPointer)
{                  
    g_EventStack[g_NewestEventStackMarker].address                   = address;
	g_EventStack[g_NewestEventStackMarker].eventData.time            = dataPointer[1];
	g_EventStack[g_NewestEventStackMarker].eventData.data            = dataPointer[2];
	g_EventStack[g_NewestEventStackMarker].eventData.categoryAndDate = dataPointer[0];  
	
	g_NewestEventStackMarker = ++g_NewestEventStackMarker & (EVENTSTACK_SIZE-1); // push event stack
} // end of pushEvent                                      



/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* eventRec_activate 			 											*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Eventrecorder Hauptroutine. Generiert Events, schreibt sie in den Eventpuffer
 * und überwacht den BlockChange.
 *
 * 
 * Change History
 * --------------
 * 29.07.1997 Clemens Bürgi  Erste Version
 *
 * Calls to other functions
 * ------------------------
 * CreateEvent
 * pushEvent
 */          


#define MAX_EVENTS_PER_INVOCATION  4

void SEGMENT_EXT_CODE eventRec_activate(void)
{
	if (g_ERStatus.eventRecorderIsEnabled) // wenn der Eventrecorder aktiviert ist...
	{
		// process PreEvent Stack. Pop at most MAX_EVENTS_PER_INVOCATION events and call createEvent
		long i = 0;

		if (g_NewestPreEventStackMarker != g_OldestPreEventStackMarker) // liegt etwas auf dem Stack ?
		{
			do
			{
				if (createEvent(g_PreEventStack[g_OldestPreEventStackMarker].category,
								g_PreEventStack[g_OldestPreEventStackMarker].data,
								g_PreEventStack[g_OldestPreEventStackMarker].category >= MAX_KAT_COMMAND_BLOCK ? 
								                                             ALM_EVENTS_BLOCK : CMD_EVENTS_BLOCK)) 
				{
					// count only if event was successfully created 
					if ( g_PreEventStack[g_OldestPreEventStackMarker].doCount                   &&
						(g_PreEventStack[g_OldestPreEventStackMarker].category < MAX_EVENT_COUNTERS)     )
					{ 
						g_Counters[g_PreEventStack[g_OldestPreEventStackMarker].category]++; 
					}
				}

				g_OldestPreEventStackMarker = ++g_OldestPreEventStackMarker & (PRE_EVENTSTACK_SIZE-1); // pop pre-event stack

			} // while stack not empty and loop less than MAX_EVENTS_PER_INVOCATION executed 
			while ((g_NewestPreEventStackMarker != g_OldestPreEventStackMarker) && (i++ < MAX_EVENTS_PER_INVOCATION));  		
		}

    } // end of if (g_ERStatus.eventRecorderIsEnabled)                       
} // end of MainEventRecorder


void SEGMENT_EXT_CODE eventRec_activateBlockRotationHandler(void)
{
	if (g_ERStatus.eventRecorderIsEnabled) // wenn der Eventrecorder aktiviert ist...
	{
		// Handle counter reset
		if (g_ERStatus.ResetCounter != DO_NOT_RESET) 
		{
			g_Counters[g_ERStatus.ResetCounter] = 0; // Zähler auf Null

			createEvent(ALARM_EVENT_RESET_COUNTER,
					    g_ERStatus.ResetCounter,
						g_ERStatus.ResetCounter < MAX_KAT_COMMAND_BLOCK ? CMD_EVENTS_BLOCK: ALM_EVENTS_BLOCK);	

			g_ROMCounters[g_ERStatus.ResetCounter].date    = GET_DATE();
			g_ROMCounters[g_ERStatus.ResetCounter].time    = GET_TIME();
			g_ROMCounters[g_ERStatus.ResetCounter].counter = 0;

			g_ERStatus.ResetCounter = DO_NOT_RESET; 	
		}
		                                 
		// Falls Events verloren gegangen und Platz auf dem Stack, EventLost generieren
	    if ((g_ERStatus.numberOfLostEvents != 0) &&  
	      (((g_NewestEventStackMarker + 1) & (EVENTSTACK_SIZE-1)) != g_OldestEventStackMarker))
	    {
			if(!g_ERStatus.blockRotationIsOngoing) 
			{ 
				// Während BW keiner numberOfLostEvents erzeugen, weil sonst bei ueberlast sehr viele lost events erzeugt werden
	    		createEvent(ALARM_EVENT_EVENTS_LOST,
					        g_ERStatus.numberOfLostEvents,
							ALM_EVENTS_BLOCK);

				g_ERStatus.numberOfLostEvents = 0; // EventsLost Zähler zurücksetzen
			}
	    }      
	    
	    // block rotation state machine              
		switch(g_ERStatus.state)
		{
			case e_BlockRotationState_Idle:
				break;

			case e_BlockRotationState_WriteStartMark:
			{	 
				if (!g_ERStatus.isWriting && !g_Mutex)  // Warten bis nicht mehr geschrieben wird
				{  

					// BW Startmarke schreiben
					if (writeByteToEprom(BR_START_MARK, 
						                 (long*)(g_ERStatus.OldSectorAddress + BR_START_MARK_OFFSET),
						                 FALSE)) 
					{
						g_Mutex = TRUE; // Während der BR keine anderen ans Flash lassen  
						g_ERStatus.state = e_BlockRotationState_StartBlockErase;
					}
				}
			break;
			} 

			case e_BlockRotationState_StartBlockErase: 
			{	
				switch(writeByteToEpromStatus(BR_START_MARK, (long*)(g_ERStatus.OldSectorAddress + BR_START_MARK_OFFSET))) // fertig geschrieben ?
				{
				 	case e_EpromWriteStatus_Ok:
					  
						if(eraseBlock(g_ERStatus.CurrentSectorAddress))// Block löschen
						{
				 			g_ERStatus.state = e_BlockRotationState_TestEndOfErase;  // Weiter mit ..
						}
				 		break;	
				 	

					case e_EpromWriteStatus_Timeout: // Eprom timeout 
						epromTimeout();
				 		break;	

					// default e_EpromWriteStatus_AwaitWrite
				 }

			break;
			}

			case e_BlockRotationState_TestEndOfErase: 
			{    
				 switch(eraseBlockStatus(g_ERStatus.CurrentSectorAddress)) // fertig gelöscht ?
				 {
				 	case e_EraseBlockStatus_Ok:

						// ER Blockmarke schreiben
						if (writeByteToEprom(EVENT_SECTOR_MARK, 
							                 (long*)(g_ERStatus.CurrentSectorAddress), 
							                 FALSE))
						{
				 			g_ERStatus.state = e_BlockRotationState_MarkBlock;  // Weiter mit ..
						}
				 		break;	

					case e_EraseBlockStatus_Timeout: // Eprom timeout 

						epromTimeout();
				 		break;	
				 	
					// default e_EraseBlockStatus_AwaitErase
				 }
			break;	   
			}

			case e_BlockRotationState_MarkBlock:

				switch(writeByteToEpromStatus(EVENT_SECTOR_MARK, 
					                           (long*)(g_ERStatus.CurrentSectorAddress))) // fertig gelöscht ?
				 {
				 	case e_EpromWriteStatus_Ok:
					  
						g_ERStatus.writeEnable = TRUE; // Schreiben wieder ermöglichen
						g_ERStatus.state = e_BlockRotationState_CopyCmdEvents;  // Weiter mit ..
				 		break;	
				 	

					case e_EpromWriteStatus_Timeout: // Eprom timeout 
						
						epromTimeout();
				 		break;	

					// default e_EpromWriteStatus_AwaitWrite
				 }
				break;

			case e_BlockRotationState_CopyCmdEvents:  // Block 0 kopieren 
				
				if (copyEvents(CMD_EVENTS_BLOCK))
				{
					g_ERStatus.state = e_BlockRotationState_CopyAlmEvents;
				}
				 
				break;

			case e_BlockRotationState_CopyAlmEvents:   // Block 1 kopieren
	
				if (copyEvents(ALM_EVENTS_BLOCK))
				{
					g_ERStatus.state = e_BlockRotationState_DoCmdCounters;
				}
				
				break;

			case e_BlockRotationState_DoCmdCounters:          // command Zähler nachführen

				if (updateCounters(CMD_EVENTS_BLOCK))
				{
					g_ERStatus.state = e_BlockRotationState_DoAlmCounters;
				}
				
				break;
				
			case e_BlockRotationState_DoAlmCounters: // Die Alarm-Zähler nachführen

				if (updateCounters(ALM_EVENTS_BLOCK))
				{
					g_ERStatus.state = e_BlockRotationState_CopyCounters;
				}
				
				break;

			case e_BlockRotationState_CopyCounters:
			    
				if (g_copyCounter < MAX_EVENT_COUNTERS)
				{                 
				    if ((!(((g_NewestEventStackMarker + 1) & (EVENTSTACK_SIZE-1)) == g_OldestEventStackMarker)) &&
				    	(!g_ERStatus.isWriting))
				 	{       
					    long event[BYTES_PER_EVENT]; 

				 	    event[0] = g_ROMCounters[g_copyCounter].date;
				 	    event[1] = g_ROMCounters[g_copyCounter].time;
				 	    event[2] = g_ROMCounters[g_copyCounter].counter;

				 		pushEvent(g_ERStatus.CurrentSectorAddress + COUNTER_OFFSET + g_copyCounter++ * sizeof(event),
							      event); // in den Puffer damit
				 	}	
				}       
				else // all counters copied
				{   
					g_ERStatus.writeEnable = FALSE; // Schreiben verhindern

					if(!g_ERStatus.isWriting)
					{
						g_copyCounter = 0; 
						
						// EB als ungültig markieren
						if(writeByteToEprom(EMPTY_SECTOR_MARK, 
							                (long*)g_ERStatus.OldSectorAddress, 
							                FALSE)) 
						{
							g_ERStatus.state = e_BlockRotationState_BlockRotationEnd;  
						}
					}
				}
				break;
			
 			case e_BlockRotationState_BlockRotationEnd:
 			      
 			 	 switch(writeByteToEpromStatus(EMPTY_SECTOR_MARK, 
					                           (long*)g_ERStatus.OldSectorAddress)) // fertig geschrieben ?
				 {
				 	case e_EpromWriteStatus_Ok:
					{  
					   	g_Mutex = FALSE;
						
	 					g_ERStatus.blockRotationIsOngoing = g_ongoingBlockRotationWarning = FALSE;

						g_ERStatus.state = e_BlockRotationState_Idle;   
						g_ERStatus.writeEnable = TRUE;       

						g_EmptySector = g_CurrentEventSector;
						g_CurrentEventSector = g_TempSectorForBR;       
						g_TempSectorForBR = 0;

				 	break;	
				 	}

					case e_EpromWriteStatus_Timeout: // Eprom timeout 
					
						epromTimeout();
				 		break;	
				 	
					// default e_EpromWriteStatus_AwaitWrite
				 }
 				 break;
			
			default: ASSERT(FALSE);
			
		} // end of switch(g_ERStatus.state)
    } // end of if (g_ERStatus.eventRecorderIsEnabled)                       
} 
 
/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* loadROMCounters															*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Subroutine for use in initeventrecorder.
 */  
static void SEGMENT_EXT_CODE loadROMCounters(long blockAddress, t_EventCounter *romCounter)
{   
	long i;
	
	for (i = 0; i < MAX_EVENT_COUNTERS; i++)
	{ 
		long data[BYTES_PER_EVENT]; 

		BOOL bSuccess = 
			readBytesFromEprom((long*)(blockAddress + COUNTER_OFFSET + i * sizeof (data)), 
							   data,
							   sizeof (data),
							   1);    
		
		ASSERT(bSuccess); // must not be busy during initialization

		if (data[0] == 0xFF)
		{
			romCounter[i].date    = STANDARD_DATE;  
			romCounter[i].time    = STANDARD_TIME;
			romCounter[i].counter = 0;
		}
		else // Zähler ins Ram kopieren
		{
			shrink12CharsTo3Longs(data); 
			
			romCounter[i].date =    data[0];
			romCounter[i].time =    data[1];
			romCounter[i].counter = data[2];
		}
	}
}     
 


/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* CountEvents															*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Subroutine for use in initeventrecorder.
 * StartOnly = 0 :[Don't count categories], 1 :[Count categorie start only], 2:[count all]
 */                                          
static void SEGMENT_EXT_CODE countEvents(   long            startAddr, 
											long           *total, 
											long           *singleCount, 
											t_EventCounter *romCounter, 
											long            startOnly, 
											long            max)
{ 
	long i = 0;
	long oldVal = 0;
	
	do
	{
	    BOOL bSuccess;

		long initData[BYTES_PER_EVENT];   

		oldVal = *total;

		bSuccess = readBytesFromEprom((long*)(startAddr + i * sizeof(initData)),
									   initData,
									   sizeof(initData),
									   1);

		ASSERT(bSuccess); // must not be busy during initialization

		if(initData[0] != 0xFF) 
		{
			(*total)++; // Gesamtanzahl zählen  
			
			if(startOnly > 0)
			{
				// Nach Kategorie zählen
				if(startOnly == 1)
				{ 
					if((initData[0] < MAX_EVENT_COUNTERS) && (initData[8] == START)) 
					{ 
						singleCount[(long)initData[0]]++; 
					} 
				}
				else
				{
					if(initData[0] < MAX_EVENT_COUNTERS) 
					{ 
						singleCount[(long)initData[0]]++; 
					}
				}
				
				if(initData[0] == ALARM_EVENT_RESET_COUNTER)
				{
					long event[BYTES_PER_EVENT];

					bSuccess = readBytesFromEprom((long*)(startAddr + i * sizeof(event)), 
												   event, 
												   sizeof(event),
												   1);

					ASSERT(bSuccess); // must not be busy during initialization

					shrink12CharsTo3Longs(event);
					
					if(event[2] < MAX_EVENT_COUNTERS)
					{
						romCounter[(long)event[2]].date    = event[0]; 
						romCounter[(long)event[2]].time    = event[1];
						romCounter[(long)event[2]].counter = 0;

						singleCount[(long)event[2]] = 0;
					}
				}
			}
		}
		i++;
	}
	while((*total != oldVal) && (i < max));
}


 
/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* InitEventRecorder 														*/
/* 																			*/
/*--------------------------------------------------------------------------*/
/* Description
 * ----------- 
 * Initialisiert den Eventrecorder.      
 *                                                                                       
 * Remarks
 * -------
 * EpromBusy on DSP B must be set to NOT_BUSY, DetermineBlocks has to run first 
 * on DSP B, before this routine is called
 * 
 * Change History
 * --------------
 * 10.03.1998 Clemens Bürgi  Erste Version
 */ 
 
void SEGMENT_EXT_CODE eventRec_init(void)  
{     
    BOOL bSuccess;

	long n_CmdEvents = 0;
	long n_AlmEvents = 0;
	 
	long counter[MAX_EVENT_COUNTERS]  = {0};  

	long k = 0; 
	long counterIx = 0;

    long sectorMark[2]; // there are two event sectors

    // Read block marks

	bSuccess = readByteFromEprom((long*)(EVENT_SECTOR1), &sectorMark[0]); 
	ASSERT(bSuccess); // must not be busy during initialization

	bSuccess = readByteFromEprom((long*)(EVENT_SECTOR2), &sectorMark[1]); 
	ASSERT(bSuccess); // must not be busy during initialization
 
	/*
	Situations:

	1. unmarked		 	 unmarked 
		-mark 1st sector as parameter sector

	2. unmarked or empty 	 event
		-empty sector = unmarked or empty sector, current = event sector

	3. event event, none with BR mark
		- erase both sectors, then goto 1
		
	4. event event, one with BR mark
	 crashed in BR

	BR:
	1. start mark on current sector
	2. erase new sector
	3. mark new sector with event sector mark
	4. make old sector as empty

	*/

	if (SECTOR_MARKS_ARE(UNMARKED_SECTOR_MARK, UNMARKED_SECTOR_MARK) || // virgin case
		SECTOR_MARKS_ARE(EMPTY_SECTOR_MARK,    UNMARKED_SECTOR_MARK))   // crashed in case x
	{
		initEventSectors();		
	}
	else if ((SECTOR_MARKS_ARE(EVENT_SECTOR_MARK, EMPTY_SECTOR_MARK))     || // normal case
		     (SECTOR_MARKS_ARE(EVENT_SECTOR_MARK, UNMARKED_SECTOR_MARK)))    // crash in BR state e_BlockRotationState_TestEndOfErase
	{
		g_TempSectorForBR = 0; // no ongoing block rotation

		// find the event block
		if (EVENT_SECTOR_MARK == sectorMark[0])
		{
			g_ERStatus.CurrentSectorAddress   = g_CurrentEventSector    = EVENT_SECTOR1;
			g_ERStatus.OldSectorAddress       = g_EmptySector           = EVENT_SECTOR2; 
		}
		else
		{
			g_ERStatus.CurrentSectorAddress   = g_CurrentEventSector    = EVENT_SECTOR2;
			g_ERStatus.OldSectorAddress       = g_EmptySector           = EVENT_SECTOR1; 
		}

		// Romzähler laden
		loadROMCounters(g_CurrentEventSector, g_ROMCounters);

		// Events zählen
		countEvents((long)(g_CurrentEventSector + g_ERStatus.BlockOffset[CMD_EVENTS_BLOCK]), 
			        &n_CmdEvents, 
					counter, 
					g_ROMCounters, 
					1, 
					MAX_CMD_EVENTS);

		countEvents((long)(g_CurrentEventSector + g_ERStatus.BlockOffset[ALM_EVENTS_BLOCK]), 
			        &n_AlmEvents, 
					counter, 
					g_ROMCounters, 
					2, 
					MAX_ALM_EVENTS);
		
		g_ERStatus.BlockNextPlace[CMD_EVENTS_BLOCK] = n_CmdEvents;
		g_ERStatus.BlockNextPlace[ALM_EVENTS_BLOCK] = n_AlmEvents;

		// Zähler setzen
		for(counterIx = 0; counterIx < MAX_EVENT_COUNTERS; counterIx++) 
		{ 
			g_Counters[counterIx] = counter[counterIx] + g_ROMCounters[counterIx].counter; 
		}

	}
	else if (SECTOR_MARKS_ARE(EVENT_SECTOR_MARK, EVENT_SECTOR_MARK)) 
		// two event blocks --> crashed during BR. Find out which one has a BR mark
	{
		long n_CmdEventsOverMinimum = 0;   
		long n_AlmEventsOverMinimum = 0;  

		long           counterOverMinimum   [MAX_EVENT_COUNTERS] =  {0};
		t_EventCounter romCounterOverMinimum[MAX_EVENT_COUNTERS] = {{0}};

		long brMarkOfSector1, brMarkOfSector2;               
		
		g_TempSectorForBR    = EVENT_SECTOR1;
		g_CurrentEventSector = EVENT_SECTOR2;

		// Falls zwei gültige Eventblöcke vorhanden sind, ist der Block mit der BW_START Marke der ursprüngliche

		bSuccess = readBytesFromEprom((long*)(g_TempSectorForBR    + BR_START_MARK_OFFSET),&brMarkOfSector1, 1, 1);
		ASSERT(bSuccess); // must not be busy during initialization

		bSuccess = readBytesFromEprom((long*)(g_CurrentEventSector + BR_START_MARK_OFFSET),&brMarkOfSector2, 1, 1);
		ASSERT(bSuccess); // must not be busy during initialization

		if(brMarkOfSector1 == BR_START_MARK)
		{
			g_ERStatus.OldSectorAddress       = g_TempSectorForBR; 
			g_ERStatus.CurrentSectorAddress   = g_CurrentEventSector;
		}
		else
		{
			if(brMarkOfSector2 == BR_START_MARK)
			{
				g_ERStatus.OldSectorAddress       = g_CurrentEventSector; 
				g_ERStatus.CurrentSectorAddress   = g_TempSectorForBR;
			}
			else // Keine BW_START_MARKE gefunden -> 
			{
				initEventSectors();
			}
		}

		loadROMCounters(g_ERStatus.OldSectorAddress, g_ROMCounters); // ROMZähler laden

		countEvents((long)(g_ERStatus.OldSectorAddress + g_ERStatus.BlockOffset[CMD_EVENTS_BLOCK]), &n_CmdEvents, counter, g_ROMCounters, 1, MAX_CMD_EVENTS);
		countEvents((long)(g_ERStatus.OldSectorAddress + g_ERStatus.BlockOffset[ALM_EVENTS_BLOCK]), &n_AlmEvents, counter, g_ROMCounters, 2, MAX_ALM_EVENTS);

		k = (n_CmdEvents < MINIMUM_EVENTS)? n_CmdEvents:MINIMUM_EVENTS;

		countEvents((long)(g_ERStatus.CurrentSectorAddress + g_ERStatus.BlockOffset[CMD_EVENTS_BLOCK] + k * BYTES_PER_EVENT), 
			        &n_CmdEventsOverMinimum, 
					counterOverMinimum, 
					romCounterOverMinimum, 
					1, 
					MAX_CMD_EVENTS);
		
		k = (n_AlmEvents < MINIMUM_EVENTS)? n_AlmEvents:MINIMUM_EVENTS;

		countEvents((long)(g_ERStatus.CurrentSectorAddress + g_ERStatus.BlockOffset[ALM_EVENTS_BLOCK] + k * BYTES_PER_EVENT), 
			        &n_AlmEventsOverMinimum, 
					counterOverMinimum, 
					romCounterOverMinimum, 
					2, 
					MAX_ALM_EVENTS);	  

		g_ERStatus.numberOfLostEvents += (n_CmdEventsOverMinimum + n_AlmEventsOverMinimum); // Events Lost setzen

		// Zähler setzen
		for(counterIx = 0; counterIx < MAX_EVENT_COUNTERS; counterIx++) 
		{ 
			g_Counters[counterIx] = counter[counterIx] + counterOverMinimum[counterIx] + g_ROMCounters[counterIx].counter; 
		}

		// counterOverMinimum g_Counters zu den ROMZählern addieren, da sie sonst verloren gehen
		for(counterIx = 0; counterIx < MAX_EVENT_COUNTERS; counterIx++) 
		{ 
			g_ROMCounters[counterIx].counter = g_ROMCounters[counterIx].counter + counterOverMinimum[counterIx]; 
		}

		// Block ungültig markieren
		while (!
			writeByteToEprom(EMPTY_SECTOR_MARK, 
							 (long*)(g_ERStatus.OldSectorAddress), 
							 TRUE)); // blocking write 

		g_CurrentEventSector = g_ERStatus.OldSectorAddress;
		g_EmptySector = 0;
		g_TempSectorForBR = g_ERStatus.CurrentSectorAddress; 
		
		g_ERStatus.blockRotationIsOngoing = TRUE;
		g_ERStatus.state = e_BlockRotationState_WriteStartMark;
		g_ERStatus.OldBlockNextPlace[CMD_EVENTS_BLOCK]	= n_CmdEvents;
		g_ERStatus.OldBlockNextPlace[ALM_EVENTS_BLOCK]	= n_AlmEvents;

		if(n_CmdEvents < MINIMUM_EVENTS)
		{	g_ERStatus.BlockNextPlace[CMD_EVENTS_BLOCK] = n_CmdEvents;
			g_ERStatus.EventsToCopy[CMD_EVENTS_BLOCK]   = n_CmdEvents;
			g_ERStatus.ReadStartAddress[CMD_EVENTS_BLOCK] = g_ERStatus.OldSectorAddress + g_ERStatus.BlockOffset[CMD_EVENTS_BLOCK];    	
		}
		else 
		{	g_ERStatus.BlockNextPlace[CMD_EVENTS_BLOCK] = MINIMUM_EVENTS; 
			g_ERStatus.EventsToCopy[CMD_EVENTS_BLOCK]   = MINIMUM_EVENTS;
			g_ERStatus.ReadStartAddress[CMD_EVENTS_BLOCK] = g_ERStatus.OldSectorAddress + g_ERStatus.BlockOffset[CMD_EVENTS_BLOCK] + BYTES_PER_EVENT*(n_CmdEvents - MINIMUM_EVENTS);
		}

		if(n_AlmEvents < MINIMUM_EVENTS) 
		{	g_ERStatus.BlockNextPlace[ALM_EVENTS_BLOCK] = n_AlmEvents; 
			g_ERStatus.EventsToCopy[ALM_EVENTS_BLOCK]   = n_AlmEvents;
			g_ERStatus.ReadStartAddress[ALM_EVENTS_BLOCK] = g_ERStatus.OldSectorAddress + g_ERStatus.BlockOffset[ALM_EVENTS_BLOCK];    	
		}
		else 
		{	g_ERStatus.BlockNextPlace[ALM_EVENTS_BLOCK] = MINIMUM_EVENTS;  
			g_ERStatus.EventsToCopy[ALM_EVENTS_BLOCK]   = MINIMUM_EVENTS;
			g_ERStatus.ReadStartAddress[ALM_EVENTS_BLOCK] = g_ERStatus.OldSectorAddress + g_ERStatus.BlockOffset[ALM_EVENTS_BLOCK] + BYTES_PER_EVENT*(n_AlmEvents - MINIMUM_EVENTS);
		}

		g_ERStatus.writeEnable = FALSE;
	}
	else
	{
		initEventSectors();
	}
	
	g_pInternalTime_Decimal = rtc_getIntTimePtr();
	g_pInternalTime_BCD     = rtc_getIntTimeBCDPtr();
	
	// Startevent erzeugen
	eventRec_recordEvent(ALARM_EVENT_MACHINE_START,0,FALSE);    
} 

static void SEGMENT_EXT_CODE initEventSectors(void)
{
	BOOL bSuccess;

	g_ERStatus.CurrentSectorAddress   = g_CurrentEventSector    = EVENT_SECTOR1;
	g_ERStatus.OldSectorAddress       = g_EmptySector           = EVENT_SECTOR2; 
	g_TempSectorForBR = 0; 

	// erase the 2 block in case of crash in x
	bSuccess = eraseBlock(EVENT_SECTOR1);
	ASSERT(bSuccess);

	while(eraseBlockStatus(EVENT_SECTOR1) == e_EraseBlockStatus_AwaitErase);

	bSuccess = eraseBlock(EVENT_SECTOR2);// Blocklöschen
	ASSERT(bSuccess);

	while(eraseBlockStatus(EVENT_SECTOR2) == e_EraseBlockStatus_AwaitErase);

	// Blockmarke schreiben
	bSuccess =
		writeByteToEprom(EMPTY_SECTOR_MARK, 
						 (long*)(g_ERStatus.OldSectorAddress), 
						 TRUE); // blocking write operation                              
	ASSERT(bSuccess);

	// what if we crash here? --> case x

	// Blockmarke schreiben
	bSuccess =
		writeByteToEprom(EVENT_SECTOR_MARK, 
						 (long*)(g_ERStatus.CurrentSectorAddress), 
						 TRUE); // blocking write operation   
	ASSERT(bSuccess);
}                           
  
static SEGMENT_EXT_CODE BOOL copyEvents(unsigned long eventBlockType)
{
	 if (g_copyCounter < g_ERStatus.EventsToCopy[eventBlockType])
	 {
		if ((!(((g_NewestEventStackMarker + 1) & (EVENTSTACK_SIZE-1)) == g_OldestEventStackMarker)) 
			 &&	(!g_ERStatus.isWriting)) // Eventpuffer voll oder Eprom beschäftigt ?
		{   
			long event[BYTES_PER_EVENT]; 

			BOOL bSuccess =	readBytesFromEprom((long*) (g_ERStatus.ReadStartAddress[eventBlockType] + 
														g_copyCounter * sizeof(event)), 
											   event, 
											   sizeof(event), 
											   1); 
			if (!bSuccess)
				return FALSE; // try again

			// Bei Zeitschlitzproblemen, hier in weitere cases zerlegen				   
			shrink12CharsTo3Longs(event); // longs zusammensetzen

			pushEvent(g_ERStatus.CurrentSectorAddress + 
					  g_ERStatus.BlockOffset[eventBlockType] + 
					  g_copyCounter++ * sizeof(event), 
					  event); // in den Puffer damit
		}
		return FALSE;
	 }
	 else
	 {
		g_copyCounter = 0;
		return TRUE;
	 }
}

static SEGMENT_EXT_CODE BOOL updateCounters(unsigned long eventBlockType)
{
	if (!g_ERStatus.isWriting)
	{
		if (g_copyCounter < (g_ERStatus.OldBlockNextPlace[eventBlockType] - MINIMUM_EVENTS))
		{
			long event[BYTES_PER_EVENT]; 

			BOOL bSuccess = 
				readBytesFromEprom((long*)(g_ERStatus.OldSectorAddress + 
										   g_ERStatus.BlockOffset[eventBlockType] + 
										   g_copyCounter * sizeof(event)),
									event, 
									2, // read category and data
									8);   

			if (bSuccess) // else try again
			{
				if ((event[0] < MAX_EVENT_COUNTERS) && (event[1] == START)) 
				{ 
					g_ROMCounters[event[0]].counter++; 
				}
				
				if (event[0] == ALARM_EVENT_RESET_COUNTER)     // Zähler reset 
				{                                              // Zählernummer lesen
					long data[BYTES_PER_EVENT]; 

					bSuccess = 
	    				readBytesFromEprom((long*)(g_ERStatus.OldSectorAddress + 
												   g_ERStatus.BlockOffset[eventBlockType] + 
												   g_copyCounter * sizeof(data)),
										   data,
										   sizeof(data),
										   1); 

					ASSERT (bSuccess); // must not be busy now
					
	    			shrink12CharsTo3Longs(data);

	    			g_ROMCounters[data[8]].date = data[0];  // das 8. Byte des Events
	    			g_ROMCounters[data[8]].time = data[1];  // gibt die Zählernummer an
	    			g_ROMCounters[data[8]].counter = 0;
				}

				g_copyCounter++;  
			}
		}   
		else
		{	                                
			g_copyCounter = 0;
			return TRUE;  
		}
	}
	return FALSE;
}


static SEGMENT_EXT_CODE void epromTimeout(void)
{
	g_ERStatus.eventRecorderIsEnabled = g_ongoingBlockRotationWarning = FALSE; // disable EventRecorder

	resetEprom(); // Reset eprom 

	g_Mutex = FALSE;   
}

void SEGMENT_EXT_CODE eventRec_enable(BOOL bEnable)
{
	static BOOL enabled;

	if (bEnable)
	{
		if (enabled) // do not enable event recorder if it has been disabled internally because of internal error
		{
			g_ERStatus.eventRecorderIsEnabled = TRUE;
		}
	}
	else // disable
	{
		// save current state
		enabled = g_ERStatus.eventRecorderIsEnabled;

		g_ERStatus.eventRecorderIsEnabled = FALSE;
	}
}

BOOL SEGMENT_EXT_CODE eventRec_isEnabled(void)
{
	return g_ERStatus.eventRecorderIsEnabled;
}


void SEGMENT_EXT_CODE eventRec_clearAndReset(void)
{
	initEventSectors();
	resetP4LT();
}

BOOL SEGMENT_EXT_CODE eventRec_blockRotationIsOngoing(void)
{
	return g_ongoingBlockRotationWarning;
}

void SEGMENT_EXT_CODE eventRec_EmptyStack(void)
{
	int i;

	for (i = 0; i<5000 ; i++)  // should be enough time to terminate all pending tasks
	{
		IO_BUFFER_READ = *(long*)IO_BUFFER_DSP; //serve watchdog

		eventRec_activate(); 
		eventRec_activateBlockRotationHandler();
		eventRec_writeEventsToEprom();         
	}
}

