/*-------------------------------------------------------------------------*
 * $Workfile: EPROMHI.C $
 * Part of	: ETL600
 * Language	: C
 * Created by	: CLEMENS BUERGI    19.01.1998
 * Remarks	:     
 * Purpose	:     Eprom High level routines
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Master_DSP/C/EPROMHI.C $
 * 
 * 12    25.07.05 16:58 Chchlee
 * g_firmware_startAddress now static
 * 
 * 11    16.07.05 18:44 Chmibuh
 * exchange FIRMWARE_START_ADDRESS (define) against
 * g_firmware_startAddress (variable) to allow dynamic setting of start
 * address (Emergency FW vs. normal application)
 * 
 * 10    30.06.05 8:18 Chchlee
 * 
 * 9     13.06.05 16:08 Chchlee
 * 
 * 8     13.06.05 7:33 Chchlee
 * additional manipulation events added
 * 
 * 7     25.05.05 17:12 Chchlee
 * Configuration check added
 * 
 * 6     17.05.05 18:10 Chmibuh
 * Compiler Switch P4LW_PTTY2 added - this allows differentiation from
 * 0-Serie
 * 
 * 5     24.04.05 17:02 Chmibuh
 * #define P4LT_PROTOTYPE2HW added for Emerg. Test _ do not erase EEPROM
 * Block SA0-SA7
 * 
 * 4     11.04.05 18:32 Chchlee
 * Concurrent flash access bug fixed
 * 
 * 3     16.02.05 10:25 Chchlee
 * Upgrade to Ptty2
 * 
 * 2     26.01.05 13:22 Chchlee
 * bug with multiple download of temporary configuration fixed
 * 
 * 1     19.01.05 15:48 Chchlee
 * 
 * 20    13.01.05 13:11 Chchlee
 * Sectors for diagnose recorder added
 * 
 * 19    11.12.04 18:58 Chmibuh
 * diferrent FIRMWARE_START_ADDRESS and No. of erease Blocks for
 * EMERGENCY_FW and normal appl.
 * 	#define  
 * 
 * 18    27.09.04 7:53 Leeb02
 * 
 * 17    18.08.04 17:22 Leeb02
 * Bug fixed: no write to flash if config sub structure has zero length
 * 
 * 16    16.08.04 9:57 Leeb02
 * 
 * 15    16.08.04 7:37 Leeb02
 * 
 * 14    4.08.04 16:15 Leeb02
 * 
 * 13    4.08.04 11:26 Leeb02
 * Block management improved
 * 
 * 12    3.08.04 14:46 Leeb02
 * first 8 K Sector Problem - ereased "by hand" 
 * 
 * 11    3.08.04 11:09 Leeb02
 * 
 * 10    2.08.04 9:09 Leeb02
 * 
 * 9     2.08.04 8:17 Leeb02
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/
#include "basetype.h"
#include "diag.h" 

#include "epromhi.h" 
#include "eprommed.h" 
#include "epromlow.h" 
#include "crc.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// configuration sector constants
#define PERMANENT_CFG_SECTOR_MARK		0x30   // permanent configuration
#define TEMP_INITIAL_CFG_SECTOR_MARK    0xF0   // temporary configuration before reset
#define TEMP_CFG_SECTOR_MARK		    0x70   // temporary configuration (after first reset)
#define UNMARKED_CFG_SECTOR_MARK	    0xFF   
#define EMPTY_CFG_SECTOR_MARK	     	0x10   

// the 2 configuration sectors
#define CFG_SECTOR1        SECTOR_SA67
#define CFG_SECTOR2        SECTOR_SA68

// offsets: sector mark has offset 0
#define DATA_OFFSET         0x0008
#define CRC_OFFSET          0xFFFC  // CRC wird immer an diese Addresse im Parametersector gespeichert


#define FIRMWARE_END_ADDRESS      SECTOR_SA64

/* LOCAL TYPE DECLARATIONS ***************************************************/
 

/* LOCAL MACRO DEFINITIONS ***************************************************/

// CAUTION: "sectorMark" must be static because pointer to this variable is put on the stack and processed later!!
#define MARK_SECTOR(sector, mark)\
	{\
		static long sectorMark = (mark) | MASK_24MSB;\
		eprom_pushCommand(SAVE_DATA,&sectorMark,(long*)(sector), sizeof(sectorMark));\
	}

#define CFG_SECTOR_MARKS_ARE(mark_a, mark_b)\
	   ((cfgSectorMarks[0] == (mark_a))   && (cfgSectorMarks[1] == (mark_b))       ||\
	    (cfgSectorMarks[0] == (mark_b))   && (cfgSectorMarks[1] == (mark_a))          )

#define GET_CFG_SECTOR(mark) ((cfgSectorMarks[0] == (mark)) ? CFG_SECTOR1 : CFG_SECTOR2)
#define OTHER_CFG_SECTOR(sector) (((sector) == CFG_SECTOR1) ? CFG_SECTOR2 : CFG_SECTOR1)


/* LOCAL VARIABLES ***********************************************************/

static                  long                  g_currentParameterSector;
static SEGMENT_EXT_DATA t_ConfigStructsList * g_pCfgList;

static                  BOOL                  g_IsTemporaryConfiguration;
static                  BOOL                  g_IsTemporaryConfiguration;

#ifdef P4LW_PTTY2
	static unsigned long g_firmware_startAddress = SECTOR_SA0;
#else
	static unsigned long g_firmware_startAddress = SECTOR_SA8;
#endif


/* LOCAL FUNCTIONS ***********************************************************/

static void saveConfigStructure      (long sector);
static BOOL loadParamSector           (long sector);
static void saveDefaultConfiguration (long sector, void (*getDefaultCfg)(void));
static long CRC16Long(unsigned long reg, unsigned long value);

/* IMPLEMENTATION ************************************************************/


// CAUTION: this function must not be called at runtime, because it's using eprom_emptyStack
void SEGMENT_EXT_CODE eprom_init(t_ConfigStructsList * p_CfgList, 
								 BOOL * isTemporaryConfiguration,
								 void (*getDefaultCfg)(void))
{
    BOOL bSuccess;
	long cfgSectorMarks[2]; // there are two configuration sectors

	// make sure that the configuration structure is not 
	// bigger than the available space in flash
	ASSERT(NR_OF_BYTES_IN_LONG * sizeof(t_ConfigStructsList) <= (CRC_OFFSET - DATA_OFFSET));

	g_pCfgList = p_CfgList; // remember pointer to t_ConfigStructsList

	*isTemporaryConfiguration = FALSE;

	resetEprom(); 

    // Read sector marks
	bSuccess = readByteFromEprom((long*)(CFG_SECTOR1), &cfgSectorMarks[0]); 
	ASSERT(bSuccess); // must not be busy during initialization

	bSuccess = readByteFromEprom((long*)(CFG_SECTOR2), &cfgSectorMarks[1]); 
	ASSERT(bSuccess); // must not be busy during initialization

	// now check which situation we are in
	if ((cfgSectorMarks[0] == TEMP_INITIAL_CFG_SECTOR_MARK) || 
		(cfgSectorMarks[1] == TEMP_INITIAL_CFG_SECTOR_MARK))
	{
		// found a temporary configuration. Mark it with TEMP_CFG_SECTOR_MARK:
		// so, at next start-up, the permanent configuration is used

		g_currentParameterSector = GET_CFG_SECTOR(TEMP_INITIAL_CFG_SECTOR_MARK);

		// set mark to TEMP_CFG_SECTOR_MARK
		MARK_SECTOR(g_currentParameterSector, TEMP_CFG_SECTOR_MARK);

		if (!loadParamSector(g_currentParameterSector))
		{
			// configuration was faulty -> store default configuration
			saveDefaultConfiguration(g_currentParameterSector, getDefaultCfg);
		}
		else
		{
			*isTemporaryConfiguration = g_IsTemporaryConfiguration = TRUE;
		}

		return;
	}

	if (  
	      // the time-out case: the board has reset before the temporary configuration was saved
		  CFG_SECTOR_MARKS_ARE(PERMANENT_CFG_SECTOR_MARK, TEMP_CFG_SECTOR_MARK)    || 
		  // the normal case
          CFG_SECTOR_MARKS_ARE(PERMANENT_CFG_SECTOR_MARK, EMPTY_CFG_SECTOR_MARK)   ||
           // the virgin case
          CFG_SECTOR_MARKS_ARE(PERMANENT_CFG_SECTOR_MARK, UNMARKED_CFG_SECTOR_MARK)       )

	{
		// use the permanent configuration
		g_currentParameterSector = GET_CFG_SECTOR(PERMANENT_CFG_SECTOR_MARK);

		if (!loadParamSector(g_currentParameterSector))
		{
			// configuration was faulty -> store default configuration
			saveDefaultConfiguration(g_currentParameterSector, getDefaultCfg);
		}

		return;
	}

	if (CFG_SECTOR_MARKS_ARE(EMPTY_CFG_SECTOR_MARK, TEMP_CFG_SECTOR_MARK))
	{
		// crash case 1: use temp sector
		g_currentParameterSector = GET_CFG_SECTOR(TEMP_CFG_SECTOR_MARK);
	    MARK_SECTOR(g_currentParameterSector, PERMANENT_CFG_SECTOR_MARK); // and make it permanent

		if (!loadParamSector(g_currentParameterSector))
		{
			// configuration was faulty -> store default configuration
			saveDefaultConfiguration(g_currentParameterSector, getDefaultCfg);
		}

		return;
	}

	// in all other cases: save default configuration
	eprom_saveDefaultConfiguration(getDefaultCfg);
}

// CAUTION: this function must not be called at runtime, because it calls eprom_emptyStack()
void SEGMENT_EXT_CODE eprom_saveDefaultConfiguration(void (*getDefaultCfg)(void))
{
	g_currentParameterSector = CFG_SECTOR1;
	
	saveDefaultConfiguration(g_currentParameterSector, getDefaultCfg); 
}



void SEGMENT_EXT_CODE eprom_storeTemporaryConfiguration(void)
{                              
	long sectorWithTemporaryConfiguration;
//	static BOOL bFlag = FALSE;
	
	// ignore a second invocation of this function. This could happen if the HMI makes a retry.
//	if (bFlag) return; 
	
//	bFlag = TRUE;

	// if current configuration is temporary: overwrite currrent (temporary) parameter sector.

	//                                   currentParameterSector         other parameter sector
	//   Eventually we go from situation TEMP_CFG_SECTOR_MARK         / PERMANENT_CFG_SECTOR_MARK to 
	//                                   TEMP_INITIAL_CFG_SECTOR_MARK / PERMANENT_CFG_SECTOR_MARK

	// if current configuration is permanent: write to other sector

	//                                   currentParameterSector         other parameter sector
	//   Eventually we go from situation PERMANENT_CFG_SECTOR_MARK    / any 
	//                                   PERMANENT_CFG_SECTOR_MARK    / TEMP_INITIAL_CFG_SECTOR_MARK

	sectorWithTemporaryConfiguration = g_IsTemporaryConfiguration ?                  g_currentParameterSector:
	                                                                OTHER_CFG_SECTOR(g_currentParameterSector);

	eprom_pushCommand(ERASE_BLOCK,0,(long*)sectorWithTemporaryConfiguration, 0); // Sector löschen  

	saveConfigStructure(sectorWithTemporaryConfiguration);

	MARK_SECTOR(sectorWithTemporaryConfiguration, TEMP_INITIAL_CFG_SECTOR_MARK);

	// now, the P4LT will reset, find the temporary configuration and set
	// its sector mark to TEMP_CFG_SECTOR_MARK
}                    


void SEGMENT_EXT_CODE eprom_markTemporaryConfigurationAsPermanent(void)
{
	// no negative effect if this function is called more than once due to HMI retries
	MARK_SECTOR(OTHER_CFG_SECTOR(g_currentParameterSector), EMPTY_CFG_SECTOR_MARK); // make previous permanent sector as empty
	
	// What happens if we crash here? We have an empty and a temp sector. So we use the temp sector.
	// See case 1 in eprom_init.

	MARK_SECTOR(g_currentParameterSector, PERMANENT_CFG_SECTOR_MARK);

	g_IsTemporaryConfiguration = FALSE;
}

void SEGMENT_EXT_CODE eprom_storePermanentConfiguration(void)
{
	long sectorWithPermanentConfiguration;
//	static BOOL bFlag = FALSE;
	
	// ignore a second invocation of this function. This could happen if the HMI makes a retry.
//	if (bFlag) return; 
	
//	bFlag = TRUE;

	// if current configuration is temporary: overwrite other (permanent) parameter sector.
	//                                   currentParameterSector            other parameter sector
	//   Eventually we go from situation TEMP_CFG_SECTOR_MARK            / PERMANENT_CFG_SECTOR_MARK 
	// 									 EMPTY_CFG_SECTOR_MARK           / PERMANENT_CFG_SECTOR_MARK

	// if current configuration is permanent: write current sector
	//                                   currentParameterSector           other parameter sector
	//   Eventually we go from situation PERMANENT_CFG_SECTOR_MARK      / any 
	//                                   PERMANENT_CFG_SECTOR_MARK      / EMPTY_CFG_SECTOR_MARK


	sectorWithPermanentConfiguration = g_IsTemporaryConfiguration ? OTHER_CFG_SECTOR(g_currentParameterSector):
	                                                                                 g_currentParameterSector;

	eprom_pushCommand(ERASE_BLOCK,0,(long*)sectorWithPermanentConfiguration, 0); // erase sector

	saveConfigStructure(sectorWithPermanentConfiguration);

	MARK_SECTOR(sectorWithPermanentConfiguration, PERMANENT_CFG_SECTOR_MARK);
	// if we crash at this very location, we have an invalid sector situation which will lead to the default configuration
	MARK_SECTOR(OTHER_CFG_SECTOR(sectorWithPermanentConfiguration), EMPTY_CFG_SECTOR_MARK);
	
	g_IsTemporaryConfiguration = FALSE;

	// now, the P4LT will reset, find the permanent configuration

}

static void SEGMENT_EXT_CODE saveConfigStructure(long sector)
{
	int x;
	long ptr = DATA_OFFSET; // start at data offset     

	// Alle in ConfigStructsList eingetragenen Structs speichern
	for(x = 0; x < NUMBER_OF_CONFIG_STRUCTS; x++)
	{
		if ((*g_pCfgList)[x].sizeInLongs > 0)
		{
			eprom_pushCommand(SAVE_DATA,
							(long*)(*g_pCfgList)[x].address,  // from ram address
							(long*)(sector + ptr),			  // to flash address
							(*g_pCfgList)[x].sizeInLongs);    // number of longs

			ptr += (*g_pCfgList)[x].sizeInLongs * NR_OF_BYTES_IN_LONG;
		}

	}

	eprom_pushCommand(CALC_AND_WRITE_CRC, 
			        (long*)(sector + DATA_OFFSET), // from 
					(long*)(sector + CRC_OFFSET),  // to 
			        ptr - DATA_OFFSET);           // that many bytes
}


static long CRC16Long(unsigned long reg, unsigned long value) 
{    
	reg = crc16(reg,  (value & 0xFF));
	reg = crc16(reg, ((value & 0xFF00)     >> 8));
	reg = crc16(reg, ((value & 0xFF0000)   >> 16));
	reg = crc16(reg, ((value & 0xFF000000) >> 24));

	return(reg);
}                    
                     
 
// CAUTION: this function must not be called at runtime, because it calls eprom_emptyStack()
static void SEGMENT_EXT_CODE saveDefaultConfiguration(long sector,
													  void (*getDefaultCfg)(void))
{   
	eprom_pushCommand(ERASE_BLOCK,0,(long*)sector,0);   // Sector löschen 

	getDefaultCfg();
	saveConfigStructure(sector);

	MARK_SECTOR(g_currentParameterSector, PERMANENT_CFG_SECTOR_MARK);

	// erase other parameter sector
	eprom_pushCommand(ERASE_BLOCK,0,(long*)OTHER_CFG_SECTOR(sector), 0);

	eprom_emptyStack(); // process all stack commands
} 

 
static BOOL SEGMENT_EXT_CODE loadParamSector(long sector)
{                     
	long offset = DATA_OFFSET;

 	long CRCinROM;  

    long calcCRC = VOIDCRC; // CRC start value
   
	int x;

	BOOL bSuccess = readLongFromEprom((long*)(sector + CRC_OFFSET), &CRCinROM);  
	ASSERT (bSuccess); // flash must not be busy during initialization

	for(x = 0; x < NUMBER_OF_CONFIG_STRUCTS; x++)
	{
		int ixLong;
	    unsigned long *ramAddress = (unsigned long *)(*g_pCfgList)[x].address;

		for (ixLong = 0; ixLong < (*g_pCfgList)[x].sizeInLongs; ixLong++)
		{
			bSuccess = readLongFromEprom((long*)(sector + offset + NR_OF_BYTES_IN_LONG * ixLong), (long*)ramAddress);
			ASSERT (bSuccess); // flash must not be busy during initialization

			calcCRC = CRC16Long(calcCRC, *ramAddress++);  // calculate CRC
		}
		
		offset += (*g_pCfgList)[x].sizeInLongs * NR_OF_BYTES_IN_LONG;
	}	

	// Check CRC   
	return (calcCRC == CRCinROM);
}             
   
   

/*********************** LOCAL FUNCTION DEFINITIONS *************************/
/* programData()											 				*/
/* Writes a program to the Flash Eprom										*/
/*--------------------------------------------------------------------------*/
/* Description
 * -----------                                                                
 * Writes a firmware sector to the Flash Eprom. During a firmware update, this
 * routine is called a number of times with 20H bytes to write. If an error is
 * returned, this will be indicated on MMI.
 *
 * Input
 * -----
 * messagePtr	pointer to programData message, built up like
 *				this (a word is 32 bits, a byte is 8 bits):
 *
 *				word(0015)	word(0017)	contents
 *				----------	----------	--------
 *				1			-			message byte (not relevant here) 
 *				2       	1			number of bytes in firmware sector
 *				3       	2			start address in Eprom where FW sector 
 *										is to be written
 *				4			3			first 4 bytes of Eprom code
 *				5			4			following 4 bytes, and so on.	
 *
 * Output
 * ------
 * Return Value is FALSE if an error occurred, else TRUE.
 *                   
 */
 
BOOL SEGMENT_EXT_CODE programData(long *messagePtr)
{
    long bytePositionInLong = 0;
	long nr_of_bytes  = *messagePtr++;
	long base_address = *messagePtr++;
    long * addressInEprom = (long*)(g_firmware_startAddress + (unsigned long)base_address);  

    

// Range plausibility check : Make sure that no WRITE operation can accidentally
// overwrite the parameter sectors. Unfortunately, by this time, the Eprom will 
// already have been erased, making damage inevitable. But like this, FW update
// will at least terminate immediately if address is not plausible.

	if ((long)addressInEprom + nr_of_bytes > FIRMWARE_END_ADDRESS + SECTOR_SIZE_64K - 1)
	{
	   	return FALSE;			    	
	}   
    
    do
    {   
		long theByte = ((*messagePtr) >> (bytePositionInLong * NR_OF_BITS_IN_BYTE)) & MASK_8LSB;   

        if (++bytePositionInLong >= NR_OF_BYTES_IN_LONG)
        {
   	 		bytePositionInLong = 0;
   		    messagePtr++;
		}

		if (!writeByteToEprom(theByte, addressInEprom++, TRUE )) // is blocking write
		{
			return FALSE; // eprom was busy --> try again
		}
    } 
	while((long)addressInEprom < (g_firmware_startAddress + base_address + nr_of_bytes));
  									
    return TRUE;
}

void SEGMENT_EXT_CODE eprom_eraseFirmware(void)
{
	long sector = SECTOR_SA8; // first 64k sector

	if(g_firmware_startAddress == SECTOR_SA0)
	{
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA0,0);   
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA1,0);   
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA2,0);   
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA3,0);   
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA4,0);   
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA5,0);   
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA6,0);   
		eprom_pushCommand(ERASE_BLOCK,0,(long*)SECTOR_SA7,0);   
	}

	// delete all 64k sectors
	while (sector <= FIRMWARE_END_ADDRESS)
	{
		eprom_pushCommand(ERASE_BLOCK,0,(long*)sector,0);   
		sector += SECTOR_SIZE_64K;
	}
}

 
unsigned long SEGMENT_EXT_CODE * eprom_get_firmware_startAddress_Ptr(void)
{
	return &g_firmware_startAddress;
}
