/*-------------------------------------------------------------------------*
 * $Workfile: MemoryCheck.c $
 * Part of	: ETL600
 * Language	: C
 * Created by: 
 * Remarks	:  
 * Purpose	: Program and data memory check during startup/in operation
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/FE_DSP/C/MemoryCheck.c $
 * 
 * 13    10.10.05 16:26 Chchlee
 * 
 * 12    28.06.05 18:21 Chmibuh
 * replace bool by BOOL
 * 
 * 11    24.06.05 11:20 Chmibuh
 * the previously used fct. calc_checksum48() is devided in bytewise check
 * to handle limited computing time on Upconverter DSP
 * 
 * 10    23.06.05 12:04 Chmibuh
 * now check complete memory segments one by one
 * 
 * 9     2.06.05 11:32 Chmibuh
 * if a crc error disappears the alarm bit is now not cleared. Once a crc
 * error occure it is hold off forever
 * 
 * 8     19.05.05 10:53 Chmibuh
 * memoryCheckRespondToMaster() added
 * 
 * 7     18.05.05 16:06 Chmibuh
 * CRC_PM_RTH_ERROR enabled
 * 
 * 6     18.05.05 13:24 Chmibuh
 * CRC_PM_RTH_ERROR & CRC_PM_INIT_ERROR disabled
 * 
 * 5     18.05.05 10:13 Chmibuh
 * 
 * 4     12.05.05 14:04 Chmibuh
 * 
 * 3     11.05.05 18:54 Chmibuh
 * 
 * 2     11.05.05 16:45 Chmibuh
 * 
 * 1     9.05.05 11:43 Chbrbus
 * 
 * 
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/
#include "basetype.h"
#include "crc.h"
#include <def21161.h>
asm("#include <def21161.h>");

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

// Move to common file
#define DATA_MEM_INTERN_ERROR		0x01000000
#define DATA_MEM_SDRAM_ERROR		0x02000000
#define DATA_MEM_INTERN_PM_ERROR	0x04000000
#define CRC_PM_RTH_ERROR			0x08000000
#define CRC_PM_INIT_ERROR			0x10000000
#define CRC_PM_ERROR				0x20000000
#define CRC_PM_DM_ERROR				0x40000000
#define CRC_PM_SDRAM_ERROR			0x80000000


#define CHECK_CASE_PM_RTH				0
#define CHECK_CASE_PM_INIT				1
#define CHECK_CASE_PM_INTERN			2
#define CHECK_CASE_PM_EXTERN			3
#define CHECK_CASE_DM_INTERN_PM			4
#define CHECK_CASE_DM_INTERN			5
#define CHECK_CASE_DM_EXTERN			6
#define CHECK_CASE_MAX					CHECK_CASE_DM_EXTERN	//always the last one!

// subcases for new 48-bit instruction test
#define CASE_GET_PM_INSTR	0
#define CASE_CHECK_1_BYTE	1
#define CASE_CHECK_2_BYTE	2
#define CASE_CHECK_3_BYTE	3
#define CASE_CHECK_4_BYTE	4
#define CASE_CHECK_5_BYTE	5
#define CASE_CHECK_6_BYTE	6


// defined in InterruptTable.asm
// must be defined there to "guarantee" the location of these variables at the
// beginning of the segment seg_dmda
extern const long CRC_Checksum_pm_rth;
extern const long CRC_Checksum_pm_init;
extern const long CRC_Checksum_pm_intern;
extern const long PM_InternalSegment_rth_Start;
extern const long PM_InternalSegment_rth_Stop;
extern const long PM_InternalSegment_init_Start;
extern const long PM_InternalSegment_init_Stop;
extern const long PM_InternalSegmentStart;
extern const long PM_InternalSegmentStop;
extern const long DM_InternalProgramStart;
extern const long DM_InternalProgramStop;
extern const long DM_InternalStart;
extern const long DM_InternalStop;
extern const long CRC_Checksum_pm_extern;
extern const long PM_ExternalSegmentStart;
extern const long PM_ExternalSegmentStop;
extern const long DM_ExternalStart;
extern const long DM_ExternalStop;


// Data memory check
static long *Access_seg_dmda_Ptr;
static long *Access_seg_pmda_Ptr;
static long *Access_ext_dmda_Ptr;

static long g_memoryCheckAlarms;
// Check patterns are chosen to write 1 and 0 to every bit
const long CheckPattern1 = 0xaaaaaaaa;
const long CheckPattern2 = 0x55555555;

/* LOCAL TYPE DECLARATIONS ***************************************************/

// structure crc values for status  
typedef struct{
	long	CRC_PM_Internal;
	long	CRC_PM_RTH;
	long	CRC_PM_INIT;
	long	CRC_PM_SDRAM;
}crc_status_type;


// structure for px register read
typedef struct{ 
	unsigned long  low;
  	unsigned long  high;	  
}px_reg_data;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static px_reg_data px_reg;
static crc_status_type crc_status;

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/
 
/*-------------------------------------------------------------------------*/               
/* void get_pm_inst(unsigned long *ptr_pm, struct px_reg_data *ptr_px)      */
/*							   											   */ 
/* Reads an instruction at location ptr_pm from the program memory         */
/* and returns it using the structure px_reg_data. The lower 16 bits       */
/* are written to px_reg_data.low and the upper 32 bits to                 */
/* px_reg_data.high                                                        */               
/*                                                                         */
/* Input: pointer to program memory location ptr_pm                        */
/* Ouput: px register content in structure px_reg_data                     */                                                   
/*-------------------------------------------------------------------------*/

// static prevents -e switch to get rid of function if not used
static void get_pm_inst(unsigned long *ptr_pm, px_reg_data *ptr_px)

{
  asm("i12=r4;");          // ptr_pm is loaded into DAG2 register i12 
  asm("px=pm(0,i12);");    // read instrucion from program memory 
  asm("i4=r8;");           // load DAG1 register i4 with ptr_px 
  asm("dm(0,i4)=px1;");    // lower 16 bits are saved in ptr_px->low 
  asm("dm(1,i4)=px2;");    // upper 32 bits are saved in ptr_px->high 
}    

/*-------------------------------------------------------------------------*/               
/* unsigned long calc_checksum32(unsigned long *ptr_pm, unsigned long crc_sum)*/
/*							   											   */ 
/* Computes the new CRC16 value given the address of the next 32 bit word  */
/* to be tested. Message words are split into 4 bytes and the new CRC      */ 
/* value is computed using the function crc16().                           */                                            
/*                                                                         */
/* Input: address of next 32 bit data word			                       */
/* Output: new CRC 16 value												   */
/*-------------------------------------------------------------------------*/

// static prevents -e switch to get rid of function if not used
/*static unsigned long SEGMENT_EXT_CODE calc_checksum32(unsigned long *ptr_pm, unsigned long crc_sum)
{
  long data;
  
  data = *ptr_pm;
  
  crc_sum = crc16(crc_sum,(data >> 24) & 0x000000ff); // crc sum for the 1. byte
  crc_sum = crc16(crc_sum,(data >> 16) & 0x000000ff); // crc sum for the 2. byte 
  crc_sum = crc16(crc_sum,(data >> 8) & 0x000000ff);  // crc sum for the 3. byte
  crc_sum = crc16(crc_sum,data & 0x000000ff); 		  // crc sum for the 4. byte
  
  return crc_sum;

}   
*/

/*-------------------------------------------------------------------------*/               
/* unsigned long calc_checksum48(unsigned long *ptr_pm, unsigned long crc_sum)*/
/*							   											   */ 
/* Computes the new CRC16 value given the address of the next 48 bit word  */
/* to be tested. Message words are split into 6 bytes and the new CRC      */ 
/* value is computed using the function crc16().                           */                                            
/*                                                                         */
/* Input: address of next program memory location, previous crc            */
/*		  ifExtern	TRUE - swap px_reg.high with .low (external access)	   */
/*					FALSE - use px_reg as is							   */
/* Output: new CRC 16 value												   */
/*																		   */
/*	px_reg.high	 0xXXXXXXXX		Bit 47-16 of 48-Bit instr.				   */
/*	px_reg.low	 0xXXXX0000		Bit 15-0 of 48-Bit instr.				   */
/*	given by get_pm_inst() by px1 and px2 - special for ADSP-21161		   */
/*-------------------------------------------------------------------------*/

static unsigned long SEGMENT_EXT_CODE calc_checksum48(unsigned long *ptr_pm, unsigned long crc_sum, BOOL ifExtern)
{                              
    unsigned long temp;
	// read 48 bit (6 bytes) instruction from pm                 
    get_pm_inst(ptr_pm, &px_reg); 

	if(ifExtern)
	{
		temp = px_reg.high;
		px_reg.high = px_reg.low;
		px_reg.low = temp;
	}

    crc_sum = crc16(crc_sum,(px_reg.high >> 24) & 0x000000ff);
     // crc sum for the 1. byte
    crc_sum = crc16(crc_sum,(px_reg.high >> 16) & 0x000000ff);
     // crc sum for the 2. byte 
    crc_sum = crc16(crc_sum,(px_reg.high >> 8) & 0x000000ff);
     // crc sum for the 3. byte
    crc_sum = crc16(crc_sum,px_reg.high & 0x000000ff);
	 // crc sum for the 4. byte 
	crc_sum = crc16(crc_sum,(px_reg.low >> 24) & 0x000000ff);
	 // crc sum for the 5. byte
	crc_sum = crc16(crc_sum,(px_reg.low >> 16) & 0x000000ff); 
	 // crc sum for the 6. byte

	return crc_sum;      
}


/////////////////////////////////
// Memory check functions during startup
//*************************************
// >>> A C H T U N G ! ! ! ! ! <<<<<<<<
//*************************************
//
// wird mit dem Emulator und Breakpoints gearbeitet stimmt die berechnete 
// CRC Summe nicht mit der Summe des PC's ueberein. 

// Returns 1 if CRC is not ok, 0 if it is ok
long SEGMENT_EXT_CODE CheckWholeProgrammMemorySegment(long *Start_PM_Ptr, long *Stop_PM_Ptr, long *Actual_CRC16, long Expected_CRC16)
{
	long Index;
	unsigned long TempCRC = 0;
	long NumberOfAddresses = 0;

	NumberOfAddresses =  ((long) Stop_PM_Ptr - (long) Start_PM_Ptr);
	if (NumberOfAddresses <= 0) return 1;
	for (Index = 0; Index < (NumberOfAddresses + 1); Index++)
	{
		TempCRC = calc_checksum48((unsigned long*) (Start_PM_Ptr + Index) , TempCRC, FALSE);
	}
	*Actual_CRC16 = TempCRC;
	if (TempCRC == Expected_CRC16) return 0;
	else return 1;
}

long SEGMENT_EXT_CODE CheckWholeProgrammMemorySegmentExtern(long *Start_PM_Ptr, long *Stop_PM_Ptr, long *Actual_CRC16, long Expected_CRC16)
{
	long Index;
	unsigned long TempCRC = 0;
	long NumberOfAddresses = 0;

	NumberOfAddresses =  ((long) Stop_PM_Ptr - (long) Start_PM_Ptr);
	if (NumberOfAddresses <= 0) return 1;
	for (Index = 0; Index < (NumberOfAddresses + 1); Index++)
	{
		TempCRC = calc_checksum48((unsigned long*) (Start_PM_Ptr + Index) , TempCRC, TRUE);
	}
	*Actual_CRC16 = TempCRC;
	if (TempCRC == Expected_CRC16) return 0;
	else return 1;
}


// Returns 1 if CRC is not ok, 0 if it is ok
/*long SEGMENT_EXT_CODE CheckWholeDataMemorySegmentCRC(long *Start_DM_Ptr, long *Stop_DM_Ptr, long *Actual_CRC16, long Expected_CRC16)
{
	long Index;
	static unsigned long TempCRC = 0;
	long NumberOfAddresses = 0;


	NumberOfAddresses = (long) (Stop_DM_Ptr - Start_DM_Ptr);
	if (NumberOfAddresses <= 0) return 1;
	for (Index = 0; Index < (NumberOfAddresses + 1); Index++)
	{
		TempCRC = calc_checksum32((unsigned long*) (Start_DM_Ptr + Index) , TempCRC);
	}
	*Actual_CRC16 = TempCRC;
	if (TempCRC == Expected_CRC16) return 0;
	else return 1;
}
*/
void SEGMENT_EXT_CODE InitializeDataMemoryCheck(void)
{
	Access_seg_dmda_Ptr = (long*) DM_InternalStart;
	Access_seg_pmda_Ptr = (long*) DM_InternalProgramStart;
	Access_ext_dmda_Ptr = (long*) DM_ExternalStart;
	g_memoryCheckAlarms = 0;
}

// Alarm if failing data memory check will never be set back again!!!!
// No interrupt must be enabled


void SEGMENT_EXT_CODE CheckWholeDataMemoryStartup(BOOL DoAlsoExternalSDRAM)
{
	register long *ActualAddressPtr;
	long save;
	unsigned long TempCRC16Value = 0;

	// Check internal data memory segment DM
	ActualAddressPtr = (long*) DM_InternalStart;
	while ((long) ActualAddressPtr < DM_InternalStop)
	{
		save = *ActualAddressPtr;						// store contents of ActualAddress
		*ActualAddressPtr = CheckPattern1;      // write CheckPattern1 to ActualAddress
		if (*ActualAddressPtr != CheckPattern1)	g_memoryCheckAlarms |= DATA_MEM_INTERN_ERROR;		 
		*ActualAddressPtr = CheckPattern2;		// write CheckPattern2 to ActualAddress
		if (*ActualAddressPtr != CheckPattern2)	g_memoryCheckAlarms |= DATA_MEM_INTERN_ERROR;
		*ActualAddressPtr = save;						// restore old value and increment pActualAddress
		ActualAddressPtr++;
	}
	// check internal data memory segment PM
	ActualAddressPtr = (long*) DM_InternalProgramStart;
	while ((long) ActualAddressPtr < DM_InternalProgramStop)
	{
		save = *ActualAddressPtr;						// store contents of ActualAddress
		*ActualAddressPtr = CheckPattern1;      // write CheckPattern1 to ActualAddress
		if (*ActualAddressPtr != CheckPattern1)	g_memoryCheckAlarms |= DATA_MEM_INTERN_PM_ERROR;		 
		*ActualAddressPtr = CheckPattern2;		// write CheckPattern2 to ActualAddress
		if (*ActualAddressPtr != CheckPattern2)	g_memoryCheckAlarms |= DATA_MEM_INTERN_PM_ERROR;
		*ActualAddressPtr = save;						// restore old value and increment pActualAddress
		ActualAddressPtr++;
	}
	if(DoAlsoExternalSDRAM)
	{
		// Check external data memory 
		ActualAddressPtr = (long*) DM_ExternalStart;
		while ((long) ActualAddressPtr < DM_ExternalStop)
		{
			save = *ActualAddressPtr;						// store contents of ActualAddress
			*ActualAddressPtr = CheckPattern1;      // write CheckPattern1 to ActualAddress
			if (*ActualAddressPtr != CheckPattern1)	g_memoryCheckAlarms |= DATA_MEM_SDRAM_ERROR;		 
			*ActualAddressPtr = CheckPattern2;		// write CheckPattern2 to ActualAddress
			if (*ActualAddressPtr != CheckPattern2)	g_memoryCheckAlarms |= DATA_MEM_SDRAM_ERROR;
			*ActualAddressPtr = save;						// restore old value and increment pActualAddress
			ActualAddressPtr++;
		}
	}
}



void SEGMENT_EXT_CODE MemoryCheckStartupSHARC(BOOL DoAlsoExternalSDRAM)
{

	InitializeDataMemoryCheck();
	// seg_pm_rth
	if (CheckWholeProgrammMemorySegment((long*) PM_InternalSegment_rth_Start, 
		(long*) PM_InternalSegment_rth_Stop, &crc_status.CRC_PM_RTH, CRC_Checksum_pm_rth)) 
	{
		g_memoryCheckAlarms |= CRC_PM_RTH_ERROR;
	}
	// seg_pm_init
/*	if (CheckWholeProgrammMemorySegment((long*) PM_InternalSegment_init_Start, 
		(long*) PM_InternalSegment_init_Stop, &crc_status.CRC_PM_INIT, CRC_Checksum_pm_init)) 
	{
		g_memoryCheckAlarms |= CRC_PM_INIT_ERROR;
		asm("nop;");
	}
*/
	// seg_pmco
	if (CheckWholeProgrammMemorySegment((long*) PM_InternalSegmentStart, 
		(long*) PM_InternalSegmentStop, &crc_status.CRC_PM_Internal, CRC_Checksum_pm_intern)) 
	{
		g_memoryCheckAlarms |= CRC_PM_ERROR;
	}

	if(DoAlsoExternalSDRAM)
	{
		// ext_pmco in SDRAM
		if (CheckWholeProgrammMemorySegmentExtern((long*) PM_ExternalSegmentStart, 
			(long*) PM_ExternalSegmentStop, &crc_status.CRC_PM_SDRAM, CRC_Checksum_pm_extern)) 
			g_memoryCheckAlarms |= CRC_PM_SDRAM_ERROR;
	}

	CheckWholeDataMemoryStartup(DoAlsoExternalSDRAM);
}

//-------------------------------------------------------------------------
// void check_data_memory()								     		       
// void check_program_memory(long *AlarmWord, long AlarmBit)							
//	Input:	long *AlarmWord:	a pointer to an alarm word, which contains the error bit
//			long AlarmBit:		a define value with the Bit representing the MEMDATAERR					   											   
//	Return value:	nothing
//							   											   
// Tests the data memory sections by cyclic read and write access on each  
// data memory location   												   
//                                                                         
//-------------------------------------------------------------------------

// Please note: When alarm is once set, there is no way to set it back!!!!
BOOL SEGMENT_EXT_CODE Check_Data_Memory_Intern(long *AlarmWord, long AlarmBit) 
{  
	static long save;
	register long *ActualAddressPtr;


	if ( (long) Access_seg_dmda_Ptr >  DM_InternalStop)
	{
		// go back to start
		Access_seg_dmda_Ptr = (long*) DM_InternalStart;
		return(TRUE);
	}
	else
	{
		// Check one data memory address,
		ActualAddressPtr =  Access_seg_dmda_Ptr;
		asm("bit clr mode1 IRPTEN;");					//  Global interrupt disable  
		save = *ActualAddressPtr;						// store contents of ActualAddress
		*ActualAddressPtr = CheckPattern1;      // write CheckPattern1 to ActualAddress
		if (*ActualAddressPtr != CheckPattern1)	*AlarmWord |= AlarmBit;		 
		*ActualAddressPtr = CheckPattern2;		// write CheckPattern2 to ActualAddress
		if (*ActualAddressPtr != CheckPattern2)	*AlarmWord |= AlarmBit;
		*ActualAddressPtr = save;						// restore old value and increment pActualAddress
		asm("bit set mode1 IRPTEN;"); 					//  Global interrupt enable
		ActualAddressPtr++;
		Access_seg_dmda_Ptr = ActualAddressPtr;
		return(FALSE);
	}
}

BOOL SEGMENT_EXT_CODE Check_Data_Memory_InternPM(long *AlarmWord, long AlarmBit)
{
	static long save;
	register long *ActualAddressPtr;


	if ( (long) Access_seg_pmda_Ptr >  DM_InternalProgramStop)
	{
		// go back to start
		Access_seg_pmda_Ptr = (long*) DM_InternalProgramStart;
		return(TRUE);
	}
	else
	{
		// Check one data memory address,
		ActualAddressPtr =  Access_seg_pmda_Ptr;
		asm("bit clr mode1 IRPTEN;");					//  Global interrupt disable  
		save = *ActualAddressPtr;						// store contents of ActualAddress
		*ActualAddressPtr = CheckPattern1;      // write CheckPattern1 to ActualAddress
		if (*ActualAddressPtr != CheckPattern1)	*AlarmWord |= AlarmBit;		 
		*ActualAddressPtr = CheckPattern2;		// write CheckPattern2 to ActualAddress
		if (*ActualAddressPtr != CheckPattern2)	*AlarmWord |= AlarmBit;
		*ActualAddressPtr = save;						// restore old value and increment pActualAddress
		asm("bit set mode1 IRPTEN;"); 					//  Global interrupt enable
		ActualAddressPtr++;
		Access_seg_pmda_Ptr = ActualAddressPtr;
		return(FALSE);
	}
}

BOOL SEGMENT_EXT_CODE Check_Data_Memory_SDRAM(long *AlarmWord, long AlarmBit) 
{
	static long save;
	register long *ActualAddressPtr; 

	if ( (long) Access_ext_dmda_Ptr >  DM_ExternalStop)
	{
		// go back to start
		Access_ext_dmda_Ptr = (long*) DM_ExternalStart;
		return(TRUE);
	}
	else
	{
		// Check one data memory address,
		ActualAddressPtr =  Access_ext_dmda_Ptr;
		asm("bit clr mode1 IRPTEN;");					//  Global interrupt disable  
		save = *ActualAddressPtr;						// store contents of ActualAddress
		*ActualAddressPtr = CheckPattern1;      // write CheckPattern1 to ActualAddress
		if (*ActualAddressPtr != CheckPattern1)	*AlarmWord |= AlarmBit;		 
		*ActualAddressPtr = CheckPattern2;		// write CheckPattern2 to ActualAddress
		if (*ActualAddressPtr != CheckPattern2)	*AlarmWord |= AlarmBit;
		*ActualAddressPtr = save;						// restore old value and increment pActualAddress
		asm("bit set mode1 IRPTEN;"); 					//  Global interrupt enable	
		ActualAddressPtr++;
		Access_ext_dmda_Ptr = ActualAddressPtr;
		return(FALSE);
	}
} 


/////////////////////////////////////////////////////////////////////////////////////////////////////
/// This implementation is not really beautiful and memory efficiently but it provides the most free 
/// computing time on Upconverter DSP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/////////////////////////////////////////////////////////////////////////////////////////////////////
void SEGMENT_EXT_CODE DoMemoryCheck(BOOL DoAlsoExternalSDRAM)
{
	static BOOL isCheckStarted = FALSE;
	static long *ProgramMemoryAccessPtr;
	static unsigned long TempCRC = 0;

	static unsigned long CheckWhichSegment = CHECK_CASE_PM_RTH;
	static unsigned long calc_checksum48Counter = 0;
	static unsigned long crc_sum = 0;

	unsigned long swapTemp;

	switch(CheckWhichSegment)
	{
	case CHECK_CASE_PM_RTH:
		// Check segment seg_pm_rth
		if (isCheckStarted)
		{
			if ((long) ProgramMemoryAccessPtr > PM_InternalSegment_rth_Stop)
			{
				crc_status.CRC_PM_RTH = TempCRC;
				if (TempCRC != CRC_Checksum_pm_rth) // there is no way to clear an alarm
				{
					g_memoryCheckAlarms |= CRC_PM_RTH_ERROR;
				}
				isCheckStarted = FALSE;
			//	CheckWhichSegment = CHECK_CASE_PM_INIT;
				CheckWhichSegment = CHECK_CASE_PM_INTERN;
			}
			else
			{
				// special implementation of calc_checksum48() - devide it in bytewise check to handle
				// limited compution time of Upconverter DSP!!!!!!!!!
				switch(calc_checksum48Counter)
				{
				case CASE_GET_PM_INSTR:
					get_pm_inst((unsigned long*)ProgramMemoryAccessPtr, &px_reg); 
					
					// swap px_reg.high with px_reg.low if extern

					calc_checksum48Counter++;
					break;
				case CASE_CHECK_1_BYTE:
					crc_sum = crc16(TempCRC,(px_reg.high >> 24) & 0x000000ff);
					// crc sum for the 1. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_2_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.high >> 16) & 0x000000ff);
					 // crc sum for the 2. byte 
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_3_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.high >> 8) & 0x000000ff);
					// crc sum for the 3. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_4_BYTE:
					crc_sum = crc16(crc_sum,px_reg.high & 0x000000ff);
					 // crc sum for the 4. byte 
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_5_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.low >> 24) & 0x000000ff);
					 // crc sum for the 5. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_6_BYTE:
					TempCRC = crc16(crc_sum,(px_reg.low >> 16) & 0x000000ff); 
					 // crc sum for the 6. byte
					calc_checksum48Counter = 0;
					ProgramMemoryAccessPtr++;
					break;
				default:
					calc_checksum48Counter = 0;

				}//end switch(calc_checksum48Counter)
			}
		}
		else
		{
			ProgramMemoryAccessPtr = (long*) PM_InternalSegment_rth_Start;
			TempCRC = 0;
			isCheckStarted = TRUE;
		}
		break;
/*	case CHECK_CASE_PM_INIT:
		// Check segment seg_pm_Init
		if (isCheckStarted)
		{
			if ((long) ProgramMemoryAccessPtr > PM_InternalSegment_init_Stop)
			{
				crc_status.CRC_PM_INIT = TempCRC;
				if (TempCRC != CRC_Checksum_pm_init) // there is no way to clear an alarm
				{
					asm("nop;");
				//	g_memoryCheckAlarms |= CRC_PM_INIT_ERROR;
				}
				isCheckStarted = FALSE;
				CheckWhichSegment = CHECK_CASE_PM_INTERN;
			}
			else
			{
				// special implementation of calc_checksum48() - devide it in bytewise check to handle
				// limited compution time of Upconverter DSP!!!!!!!!!
				switch(calc_checksum48Counter)
				{
				case CASE_GET_PM_INSTR:
					get_pm_inst((unsigned long*)ProgramMemoryAccessPtr, &px_reg); 
					
					// swap px_reg.high with px_reg.low if extern

					calc_checksum48Counter++;
					break;
				case CASE_CHECK_1_BYTE:
					crc_sum = crc16(TempCRC,(px_reg.high >> 24) & 0x000000ff);
					// crc sum for the 1. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_2_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.high >> 16) & 0x000000ff);
					 // crc sum for the 2. byte 
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_3_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.high >> 8) & 0x000000ff);
					// crc sum for the 3. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_4_BYTE:
					crc_sum = crc16(crc_sum,px_reg.high & 0x000000ff);
					 // crc sum for the 4. byte 
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_5_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.low >> 24) & 0x000000ff);
					 // crc sum for the 5. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_6_BYTE:
					TempCRC = crc16(crc_sum,(px_reg.low >> 16) & 0x000000ff); 
					 // crc sum for the 6. byte
					calc_checksum48Counter = 0;
					ProgramMemoryAccessPtr++;
					break;
				default:
					calc_checksum48Counter = 0;

				}//end switch(calc_checksum48Counter)

			//	TempCRC = calc_checksum48((unsigned long*)ProgramMemoryAccessPtr , TempCRC, FALSE);
			//	ProgramMemoryAccessPtr++;
			}
		}
		else
		{
			ProgramMemoryAccessPtr = (long*) PM_InternalSegment_init_Start;
			TempCRC = 0;
			isCheckStarted = TRUE;
		}
		break;
*/
	case CHECK_CASE_PM_INTERN:
		// Check segment seg_pmco
		if (isCheckStarted)
		{
			if ((long) ProgramMemoryAccessPtr > PM_InternalSegmentStop)
			{
				crc_status.CRC_PM_Internal = TempCRC;
				if (TempCRC != CRC_Checksum_pm_intern) // there is no way to clear an alarm
				{
					g_memoryCheckAlarms |= CRC_PM_ERROR;
				}
				isCheckStarted = FALSE;
				CheckWhichSegment = CHECK_CASE_PM_EXTERN;
			}
			else
			{
				// special implementation of calc_checksum48() - devide it in bytewise check to handle
				// limited compution time of Upconverter DSP!!!!!!!!!
				switch(calc_checksum48Counter)
				{
				case CASE_GET_PM_INSTR:
					get_pm_inst((unsigned long*)ProgramMemoryAccessPtr, &px_reg); 
					
					// swap px_reg.high with px_reg.low if extern

					calc_checksum48Counter++;
					break;
				case CASE_CHECK_1_BYTE:
					crc_sum = crc16(TempCRC,(px_reg.high >> 24) & 0x000000ff);
					// crc sum for the 1. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_2_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.high >> 16) & 0x000000ff);
					 // crc sum for the 2. byte 
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_3_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.high >> 8) & 0x000000ff);
					// crc sum for the 3. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_4_BYTE:
					crc_sum = crc16(crc_sum,px_reg.high & 0x000000ff);
					 // crc sum for the 4. byte 
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_5_BYTE:
					crc_sum = crc16(crc_sum,(px_reg.low >> 24) & 0x000000ff);
					 // crc sum for the 5. byte
					calc_checksum48Counter++;
					break;
				case CASE_CHECK_6_BYTE:
					TempCRC = crc16(crc_sum,(px_reg.low >> 16) & 0x000000ff); 
					 // crc sum for the 6. byte
					calc_checksum48Counter = 0;
					ProgramMemoryAccessPtr++;
					break;
				default:
					calc_checksum48Counter = 0;

				}//end switch(calc_checksum48Counter)
			}
		}
		else
		{
			ProgramMemoryAccessPtr = (long*) PM_InternalSegmentStart;
			TempCRC = 0;
			isCheckStarted = TRUE;
		}
		break;
	case CHECK_CASE_PM_EXTERN:
		if(DoAlsoExternalSDRAM)
		{
			// Check segment ext_pmco in SDRAM
			if (isCheckStarted)
			{
				if (PM_ExternalSegmentStop < ((long) ProgramMemoryAccessPtr))
				{
					crc_status.CRC_PM_SDRAM = TempCRC;
					if (TempCRC != CRC_Checksum_pm_extern) // there is no way to clear an alarm
					{
						g_memoryCheckAlarms |= CRC_PM_SDRAM_ERROR;
					}
					isCheckStarted = FALSE;
					CheckWhichSegment = CHECK_CASE_DM_INTERN_PM;
				}
				else
				{
					// special implementation of calc_checksum48() - devide it in bytewise check to handle
					// limited compution time of Upconverter DSP!!!!!!!!!
					switch(calc_checksum48Counter)
					{
					case CASE_GET_PM_INSTR:
						get_pm_inst((unsigned long*)ProgramMemoryAccessPtr, &px_reg); 
						
						// swap px_reg.high with px_reg.low if extern
						swapTemp = px_reg.high;
						px_reg.high = px_reg.low;
						px_reg.low = swapTemp;

						calc_checksum48Counter++;
						break;
					case CASE_CHECK_1_BYTE:
						crc_sum = crc16(TempCRC,(px_reg.high >> 24) & 0x000000ff);
						// crc sum for the 1. byte
						calc_checksum48Counter++;
						break;
					case CASE_CHECK_2_BYTE:
						crc_sum = crc16(crc_sum,(px_reg.high >> 16) & 0x000000ff);
						 // crc sum for the 2. byte 
						calc_checksum48Counter++;
						break;
					case CASE_CHECK_3_BYTE:
						crc_sum = crc16(crc_sum,(px_reg.high >> 8) & 0x000000ff);
						// crc sum for the 3. byte
						calc_checksum48Counter++;
						break;
					case CASE_CHECK_4_BYTE:
						crc_sum = crc16(crc_sum,px_reg.high & 0x000000ff);
						 // crc sum for the 4. byte 
						calc_checksum48Counter++;
						break;
					case CASE_CHECK_5_BYTE:
						crc_sum = crc16(crc_sum,(px_reg.low >> 24) & 0x000000ff);
						 // crc sum for the 5. byte
						calc_checksum48Counter++;
						break;
					case CASE_CHECK_6_BYTE:
						TempCRC = crc16(crc_sum,(px_reg.low >> 16) & 0x000000ff); 
						 // crc sum for the 6. byte
						calc_checksum48Counter = 0;
						ProgramMemoryAccessPtr++;
						break;
					default:
						calc_checksum48Counter = 0;

					}//end switch(calc_checksum48Counter)
				}
			}
			else
			{
				ProgramMemoryAccessPtr = (long*) PM_ExternalSegmentStart;
				TempCRC = 0;
				isCheckStarted = TRUE;
			}
		}
		else
		{
			isCheckStarted = FALSE;
			CheckWhichSegment = CHECK_CASE_DM_INTERN_PM;
		}
		break;
	case CHECK_CASE_DM_INTERN_PM:
		if(Check_Data_Memory_InternPM(&g_memoryCheckAlarms, DATA_MEM_INTERN_PM_ERROR))
		{
			CheckWhichSegment = CHECK_CASE_DM_INTERN;
		}
		break;
	case CHECK_CASE_DM_INTERN:
		if(Check_Data_Memory_Intern(&g_memoryCheckAlarms, DATA_MEM_INTERN_ERROR))
		{
			CheckWhichSegment = CHECK_CASE_DM_EXTERN;
		}
		break;
	case CHECK_CASE_DM_EXTERN:
		if(DoAlsoExternalSDRAM)
		{
			if(Check_Data_Memory_SDRAM(&g_memoryCheckAlarms, DATA_MEM_SDRAM_ERROR))
			{
				CheckWhichSegment = CHECK_CASE_PM_RTH;
			}
		}
		else
		{
			CheckWhichSegment = CHECK_CASE_PM_RTH;
		}
		break;
	default:
		break;
	}

}


BOOL SEGMENT_EXT_CODE memoryCheckDSP_CRC_Alarm(void)
{
	if(g_memoryCheckAlarms) // special return for bitfields
	{
		return (TRUE);
	}
	else
	{
		return (FALSE);
	}
}




void SEGMENT_EXT_CODE memoryCheckRespondToMaster(void)
{

////////////////////////////////////////////////////////////////////////////////////////////
/////// Magic Word exchange with Master DSP	
	*(long *)SPICTL = 0x00000000;	   			// clear SPI Reg
	asm("nop;");
	asm("bit set mode1 IRPTEN;");				// enable global Interrupt
	asm("nop;");

	*(long *)SPICTL = WL32 | BAUDR2 | SPIEN | NSMLS | SPTINT | SPRINT | SENDZ;
	
	if(g_memoryCheckAlarms)						// CRC Error detected! Do not send Magic Word
	{
		*(long *)SPITX = 0x00000000;
	}
	else										// No CRC Error 
	{
		*(long *)SPITX = 0x12345678;			// Magic Word !!!!!!!
	}
	
	while(!((*(long *)SPISTAT & (RXS0))))		// wait for data shifted in by Master DSP
	{
		asm("nop;");
	}    
	asm("bit clr mode1 IRPTEN;");				// disable global Interrupt
/////////////////////////////////////////////////////////////////////////////////////////////
}


