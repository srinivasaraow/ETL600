/*-------------------------------------------------------------------------*
 * $Workfile	: MemoryCheck.h $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Program and constant data memory check 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/DSP/H/memoryCheck.h $
 * 
 * 1     07-08-30 10:48 Chalnot
 * Initial version.
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

extern void memoryCheck_initializeMemoryCheck(void);
extern void memoryCheck_startupMemoryCheck(void);
extern void memoryCheck_doMemoryCheck(void);
extern bool memoryCheck_getCrcAlarm(void);
extern void memoryCheck_resetBootTries(void);

#endif


