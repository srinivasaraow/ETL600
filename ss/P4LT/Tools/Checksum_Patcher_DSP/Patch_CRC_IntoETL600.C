/*-------------------------------------------------------------------------*
 * $Workfile: Patch_CRC_IntoETL600.C $
 * Part of	:	ETL600
 * Language	:	C
 * Created by:	Bruno Busslinger PTUKT2
 * Remarks	:   
 * Purpose	:	Inserting CRC Checksums into firmware on ETL600
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Tools/Checksum_Patcher_DSP/Patch_CRC_IntoETL600.C $ 
 * 
 * 5     19.01.06 16:37 Chmibuh
 * segment ext_slaveCode added and CRC calculation if isItMasterDSP
 * 
 * 4     18.05.05 15:45 Chbrbus
 * Removed bug in writing back end adress of program memory check if an
 * offset is used
 * 
 * 3     18.05.05 11:21 Chbrbus
 * Excluded memory check range 0x40000 - 0x4000B
 * 
 * 2     18.05.05 10:08 Chbrbus
 * excluded first 8 adresses in segment seg_rth (bootloader is changing
 * the adress 0x40005) 
 * 
 * 1     9.05.05 16:24 Chbrbus
 *  
 * 
 *-------------------------------------------------------------------------*
 */      

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "Elf32_DefinitionsForSharcCRC.h"
#include "MagicNumbersForMemoryCheck.h"
#include "ConvertingFunctions.h"
#include "Patch_CRC_IntoETL600.h"


#define myFALSE 0
#define myTRUE	1

// for every segment to be checked
SHARC_MemorySegmentInformationType SegmentInternalRTH_PM;
SHARC_MemorySegmentInformationType SegmentInternalINIT_PM;
SHARC_MemorySegmentInformationType SegmentInternalPROGRAM_PM;
SHARC_MemorySegmentInformationType SegmentInternalDATA_PM;
SHARC_MemorySegmentInformationType SegmentInternalDATA_DM;
SHARC_MemorySegmentInformationType SegmentExternalPROGRAM_PM;
SHARC_MemorySegmentInformationType SegmentExternalDATA_DM;
// For Master DSP only
SHARC_MemorySegmentInformationType SegmentExternalDATA_DM_SlaveCode;


char SegmentNameLDF_seg_rth[] = MAGIC_CHAR_LDF_INTERN_PM_RTH;
char SegmentNameLDF_seg_init[] = MAGIC_CHAR_LDF_INTERN_PM_INIT;
char SegmentNameLDF_seg_pmco[] = MAGIC_CHAR_LDF_INTERN_PM;
char SegmentNameLDF_seg_pmda[] = MAGIC_CHAR_LDF_INTERN_PM_DATA;
char SegmentNameLDF_seg_dmda[] = MAGIC_CHAR_LDF_INTERN_DM_DATA;
char SegmentNameLDF_ext_code[] = MAGIC_CHAR_LDF_EXTERN_PM;
char SegmentNameLDF_ext_dmda[] = MAGIC_CHAR_LDF_EXTERN_DM;
// For Master DSP only
char SegmentNameLDF_ext_dmda_SlaveCode[] = MAGIC_CHAR_LDF_EXTERN_DM_SlaveCode;


char isItMasterDSP;


void InitMemorySegmentInformationWithDefaultValues(void)
{
	// segment seg_rth
	SegmentInternalRTH_PM.CRC16_ChecksumIfProgramMemory = CRC_CHECKSUM_PM_RTH_DEFAULT;
	SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
	SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
	SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
	SegmentInternalRTH_PM.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = PM_INTERNAL_SEGMENT_RTH_START_DEFAULT;
	SegmentInternalRTH_PM.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = PM_INTERNAL_SEGMENT_RTH_STOP_DEFAULT;
	SegmentInternalRTH_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
	SegmentInternalRTH_PM.SegmentNameInLDF = &SegmentNameLDF_seg_rth[0];
	// segment seg_init
	SegmentInternalINIT_PM.CRC16_ChecksumIfProgramMemory = CRC_CHECKSUM_PM_INIT_DEFAULT;
	SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
	SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
	SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
	SegmentInternalINIT_PM.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = PM_INTERNAL_SEGMENT_INIT_START_DEFAULT;
	SegmentInternalINIT_PM.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = PM_INTERNAL_SEGMENT_INIT_STOP_DEFAULT;
	SegmentInternalINIT_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
	SegmentInternalINIT_PM.SegmentNameInLDF = &SegmentNameLDF_seg_init[0];
	// segment seg_pmco
	SegmentInternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory = CRC_CHECKSUM_PM_INTERN_DEFAULT;
	SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
	SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
	SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
	SegmentInternalPROGRAM_PM.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = PM_INTERNAL_SEGMENT_START_DEFAULT;
	SegmentInternalPROGRAM_PM.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = PM_INTERNAL_SEGMENT_STOP_DEFAULT;
	SegmentInternalPROGRAM_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
	SegmentInternalPROGRAM_PM.SegmentNameInLDF = &SegmentNameLDF_seg_pmco[0];
	// segment seg_pmda
	SegmentInternalDATA_PM.CRC16_ChecksumIfProgramMemory = 0;
	SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
	SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
	SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
	SegmentInternalDATA_PM.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = DM_PROGRAM_INTERNAL_START_DEFAULT;
	SegmentInternalDATA_PM.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = DM_PROGRAM_INTERNAL_STOP_DEFAULT;
	SegmentInternalDATA_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
	SegmentInternalDATA_PM.SegmentNameInLDF = &SegmentNameLDF_seg_pmda[0];
	// segment seg_dmda
	SegmentInternalDATA_DM.CRC16_ChecksumIfProgramMemory = 0;
	SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
	SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
	SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
	SegmentInternalDATA_DM.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = DM_INTERNAL_START_DEFAULT;
	SegmentInternalDATA_DM.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = DM_INTERNAL_STOP_DEFAULT;
	SegmentInternalDATA_DM.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
	SegmentInternalDATA_DM.SegmentNameInLDF = &SegmentNameLDF_seg_dmda[0];
	// segment ext_code
	SegmentExternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory = CRC_CHECKSUM_PM_EXTERN_DEFAULT;
	SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
	SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
	SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
	SegmentExternalPROGRAM_PM.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = PM_EXTERNAL_SEGMENT_START_DEFAULT;
	SegmentExternalPROGRAM_PM.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = PM_EXTERNAL_SEGMENT_STOP_DEFAULT;
	SegmentExternalPROGRAM_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
	SegmentExternalPROGRAM_PM.SegmentNameInLDF = &SegmentNameLDF_ext_code[0];
	// segment ext_dmda
	SegmentExternalDATA_DM.CRC16_ChecksumIfProgramMemory = 0;
	SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
	SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
	SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
	SegmentExternalDATA_DM.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = DM_EXTERNAL_START_DEFAULT;
	SegmentExternalDATA_DM.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = DM_EXTERNAL_STOP_DEFAULT;
	SegmentExternalDATA_DM.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
	SegmentExternalDATA_DM.SegmentNameInLDF = &SegmentNameLDF_ext_dmda[0];

	if(isItMasterDSP)
	{
		// segment ext_slaveCode
		SegmentExternalDATA_DM_SlaveCode.CRC16_ChecksumIfProgramMemory = CRC_CHECKSUM_OVER_ALL_CODE;
		SegmentExternalDATA_DM_SlaveCode.LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 0;
		SegmentExternalDATA_DM_SlaveCode.LDF_MemorySegmentDefinitions.StopAdressMemorySegment = 0;
		SegmentExternalDATA_DM_SlaveCode.LDF_MemorySegmentDefinitions.WidthMemorySegment = 0;
		SegmentExternalDATA_DM_SlaveCode.ELF_MemorySegmentBoundaries.StartAdressMemorySegment = 0;
		SegmentExternalDATA_DM_SlaveCode.ELF_MemorySegmentBoundaries.StopAdressMemorySegment = 0;
		SegmentExternalDATA_DM_SlaveCode.ELF_MemorySegmentBoundaries.WidthMemorySegment = 0;
		SegmentExternalDATA_DM_SlaveCode.SegmentNameInLDF = &SegmentNameLDF_ext_dmda_SlaveCode[0];
	}

}


long DecodeInformationInLDFforaSegment(FILE *LDF_FilePtr, SHARC_MemorySegmentInformationType *SHARC_MemorySegmentInfo, long LengthOfMagicWordInChar)
{
	long Index = 0;
	long SegmentIsFound = 0;
	long MemoryKeyWordIsFound = 0;
	char TempReadValueCharArr[20];
	char TempReadByteFromFile;
	long ShiftIndex = 0;
	char StartChar[5];
	char EndChar[3];
	char WidthChar[5];
	char MemoryChar[6];
	char StartAddressInternalPorgrammMemoryCharArr[10];
	char StopAddressInternalPorgrammMemoryCharArr[10];
	char WidthAddressInternalPorgrammMemoryCharArr[2];
	char StartSegmentMagicWordArr[] = MAGIC_CHAR_LDF_START;
	char EndSegmentMagicWordArr[] = MAGIC_CHAR_LDF_END;
	char WidthSegmentMagicWordArr[] = MAGIC_CHAR_LDF_WIDTH;
	char MagicWordMemoryArr[] = MAGIC_CHAR_LDF_MEMORY;

	// search first for the "MEMORY" keyword in LDF file (ignore commands)
	fseek(LDF_FilePtr, 0, SEEK_SET);
	while( (!feof( LDF_FilePtr )) && (MemoryKeyWordIsFound == 0) )
	{
		fseek(LDF_FilePtr, Index, SEEK_SET);
		fread((char*)&(MemoryChar), sizeof(char), sizeof(MemoryChar), LDF_FilePtr);
		if (StringArrayCompare(MemoryChar,  MagicWordMemoryArr, sizeof(MemoryChar)))
		{
			MemoryKeyWordIsFound = 1;
			Index = Index + sizeof(MemoryChar);
		}
		Index++;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!!! CAUTION
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	// DO NEVER USE a "//" in the LDF file to desactivate a segment or mark an old segment. This won't be detected 
	// by this function. Filepatcher might no longer work !!!!!!
	while( !feof( LDF_FilePtr ))
	{
		fseek(LDF_FilePtr, Index, SEEK_SET);
		fread((char*)&(TempReadByteFromFile), sizeof(char), sizeof(TempReadByteFromFile), LDF_FilePtr);
		if (TempReadByteFromFile == SHARC_MemorySegmentInfo->SegmentNameInLDF[0])
		{
			fseek(LDF_FilePtr, Index, SEEK_SET);
			fread((char*)&(TempReadValueCharArr), sizeof(char), sizeof(TempReadValueCharArr), LDF_FilePtr);
			if ((StringArrayCompare(TempReadValueCharArr,  SHARC_MemorySegmentInfo->SegmentNameInLDF, LengthOfMagicWordInChar) && (SegmentIsFound == 0)))
			{
				SegmentIsFound = 1;
				fprintf(stdout, "Address segment ");
				fprintf(stdout, SHARC_MemorySegmentInfo->SegmentNameInLDF);
				fprintf(stdout," successfully found \n");
				for (ShiftIndex = sizeof(SHARC_MemorySegmentInfo->SegmentNameInLDF); ShiftIndex < 80; ShiftIndex++)
				{
					fseek(LDF_FilePtr, (Index + ShiftIndex), SEEK_SET);
					fread((char*)&(TempReadByteFromFile), sizeof(char), sizeof(TempReadByteFromFile), LDF_FilePtr);
					if (TempReadByteFromFile == StartSegmentMagicWordArr[0])
					{
						fseek(LDF_FilePtr, (Index + ShiftIndex), SEEK_SET);
						fread((char*)&(StartChar), sizeof(char), sizeof(StartChar), LDF_FilePtr);
						if (StringArrayCompare(StartChar, StartSegmentMagicWordArr, sizeof(StartChar)))
						{
							fseek(LDF_FilePtr, (Index + ShiftIndex + 6), SEEK_SET);
							fread((char*)&(StartAddressInternalPorgrammMemoryCharArr), sizeof(char), sizeof(StartAddressInternalPorgrammMemoryCharArr), LDF_FilePtr);
							SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StartAdressMemorySegment = ConvertAddressInASCIIintoLong(StartAddressInternalPorgrammMemoryCharArr);
						}
					}
					if (TempReadByteFromFile == EndSegmentMagicWordArr[0])
					{
						fseek(LDF_FilePtr, (Index + ShiftIndex), SEEK_SET);
						fread((char*)&(EndChar), sizeof(char), sizeof(EndChar), LDF_FilePtr);
						if (StringArrayCompare(EndChar, EndSegmentMagicWordArr, sizeof(EndChar)))
						{
							fseek(LDF_FilePtr, (Index + ShiftIndex + 4), SEEK_SET);
							fread((char*)&(StopAddressInternalPorgrammMemoryCharArr), sizeof(char), sizeof(StopAddressInternalPorgrammMemoryCharArr), LDF_FilePtr);
							SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StopAdressMemorySegment = ConvertAddressInASCIIintoLong(StopAddressInternalPorgrammMemoryCharArr);
						}
					}
					if (TempReadByteFromFile == WidthSegmentMagicWordArr[0])
					{
						fseek(LDF_FilePtr, (Index + ShiftIndex), SEEK_SET);
						fread((char*)&(WidthChar), sizeof(char), sizeof(WidthChar), LDF_FilePtr);
						if (StringArrayCompare(WidthChar, WidthSegmentMagicWordArr, sizeof(WidthChar)))
						{
							fseek(LDF_FilePtr, (Index + ShiftIndex + 6), SEEK_SET);
							fread((char*)&(WidthAddressInternalPorgrammMemoryCharArr), sizeof(char), sizeof(WidthAddressInternalPorgrammMemoryCharArr), LDF_FilePtr);
							SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.WidthMemorySegment = ConvertAddressInASCIIintoLong(WidthAddressInternalPorgrammMemoryCharArr);
						}
					}
				}
			}
		}
		Index++;
	}
	return SegmentIsFound;
}



unsigned long CalculateOverAllChecksum(void)
{
	unsigned short CRC_TempSum = 0;
	unsigned short count, index;
	
	unsigned long checksums[10];

	count = 0;

	// use CRC of seg_rth, seg_init, seg_pmco, ext_code and ext_slaveCode to get an overall CRC sum 
	checksums[count++] = SegmentInternalRTH_PM.CRC16_ChecksumIfProgramMemory;
	checksums[count++] = SegmentInternalINIT_PM.CRC16_ChecksumIfProgramMemory;	
	checksums[count++] = SegmentInternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory;	
	checksums[count++] = SegmentExternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory;
	checksums[count++] = SegmentExternalDATA_DM_SlaveCode.CRC16_ChecksumIfProgramMemory;

	for(index = 0;index < count;index++)
	{
		CRC_TempSum = crc16(CRC_TempSum, ((unsigned char) checksums[index]));
	}

	return (CRC_TempSum);
}


void CalculateChecksumForMemorySegment(FILE *DXE_InputPtr, ELF_DecodedFileHeaderType *ELF_DecodedFileHeaderInformation, SHARC_MemorySegmentInformationType *SHARC_MemorySegmentInfo, long StartOffset)
{
	long Index = 0;
	long LengthOfSection = 0;
	unsigned short CRC_TempSum = 0;
	unsigned short TempValueForCRC;
	long CounterFilePosition = 0;
	long OffsetInDXE_file = 0;

	fseek(DXE_InputPtr, 0, SEEK_SET);									// go back to start of file
	SHARC_MemorySegmentInfo->CRC16_ChecksumIfProgramMemory = 0;			// Start value for CRC segemnt calculation
	for (Index = 0; Index < ELF_DecodedFileHeaderInformation->NumberOfConfirmedSectionHeaders; Index++)
	{								   									   
		if (ELF_DecodedFileHeaderInformation->PhysicalAddressSharc[Index] == SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StartAdressMemorySegment)
		{
			LengthOfSection = 0;
			OffsetInDXE_file = ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[Index] + (StartOffset * (SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.WidthMemorySegment / 8));
			for (CounterFilePosition = OffsetInDXE_file; CounterFilePosition < (ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[Index + 1]); CounterFilePosition++)
			{
				fseek(DXE_InputPtr, CounterFilePosition, SEEK_SET);
				fread((char*)&TempValueForCRC, sizeof(char), sizeof(char), DXE_InputPtr);
				CRC_TempSum = crc16(CRC_TempSum, (unsigned char) TempValueForCRC);
				LengthOfSection++;
			}
			//Calculating stop address of actual segment
			LengthOfSection = LengthOfSection / (SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.WidthMemorySegment / 8);
			if (SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StopAdressMemorySegment >= (SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StartAdressMemorySegment + LengthOfSection))
			SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StopAdressMemorySegment = SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StartAdressMemorySegment + LengthOfSection  + StartOffset - 1;
			// when an offset is used, the start adress needs to be modified
			if (StartOffset > 0)
			{
				SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StartAdressMemorySegment = 
					SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StartAdressMemorySegment + StartOffset;
			}

		}
	}
	SHARC_MemorySegmentInfo->CRC16_ChecksumIfProgramMemory = CRC_TempSum;
}



long FindAdressToWriteIntoDXE(FILE *DXE_InputPtr,ELF_DecodedFileHeaderType *ELF_DecodedFileHeaderInformation, SHARC_MemorySegmentInformationType *SHARC_MemorySegmentInfo)
{
	long Index = 0;
	long AddressToWriteCRCInDXE_File = 0;

	fseek(DXE_InputPtr, 0, SEEK_SET);
	for (Index = 0; Index < ELF_DecodedFileHeaderInformation->NumberOfConfirmedSectionHeaders; Index++)
	{
		if (ELF_DecodedFileHeaderInformation->PhysicalAddressSharc[Index] == SHARC_MemorySegmentInfo->LDF_MemorySegmentDefinitions.StartAdressMemorySegment)
		{
			AddressToWriteCRCInDXE_File = ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[Index];
		}
	}
	return AddressToWriteCRCInDXE_File;
}

void DecodeELF_HeaderInformation(Elf386_External_Ehdr *ElfFileHeader, ELF_DecodedFileHeaderType *ELF_DecodedFileHeaderInformation, FILE *ELF_File)
{
	long Index;
	static Elf386_External_Shdr ElfSectionHeader[400];	// 400 are enough

	// Decode nessessary file header information
	ELF_DecodedFileHeaderInformation->NumberOfSectionHeaders = ((long) ElfFileHeader->e_shnum[0]) + (((long) ElfFileHeader->e_shnum[1]) << 8);
	ELF_DecodedFileHeaderInformation->ELF_FileHeaderSize = ((long) ElfFileHeader->e_ehsize[0]) + (((long) ElfFileHeader->e_ehsize[1]) << 8);
	ELF_DecodedFileHeaderInformation->SectionHeaderOffset = ELF_DecodedFileHeaderInformation->ELF_FileHeaderSize + ((long) ElfFileHeader->e_shoff[0]) + (((long) ElfFileHeader->e_shoff[1]) << 8) + (((long) ElfFileHeader->e_shoff[2]) << 16) + (((long) ElfFileHeader->e_shoff[3]) << 24);
	ELF_DecodedFileHeaderInformation->SectionHeaderSize = ((long) ElfFileHeader->e_shentsize[0]) + (((long) ElfFileHeader->e_shentsize[1]) << 8);
	ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[0] = ELF_DecodedFileHeaderInformation->ELF_FileHeaderSize;
	ELF_DecodedFileHeaderInformation->NumberOfConfirmedSectionHeaders = 0;


// Read Section headers
	fprintf(stdout,"Searching for section headers in ELF File ..");
	for (Index = 0; Index < ELF_DecodedFileHeaderInformation->NumberOfSectionHeaders; Index++)
	{
		fseek(ELF_File, (ELF_DecodedFileHeaderInformation->SectionHeaderOffset + (ELF_DecodedFileHeaderInformation->SectionHeaderSize * Index)), SEEK_SET);
		if(fread((char*)&(ElfSectionHeader[Index]), sizeof(char), sizeof(Elf386_External_Shdr), ELF_File) != sizeof(Elf386_External_Shdr))	
		{
		  fprintf(stdout,"XX");
		}
		else
		{
			ELF_DecodedFileHeaderInformation->NumberOfConfirmedSectionHeaders++;
			fprintf(stdout,"..");
		}

	// Finds start of sharc code
		ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[Index + 1] = ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[Index] + ((long) ElfSectionHeader[Index].sh_addr[0]) + (((long) ElfSectionHeader[Index].sh_addr[1]) << 8) + (((long) ElfSectionHeader[Index].sh_addr[2]) << 16) + (((long) ElfSectionHeader[Index].sh_addr[3]) << 24);
		ELF_DecodedFileHeaderInformation->SectionAddressOffset[Index] = ((long) ElfSectionHeader[Index].sh_offset[0]) + (((long) ElfSectionHeader[Index].sh_offset[1]) << 8) + (((long) ElfSectionHeader[Index].sh_offset[2]) << 16) + (((long) ElfSectionHeader[Index].sh_offset[3]) << 24);
		ELF_DecodedFileHeaderInformation->PhysicalAddressSharc[Index] = ((long) ElfSectionHeader[Index].sh_type[0]) + (((long) ElfSectionHeader[Index].sh_type[1]) << 8) + (((long) ElfSectionHeader[Index].sh_type[2]) << 16) + (((long) ElfSectionHeader[Index].sh_type[3]) << 24);
	}
	fprintf(stdout, "\n");
	fprintf(stdout, " ELF section header successfully analyzed. \n");
}


//******************************************************************************
//PatchHexIntoElf : patches 
//******************************************************************************



void PatchHexIntoElf(int arc, char *arv, char *arv_ldr)
{       
	long  Index;
	static long ReadLongValueFromFile;

	FILE *FileInputPtr;	// ELF file generated by SHARC linker
	FILE *LDF_InputPtr;
	FILE *TempFilePtr;

	long AbsoluteAdressToWriteIntoDXE = 0;
	long FilePatched = 0;
	long LastModifiedAddress = 0;

	// New definitions for ETL600
	static Elf386_External_Ehdr ElfFileHeader;
	static Elf386_External_Shdr ElfSectionHeader[400];	// 400 are enough
	ELF_DecodedFileHeaderType ELF_DecodedFileHeaderInformation;
	long SDRAM_IsAvailable = 0;
	
	unsigned long overAllChecksum;

	// Init 
	InitMemorySegmentInformationWithDefaultValues();

	//Open input_file
	if ((FileInputPtr = fopen(arv,"r+b")) == NULL)
	{
		fprintf(stderr,"\aERROR - cannot open ELF file to be written in %s \n", arv);
		PrintHelpText();
		exit(1);
	}

	// open LDF File, get information to calculate CRC
	if ((LDF_InputPtr = fopen(arv_ldr,"rb")) == NULL)
	{
		fprintf(stderr,"\aERROR - cannot open nessessary LDF file %s \n", arv_ldr);
		PrintHelpText();
		exit(1);
	}

	//Read ELF fileheader
	if(fread((char*)&ElfFileHeader, sizeof(char), sizeof(Elf386_External_Ehdr), FileInputPtr) != sizeof(Elf386_External_Ehdr))	
	{
	  fprintf(stderr,"\aERROR - cannot read the ELF file header.\n");
	  exit(1);
	}
	else
	{
		fprintf(stdout, "ELF file header successfully found \n");
	}

	// Decode section headers
	DecodeELF_HeaderInformation(&ElfFileHeader, &ELF_DecodedFileHeaderInformation, FileInputPtr);
  
	// Decode LDF information
	TempFilePtr = LDF_InputPtr;
    if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentInternalRTH_PM, sizeof(SegmentNameLDF_seg_rth) - 1)))
	{
		fprintf(stderr,"\aERROR - cannot find rth information in the LDF file.\n");
		exit(1);
	}

	TempFilePtr = LDF_InputPtr;
	if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentInternalINIT_PM, sizeof(SegmentNameLDF_seg_init) - 1)))
	{
		fprintf(stderr,"\aERROR - cannot find init information in the LDF file.\n");
		exit(1);
	}

	TempFilePtr = LDF_InputPtr;
	if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentInternalPROGRAM_PM, sizeof(SegmentNameLDF_seg_pmco) - 1)))
	{
		fprintf(stderr,"\aERROR - cannot find internal PM information in the LDF file.\n");
		exit(1);
	}

	TempFilePtr = LDF_InputPtr;
	if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentInternalDATA_PM, sizeof(SegmentNameLDF_seg_pmda) - 1)))
	{
		fprintf(stderr,"\aERROR - cannot find internal DM in PM section of the LDF file.\n");
		exit(1);
	}

	TempFilePtr = LDF_InputPtr;
	if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentInternalDATA_DM, sizeof(SegmentNameLDF_seg_dmda) - 1)))
	{
		fprintf(stderr,"\aERROR - cannot find internal DM  of the LDF file.\n");
		exit(1);
	}

	// optional external SDRAM
	TempFilePtr = LDF_InputPtr;
	if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentExternalPROGRAM_PM, sizeof(SegmentNameLDF_ext_code) - 1)))
	{
		fprintf(stdout,"MESSAGE: Can't find external PM in the LDF file. Suggesting no SDRAM available.\n");
	}
	else SDRAM_IsAvailable = 1;

	TempFilePtr = LDF_InputPtr;
	if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentExternalDATA_DM, sizeof(SegmentNameLDF_ext_dmda) - 1)))
	{
		fprintf(stdout,"MESSAGE: Can't find external DM in the LDF file. Suggesting no SDRAM available.\n");
		if (SDRAM_IsAvailable)
		{
			fprintf(stderr,"\aERROR - SDRAM was defined. Something is wrong in the LDF file. Exiting...\n");
			exit(1);
		}
	}
	else SDRAM_IsAvailable = 1;
	
	// For Master DSP only
	if(isItMasterDSP)
	{
		TempFilePtr = LDF_InputPtr;
		if (!(DecodeInformationInLDFforaSegment(TempFilePtr, &SegmentExternalDATA_DM_SlaveCode, sizeof(SegmentNameLDF_ext_dmda_SlaveCode) - 1)))
		{
			fprintf(stdout,"MESSAGE: Can't find external DM for Slave Code in the LDF file. \n");
			if (SDRAM_IsAvailable)
			{
				fprintf(stderr,"\aERROR - SDRAM was defined. Something is wrong in the LDF file. Exiting...\n");
				exit(1);
			}
		}
		else SDRAM_IsAvailable = 1;
	}




	// Copy memory size taken from LDF file
	SegmentInternalRTH_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	SegmentInternalINIT_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	SegmentInternalPROGRAM_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	SegmentInternalDATA_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	SegmentInternalDATA_DM.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	SegmentExternalPROGRAM_PM.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	SegmentExternalDATA_DM.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	// For Master DSP only
	if(isItMasterDSP)
	{
		SegmentExternalDATA_DM_SlaveCode.ELF_MemorySegmentBoundaries.WidthMemorySegment = SegmentExternalDATA_DM_SlaveCode.LDF_MemorySegmentDefinitions.WidthMemorySegment;
	}


	// !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION
	// !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION
	// !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION !!!!!! CAUTION
	// We were experiencing problems with the checksum in the rth segement, if it is booted from the flash or
	// started with the emulator. It has something to to with the __lib_start(). The bootloader is modifying the
	// Adress 0x40005 - so there must be a crc alarm. The SPI bootloader uses even more instructions!
	// We decided to exclude the adress range 0x40000 - 0x4000B from the memory check. This is exclusively done
	// here by starting the crc calculation with an offset and modifying the start adress of the memory segment.
	// no changes has to be made on the target.
	// PATCH

	// Calculate Checksums for every program memory segment
	CalculateChecksumForMemorySegment(FileInputPtr, &ELF_DecodedFileHeaderInformation, &SegmentInternalRTH_PM, 8); // PATCH with offset 8
	// END OF PATCH
	CalculateChecksumForMemorySegment(FileInputPtr, &ELF_DecodedFileHeaderInformation, &SegmentInternalINIT_PM, 0);	// NO OFFSET
	CalculateChecksumForMemorySegment(FileInputPtr, &ELF_DecodedFileHeaderInformation, &SegmentInternalPROGRAM_PM, 0); // NO OFFSET
	if (SDRAM_IsAvailable)
	{
		CalculateChecksumForMemorySegment(FileInputPtr, &ELF_DecodedFileHeaderInformation, &SegmentExternalPROGRAM_PM, 0); // NO OFFSET		
	}
	// For Master DSP only
	if(isItMasterDSP)
	{
		CalculateChecksumForMemorySegment(FileInputPtr, &ELF_DecodedFileHeaderInformation, &SegmentExternalDATA_DM_SlaveCode, 0); // NO OFFSET
	}

	
	
	// Find adress to write checksums and segement boundaries into DXE file
	AbsoluteAdressToWriteIntoDXE = FindAdressToWriteIntoDXE(FileInputPtr, &ELF_DecodedFileHeaderInformation, &SegmentInternalDATA_DM);

	// Check if there was a segment found
	if (AbsoluteAdressToWriteIntoDXE == 0)
	{
		fprintf(stderr,"\aERROR - cannot find segment to write into DXE file.\n");
		exit(1);
	}

	// Everything is ok, now write calculated checksums and segment boundaries into SHARC DXE file

	// Write checksums back to file segment seg_dmda
	// Must be at the beginning of the segment
	FilePatched = 0;
	fseek(FileInputPtr, 0, SEEK_SET);
	for (Index = 0; Index < 200; Index++)
	{
		fseek(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SEEK_SET);
		fread((char*)&(ReadLongValueFromFile), sizeof(char), sizeof(ReadLongValueFromFile), FileInputPtr);
		ReadLongValueFromFile = MapCRC_To_DXE_Format(ReadLongValueFromFile);
		// Check the patterns
		if (ReadLongValueFromFile == CRC_CHECKSUM_PM_RTH_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalRTH_PM.CRC16_ChecksumIfProgramMemory);
			WriteCommentaryOfSucessfulWritePM(SegmentInternalRTH_PM.CRC16_ChecksumIfProgramMemory, (Index + AbsoluteAdressToWriteIntoDXE));
			FilePatched = 1;
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == CRC_CHECKSUM_PM_INIT_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalINIT_PM.CRC16_ChecksumIfProgramMemory);
			WriteCommentaryOfSucessfulWritePM(SegmentInternalINIT_PM.CRC16_ChecksumIfProgramMemory, (Index + AbsoluteAdressToWriteIntoDXE));
			FilePatched = 1;
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == CRC_CHECKSUM_PM_INTERN_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory);
			WriteCommentaryOfSucessfulWritePM(SegmentInternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory, (Index + AbsoluteAdressToWriteIntoDXE));
			FilePatched = 1;
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (SDRAM_IsAvailable)
		{
			if (ReadLongValueFromFile == CRC_CHECKSUM_PM_EXTERN_DEFAULT)
			{
				WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentExternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory);
				WriteCommentaryOfSucessfulWritePM(SegmentExternalPROGRAM_PM.CRC16_ChecksumIfProgramMemory, (Index + AbsoluteAdressToWriteIntoDXE));
				FilePatched = 1;
				LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
			}
		}

		// For Master DSP only
		if(isItMasterDSP)
		{
			if (ReadLongValueFromFile == CRC_CHECKSUM_OVER_ALL_CODE)
			{
				
				overAllChecksum = CalculateOverAllChecksum();

				WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), overAllChecksum);
				
				WriteCommentaryOfSucessfulWritePM(overAllChecksum, (Index + AbsoluteAdressToWriteIntoDXE));
				FilePatched = 1;
				LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
			}
		}


		// Now check for start / stop segments
		if (ReadLongValueFromFile == PM_INTERNAL_SEGMENT_RTH_START_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_START(SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_rth);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == PM_INTERNAL_SEGMENT_RTH_STOP_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_STOP(SegmentInternalRTH_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_rth);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == PM_INTERNAL_SEGMENT_INIT_START_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_START(SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_init);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == PM_INTERNAL_SEGMENT_INIT_STOP_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_STOP(SegmentInternalINIT_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_init);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == PM_INTERNAL_SEGMENT_START_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_START(SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_pmco);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == PM_INTERNAL_SEGMENT_STOP_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_STOP(SegmentInternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_pmco);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == DM_PROGRAM_INTERNAL_START_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_START(SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_pmda);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == DM_PROGRAM_INTERNAL_STOP_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_STOP(SegmentInternalDATA_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_pmda);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}

		if (ReadLongValueFromFile == DM_INTERNAL_START_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE),  SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_START(SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_dmda);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (ReadLongValueFromFile == DM_INTERNAL_STOP_DEFAULT)
		{
			WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment);
			WriteCommentaryOfSucessfulWriteDM_STOP(SegmentInternalDATA_DM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_seg_dmda);
			LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
		}
		if (SDRAM_IsAvailable)
		{
			if (ReadLongValueFromFile == PM_EXTERNAL_SEGMENT_START_DEFAULT)
			{
				WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment);
				WriteCommentaryOfSucessfulWriteDM_START(SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_ext_code);
				LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
			}
			if (ReadLongValueFromFile == PM_EXTERNAL_SEGMENT_STOP_DEFAULT)
			{
				WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment);
				WriteCommentaryOfSucessfulWriteDM_STOP(SegmentExternalPROGRAM_PM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_ext_code);
				LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
			}
			if (ReadLongValueFromFile == DM_EXTERNAL_START_DEFAULT)
			{
				WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment);
				WriteCommentaryOfSucessfulWriteDM_START(SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.StartAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_ext_dmda);
				LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
			}
			if (ReadLongValueFromFile == DM_EXTERNAL_STOP_DEFAULT)
			{
				WriteLongValueIntoDXE_File(FileInputPtr, (Index + AbsoluteAdressToWriteIntoDXE), SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment);
				WriteCommentaryOfSucessfulWriteDM_STOP(SegmentExternalDATA_DM.LDF_MemorySegmentDefinitions.StopAdressMemorySegment, (Index + AbsoluteAdressToWriteIntoDXE), SegmentNameLDF_ext_dmda);
				LastModifiedAddress = (Index + AbsoluteAdressToWriteIntoDXE);
			}
		}
	}
	
	if (FilePatched == 0)
	{ 
		fprintf(stderr,"\aERROR - cannot find the start pattern in array.\n");
		fprintf(stderr,"\aERROR - Did you already patched this file?\n");
		fprintf(stderr,"\aERROR - exit application ...\n");
    	exit(1);
	}
	else
	{
		fprintf(stdout,"\n");
		fprintf(stdout, "Last overwritten address location: ");
		fprintf(stdout,"%x\n", LastModifiedAddress);
		fprintf(stdout,"\n");
		fprintf(stdout,"\n");
		
		// For Master DSP only
		if(isItMasterDSP)
		{
			fprintf(stdout, "**************************************************\n");
			fprintf(stdout, "***  Checksum for Code Authentication: 0x");
			fprintf(stdout,"%x  ***\n", overAllChecksum);
			fprintf(stdout, "**************************************************\n");
			fprintf(stdout,"\n");
		}
		fprintf(stdout,"\n File is patched successfully \n");
	}
	fclose (FileInputPtr);
}


//******************************************************************************
//MAIN PROGRAM
//******************************************************************************

void main(int argc, char *argv[])
{

    static char opt;    // readed char, option char
    static int i, r=0; // flags for options

	isItMasterDSP = myFALSE;

	if (argc < 3)           /* Enough arguments? */
     {
       puts("Usage:  Not enough arguments \n");
       exit(1);
     }
	
	if ((argv[1])[0] == '-')
	{
		opt = (argv[1])[1];
		switch (opt)
		{
			case 'm':// additional CRC calculation for Master DSP
				r = 2;
				break;
		
			case 's':// all slave DSPs
				r = 1;					
				break;
					
			default:
				PrintHelpText();
				exit(1);
				break;
		}	
	}
	
	if (r == 2)	// additional CRC calculation for Master DSP
	{
		isItMasterDSP = myTRUE;
		PatchHexIntoElf(argc, argv[2], argv[3]);
		exit(0);
	}
	
	if (r == 1) // handle all slave DSPs
	{

		PatchHexIntoElf(argc, argv[2], argv[3]);
		exit(0);
	}
	
	if (r == 0)   //no options
	{
		fprintf(stderr,"\aERROR - No options choosen: what do you want to do ? \n");
		PrintHelpText();
		exit(1);
	}

}   
