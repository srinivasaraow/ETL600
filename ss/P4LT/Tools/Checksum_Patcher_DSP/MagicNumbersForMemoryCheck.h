/*-------------------------------------------------------------------------*
 * $Workfile: MagicNumbersForMemoryCheck.h $
 * Part of	: ETL600 P4LT
 * Language	: C
 * Created by	: 
 * Remarks	:  
 * Purpose	: Magic numbers for variables containing checksums. External patch
 *			  program looks for those magic numbers and replaces them with
 *			  real calculated CRCs and used memory area.
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Tools/Checksum_Patcher_DSP/MagicNumbersForMemoryCheck.h $
 * 
 * 3     19.01.06 16:37 Chmibuh
 * segment ext_slaveCode added
 * 
 * 2     6.05.05 15:32 Chbrbus
 * Added MEMORY keyword for LDF file
 * 
 * 1     19.01.05 15:50 Chchlee
 * 
 * 2     13.01.05 17:15 Chbrbus
 * DM_InternalProgramStart and DM_InternalProgramStop added
 * 
 * 1     13.01.05 13:43 Chbrbus
 * 
 * 
 *-------------------------------------------------------------------------*/
 
#ifndef MAGIC_NUMBERS_PROGRAM_MEMORY_CHECK
#define MAGIC_NUMBERS_PROGRAM_MEMORY_CHECK

// these magic numbers are reserved for CRC checksums of every possible segment
#define	 CRC_CHECKSUM_PM_RTH_DEFAULT			0x11223344
#define	 CRC_CHECKSUM_PM_INIT_DEFAULT			0x55667788         
#define	 CRC_CHECKSUM_PM_INTERN_DEFAULT			0x99aabbcc     
#define  CRC_CHECKSUM_PM_EXTERN_DEFAULT			0xddeeff00
// For Master DSP only
#define  CRC_CHECKSUM_OVER_ALL_CODE				0x12341234

// these magic numbers are reserved for start/stop adresses of every possible segment
#define	 PM_INTERNAL_SEGMENT_RTH_START_DEFAULT	0x11111111     
#define  PM_INTERNAL_SEGMENT_RTH_STOP_DEFAULT	0x22222222
#define	 PM_INTERNAL_SEGMENT_INIT_START_DEFAULT	0x33333333    
#define  PM_INTERNAL_SEGMENT_INIT_STOP_DEFAULT	0x44444444
#define	 PM_INTERNAL_SEGMENT_START_DEFAULT		0x55555555     
#define  PM_INTERNAL_SEGMENT_STOP_DEFAULT		0x66666666
#define  DM_PROGRAM_INTERNAL_START_DEFAULT		0x77777777
#define  DM_PROGRAM_INTERNAL_STOP_DEFAULT		0x88888888
#define  DM_INTERNAL_START_DEFAULT				0x99999999
#define  DM_INTERNAL_STOP_DEFAULT				0xaaaaaaaa
#define  PM_EXTERNAL_SEGMENT_START_DEFAULT		0xbbbbbbbb
#define  PM_EXTERNAL_SEGMENT_STOP_DEFAULT		0xcccccccc
#define  DM_EXTERNAL_START_DEFAULT				0xdddddddd
#define  DM_EXTERNAL_STOP_DEFAULT				0xeeeeeeee


// Magic keywords used in .ldf file 
// Purpose: this is nesessary to find out segment start adress by reading the *.ldf file
#define MAGIC_CHAR_LDF_INTERN_PM_RTH			"seg_rth"
#define MAGIC_CHAR_LDF_INTERN_PM_INIT			"seg_init"
#define MAGIC_CHAR_LDF_INTERN_PM				"seg_pmco"
#define MAGIC_CHAR_LDF_INTERN_PM_DATA			"seg_pmda"
#define MAGIC_CHAR_LDF_INTERN_DM_DATA			"seg_dmda"		// Checksums has to be written here
#define MAGIC_CHAR_LDF_EXTERN_PM				"ext_code"
#define MAGIC_CHAR_LDF_EXTERN_DM				"ext_dmda"
// For Master DSP only
#define MAGIC_CHAR_LDF_EXTERN_DM_SlaveCode		"ext_slaveCode"

#define MAGIC_CHAR_LDF_START					"START"
#define MAGIC_CHAR_LDF_END						"END"
#define MAGIC_CHAR_LDF_WIDTH					"WIDTH"
#define MAGIC_CHAR_LDF_MEMORY					"MEMORY"

#endif
