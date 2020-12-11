/*-------------------------------------------------------------------------*
 * $Workfile: MemoryCheck.h $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/H/MemoryCheck.h $
 * 
 * 4     28.06.05 18:21 Chmibuh
 * replace bool by BOOL
 * 
 * 3     19.05.05 10:53 Chmibuh
 * memoryCheckRespondToMaster() added
 * 
 * 2     11.05.05 16:45 Chmibuh
 * 
 * 1     9.05.05 11:43 Chbrbus
 * 
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef CRC_PROGRAM_MEMORY_CHECK
#define CRC_PROGRAM_MEMORY_CHECK

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

void SEGMENT_EXT_CODE DoMemoryCheck(BOOL DoAlsoExternalSDRAM);
void SEGMENT_EXT_CODE MemoryCheckStartupSHARC(BOOL DoAlsoExternalSDRAM);
BOOL SEGMENT_EXT_CODE memoryCheckDSP_CRC_Alarm(void);
void SEGMENT_EXT_CODE memoryCheckRespondToMaster(void);

#endif

