/*-------------------------------------------------------------------------*
 * $Workfile	: Patch_CRC_Into_Loaderfile.c $
 * Part of		: ETL600 / O4CV
 * Language		: C
 * Created by	: Allen Notter (AN)
 * Remarks		:  
 * Purpose		: Inserting CRC Checksum into O4CV Loaderfile
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/Tools/ChecksumPatcher/Patch_CRC_Into_Loaderfile.C $ 
 * 
 * 1     07-08-30 10:39 Chalnot
 * Initial version.
 * 
 *-------------------------------------------------------------------------*/
 
/* INCLUDE FILES *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CRC16_table.h"
#include "Elf32_Definitions.h"
#include "ConvertingFunctions.h"
#include "Patch_CRC_Into_Loaderfile.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define L1_CODE_STARTADDR		0xffa00000
#define L1_CODE_STOPADDR		0xffa10000	

#define L1_DATA_CONST_STARTADDR	0xff902000
#define L1_DATA_CONST_STOPADDR	0xff904000	

#define SDRAM0_BANK0_STARTADDR	0x4	
#define SDRAM0_BANK0_STOPADDR	0x400000

#define CRC_ADDRESS_IN_FLASH	0x2f0000	

#define LINE_LEN  1024

/* LOCAL TYPE DECLARATIONS ***************************************************/

typedef struct {
	unsigned long NumberOfSectionHeaders;
	unsigned long NumberOfConfirmedSectionHeaders;
	unsigned long ELF_FileHeaderSize;
	unsigned long SectionHeaderOffset;
	unsigned long SectionHeaderSize;
	unsigned long SectionAddressOffset[400];
	unsigned long SectionStartAddressAbsolute[400];	
	unsigned long PhysicalAddressSharc[400];
} ELF_DecodedFileHeaderType;

typedef struct {
	unsigned long CRC16_ChecksumIfProgramMemory;
	unsigned long StartAddressMemorySegment;
	unsigned long StopAddressMemorySegment;
	unsigned long WidthMemorySegment;
} MemorySectionInformationType;

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

static MemorySectionInformationType g_section_L1_Code;
static MemorySectionInformationType g_section_L1_Data_Const;
static MemorySectionInformationType g_Section_Sdram0_Bank0;

/* LOCAL FUNCTIONS ***********************************************************/

static void initMemorySegmentInformationWithDefaultValues(void);
static unsigned short convertingFunctions_crc16(unsigned short reg, unsigned char message);
static unsigned long calculateOverAllChecksum(void);
static void calculateChecksumForMemorySegment(FILE *DXE_InputPtr, 
									   ELF_DecodedFileHeaderType *ELF_DecodedFileHeaderInformation, 
									   MemorySectionInformationType *MemorySectionInformation);
static void decodeELF_HeaderInformation(Elf386_External_Ehdr *ElfFileHeader, 
								 ELF_DecodedFileHeaderType *ELF_DecodedFileHeaderInformation, 
								 FILE *ELF_File);
static void addChecksumToString(char* pLine, unsigned int length);
static void printHelpText(void);
static void patchCRCIntoLdr(int arc, char *arv_dxe, char *arv_ldr);

/* IMPLEMENTATION ************************************************************/  

/***************************************************************************
  Function:		initMemorySegmentInformationWithDefaultValues()		
  Description:	Initializes the section structures
  Inputs:		None
  Outputs:		None	
  Return:		None		
  Globals:		g_section_L1_Code, g_section_L1_Data_Const, 
				g_Section_Sdram0_Bank0  		
  Important:	
****************************************************************************/
void initMemorySegmentInformationWithDefaultValues(void)
{

	// Init L1_Code struct
	g_section_L1_Code.CRC16_ChecksumIfProgramMemory = 0;
	g_section_L1_Code.StartAddressMemorySegment = L1_CODE_STARTADDR;
	g_section_L1_Code.StopAddressMemorySegment = L1_CODE_STOPADDR;
	g_section_L1_Code.WidthMemorySegment = 8;

	// Init L1_Data_Const struct
	g_section_L1_Data_Const.CRC16_ChecksumIfProgramMemory = 0;
	g_section_L1_Data_Const.StartAddressMemorySegment = L1_DATA_CONST_STARTADDR;
	g_section_L1_Data_Const.StopAddressMemorySegment = L1_DATA_CONST_STOPADDR;
	g_section_L1_Data_Const.WidthMemorySegment = 8;

	// Init Sdram0_Bank0 struct
	g_Section_Sdram0_Bank0.CRC16_ChecksumIfProgramMemory = 0;
	g_Section_Sdram0_Bank0.StartAddressMemorySegment = SDRAM0_BANK0_STARTADDR;
	g_Section_Sdram0_Bank0.StopAddressMemorySegment = SDRAM0_BANK0_STOPADDR;
	g_Section_Sdram0_Bank0.WidthMemorySegment = 8;

}


/***************************************************************************
  Function:		convertingFunctions_crc16()		
  Description:	Calculates the crc16
  Inputs:		reg: The old crc
				message: The byte to calculate the new crc with
  Outputs:		None	
  Return:		The new crc		
  Globals:		None		
  Important:	
****************************************************************************/
unsigned short convertingFunctions_crc16(unsigned short reg, unsigned char message)
{
	unsigned short reg_temp;
	
	reg_temp = ((reg >> 8)& 0x00ff) ^ crc16_table[message ^ (reg & 0x00ff)];   
	
	return reg_temp;
}


/***************************************************************************
  Function:		calculateOverAllChecksum()	
  Description:	Calculates the crc over all sections
  Inputs:		None
  Outputs:		None	
  Return:		Overall crc		
  Globals:		g_section_L1_Code, g_section_L1_Data_Const, 
				g_Section_Sdram0_Bank0  		
  Important:	
****************************************************************************/
unsigned long calculateOverAllChecksum(void)
{
	unsigned short CRC_TempSum = 0;
	unsigned short count = 0; 
	unsigned short index = 0;
	unsigned long checksums[10];

	// Get crcs from every section and put them in an array
	checksums[count++] = g_section_L1_Code.CRC16_ChecksumIfProgramMemory;
	checksums[count++] = g_section_L1_Data_Const.CRC16_ChecksumIfProgramMemory;	
	checksums[count++] = g_Section_Sdram0_Bank0.CRC16_ChecksumIfProgramMemory;	

	// Calculate overall crc with the crcs in the array
	for(index = 0;index < count;index++)
	{
		CRC_TempSum = convertingFunctions_crc16(CRC_TempSum, ((unsigned char) checksums[index]));
	}

	return (CRC_TempSum);
}


/***************************************************************************
  Function:		calculateChecksumForMemorySegment()	
  Description:	Calculates the crc over one section
  Inputs:		DXE_InputPtr: Pointer to dxe file
				ELF_DecodedFileHeaderInformation: Pointer to decoded file 
				header structure
				MemorySectionInformation: Pointer to section information 
				structure
  Outputs:		MemorySectionInformation: Pointer to section information 
				structure	
  Return:		None	
  Globals:		None		
  Important:	
****************************************************************************/
void calculateChecksumForMemorySegment(FILE *DXE_InputPtr, ELF_DecodedFileHeaderType *ELF_DecodedFileHeaderInformation, MemorySectionInformationType *MemorySectionInformation)
{
	unsigned long Index = 0;
	unsigned long LengthOfSection = 0;
	unsigned short CRC_TempSum = 0;
	unsigned short TempValueForCRC;
	unsigned long CounterFilePosition = 0;
	unsigned long OffsetInDXE_file = 0;

	// Go to start of the dxe file
	fseek(DXE_InputPtr, 0, SEEK_SET);
	
	// Set start value of the CRC calculation			
	MemorySectionInformation->CRC16_ChecksumIfProgramMemory = 0;
	
	// Find the correct section  
	for (Index = 0; Index < ELF_DecodedFileHeaderInformation->NumberOfConfirmedSectionHeaders; Index++)
	{								   									   
		if (ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[Index] == MemorySectionInformation->StartAddressMemorySegment)
		{

			LengthOfSection = 0x0000;
			OffsetInDXE_file = ELF_DecodedFileHeaderInformation->SectionAddressOffset[Index];
			
			// Read byte after byte and calculate the crc
			for (CounterFilePosition = OffsetInDXE_file; CounterFilePosition < (ELF_DecodedFileHeaderInformation->SectionAddressOffset[Index + 1]); CounterFilePosition++)
			{
				fseek(DXE_InputPtr, CounterFilePosition, SEEK_SET);
				fread((char*)&TempValueForCRC, sizeof(char), sizeof(char), DXE_InputPtr);
				CRC_TempSum = convertingFunctions_crc16(CRC_TempSum, (unsigned char) TempValueForCRC);
				LengthOfSection++;
			}

			// Calculate the stop address and save it in the MemorySectionInformation
			LengthOfSection = LengthOfSection / (MemorySectionInformation->WidthMemorySegment / 8);
			if (MemorySectionInformation->StopAddressMemorySegment >= (MemorySectionInformation->StartAddressMemorySegment + LengthOfSection))
			{
				MemorySectionInformation->StopAddressMemorySegment = MemorySectionInformation->StartAddressMemorySegment + LengthOfSection;
			}

		}

	}

	// Save the crc in the MemorySectionInformation
	MemorySectionInformation->CRC16_ChecksumIfProgramMemory = CRC_TempSum;

}


/***************************************************************************
  Function:		decodeELF_HeaderInformation()	
  Description:	Decodes the elf and the section headers of the dxe file
  Inputs:		DXE_InputPtr: Pointer to dxe file
				ElfFileHeader: Pointer to the elf header
  Outputs:		ELF_DecodedFileHeaderInformation: Pointer to decoded section 
				header structures
  Return:		None	
  Globals:		None		
  Important:	
****************************************************************************/
void decodeELF_HeaderInformation(Elf386_External_Ehdr *ElfFileHeader, ELF_DecodedFileHeaderType *ELF_DecodedFileHeaderInformation, FILE *ELF_File)
{
	unsigned long Index;
	static Elf386_External_Shdr ElfSectionHeader[400];	

	// Decode nessessary file header information
	ELF_DecodedFileHeaderInformation->NumberOfSectionHeaders = ((long) ElfFileHeader->e_shnum[0]) + (((long) ElfFileHeader->e_shnum[1]) << 8);
	ELF_DecodedFileHeaderInformation->ELF_FileHeaderSize = ((long) ElfFileHeader->e_ehsize[0]) + (((long) ElfFileHeader->e_ehsize[1]) << 8);
	ELF_DecodedFileHeaderInformation->SectionHeaderOffset = ((long) ElfFileHeader->e_shoff[0]) + (((long) ElfFileHeader->e_shoff[1]) << 8) + (((long) ElfFileHeader->e_shoff[2]) << 16) + (((long) ElfFileHeader->e_shoff[3]) << 24);
	ELF_DecodedFileHeaderInformation->SectionHeaderSize = ((long) ElfFileHeader->e_shentsize[0]) + (((long) ElfFileHeader->e_shentsize[1]) << 8);
	ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[0] = ELF_DecodedFileHeaderInformation->ELF_FileHeaderSize;
	ELF_DecodedFileHeaderInformation->NumberOfConfirmedSectionHeaders = 0;

	// Read Section headers in the elf file
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

		// Save the section start address to the ELF_DecodedFileHeaderInformation
		ELF_DecodedFileHeaderInformation->SectionStartAddressAbsolute[Index] = ((long) ElfSectionHeader[Index].sh_addr[0]) + (((long) ElfSectionHeader[Index].sh_addr[1]) << 8) + (((long) ElfSectionHeader[Index].sh_addr[2]) << 16) + (((long) ElfSectionHeader[Index].sh_addr[3]) << 24);
		
		// Save the section offset in the dxe file to the ELF_DecodedFileHeaderInformation
		ELF_DecodedFileHeaderInformation->SectionAddressOffset[Index] = ((long) ElfSectionHeader[Index].sh_offset[0]) + (((long) ElfSectionHeader[Index].sh_offset[1]) << 8) + (((long) ElfSectionHeader[Index].sh_offset[2]) << 16) + (((long) ElfSectionHeader[Index].sh_offset[3]) << 24);
	}

	fprintf(stdout, "\n");
	fprintf(stdout, "ELF section header successfully analyzed. \n");
}


/***************************************************************************
  Function:		addChecksumToString()	
  Description:	Calculates the checksum of one line in the ldr file and
				adds it at the end of the line
  Inputs:		pLine: Pointer to line
				length: Number of bytes in the line
  Outputs:		pLine: Pointer to line
  Return:		None	
  Globals:		None		
  Important:	
****************************************************************************/
void addChecksumToString(char* pLine, unsigned int length)
{
	unsigned char byteValue = 0;
	unsigned long Index;
	unsigned char cc[2];

	// Calculate checksum from the line
	for(Index = 1; Index < length; Index+=2)
	{
		byteValue += convertingFunctions_convertASCIIintoBinary(pLine[Index])<<4 | convertingFunctions_convertASCIIintoBinary(pLine[Index+1]);	
	}
	byteValue = ~byteValue + 1;

	// Convert checksum into ASCII 
	cc[0] = convertingFunctions_convertBinaryintoASCII((char) (byteValue >> 4 & 0xf));
	cc[1] = convertingFunctions_convertBinaryintoASCII((char) (byteValue & 0xf));

	// Concate the checksum to the line
	strncat(pLine, &cc[0], 2);

}


/***************************************************************************
  Function:		printHelpText()		
  Description:	Prints the help text on the console
  Inputs:		None
  Outputs:		None	
  Return:		None	
  Globals:		None		
  Important:	
****************************************************************************/
void printHelpText(void)
{
	fprintf(stdout,"ETL600 O4CV CRC calculation program for *.ldr file\n");	
	fprintf(stdout,"USAGE: ChecksumPatcher.exe <input_file.dxe> <input_file.ldr>\n");				
}


/***************************************************************************
  Function:		patchCRCIntoLdr()	
  Description:	Creates a loaderfile consisting of the content of the old 
				loaderfile and of some additional information needed by
				the memory check
  Inputs:		arc: Number of arguments
				arv_dxe: Pointer to dxe file name string
				arv_ldr: Pointer to ldr file name string
  Outputs:		None
  Return:		None	
  Globals:		None		
  Important:	
****************************************************************************/
void patchCRCIntoLdr(int arc, char *arv_dxe, char *arv_ldr)
{       
	
	FILE *DXE_FilePtr;	
	FILE *LDR_InputFilePtr;
	FILE *LDR_OutputFilePtr;

	char* outFile = "applic_crc.ldr";

	char  line_1[LINE_LEN], *result ;

	static Elf386_External_Ehdr ElfFileHeader;
	static Elf386_External_Shdr ElfSectionHeader[400];	
	ELF_DecodedFileHeaderType ELF_DecodedFileHeaderInformation;
	
	unsigned long overAllChecksum;

	// Init 
	initMemorySegmentInformationWithDefaultValues();

	// Open DXE file
	if ((DXE_FilePtr = fopen(arv_dxe,"rb")) == NULL)
	{
		fprintf(stderr,"\aERROR - Cannot open DXE file %s \n\n", arv_dxe);
		printHelpText();
		exit(1);
	}

	// Open LDR File
	if ((LDR_InputFilePtr = fopen(arv_ldr,"rb")) == NULL)
	{
		fprintf(stderr,"\aERROR - Cannot open nessessary LDR file %s \n\n", arv_ldr);
		printHelpText();
		exit(1);
	}

	// Create LDR File
	if ((LDR_OutputFilePtr = fopen(outFile,"w+b")) == NULL)
	{
		fprintf(stderr,"\aERROR - Cannot create nessessary LDR file %s \n\n", outFile);
		printHelpText();
		exit(1);
	}

	// Read ELF fileheader
	if(fread((char*)&ElfFileHeader, sizeof(char), sizeof(Elf386_External_Ehdr), DXE_FilePtr) != sizeof(Elf386_External_Ehdr))	
	{
	  fprintf(stderr,"\aERROR - Cannot read the ELF file header.\n");
	  exit(1);
	}
	else
	{
		fprintf(stdout, "ELF file header successfully found \n");
	}

	// Decode section headers
	decodeELF_HeaderInformation(&ElfFileHeader, &ELF_DecodedFileHeaderInformation, DXE_FilePtr);

	// Calculate crc for the 2 program memory and the const data section 
	calculateChecksumForMemorySegment(DXE_FilePtr, &ELF_DecodedFileHeaderInformation, &g_section_L1_Code); 
	calculateChecksumForMemorySegment(DXE_FilePtr, &ELF_DecodedFileHeaderInformation, &g_section_L1_Data_Const);
	calculateChecksumForMemorySegment(DXE_FilePtr, &ELF_DecodedFileHeaderInformation, &g_Section_Sdram0_Bank0); 

	// Calculate overall crc
	overAllChecksum = calculateOverAllChecksum();

	// Print patch data on the console
	fprintf(stdout, "L1_Code Start: %x \n", g_section_L1_Code.StartAddressMemorySegment);
	fprintf(stdout, "L1_Code Stop: %x \n", g_section_L1_Code.StopAddressMemorySegment);
	fprintf(stdout, "L1_Data_Const Start: %x \n", g_section_L1_Data_Const.StartAddressMemorySegment);
	fprintf(stdout, "L1_Data_Const Stop: %x \n", g_section_L1_Data_Const.StopAddressMemorySegment);
	fprintf(stdout, "Sdram0_Bank0 Start: %x \n", g_Section_Sdram0_Bank0.StartAddressMemorySegment);
	fprintf(stdout, "Sdram0_Bank0 Stop: %x \n", g_Section_Sdram0_Bank0.StopAddressMemorySegment);	

	// Read ldr file line by line and write each line except the end of file line to the output ldr file
	result = fgets(line_1, LINE_LEN, LDR_InputFilePtr);
	if (result == NULL){
		printf("Error, file %s is empty\n", arv_ldr);
		exit(0);
	}
	do {
		fputs  (line_1, LDR_OutputFilePtr);
		result = fgets(line_1, LINE_LEN, LDR_InputFilePtr);
	} while (!(result[7] == '0' && result[8] == '1') && result != NULL);


	// Write high address to ldr file 	
#ifdef _DEBUG
	strcpy(line_1, ":02000004002F");	// Debug Version. Absolute address in the flash! Use when the hex loaderfile is loaded on the flash.
#else
	strcpy(line_1, ":02000004000F");	// Release Version. Only the offset in the bank not the address in the flash! Use when the binary loaderfile is loaded on the flash.
#endif
	addChecksumToString(&line_1[0], strlen(line_1));
	fputs  (line_1, LDR_OutputFilePtr);
	fputs  ("\n", LDR_OutputFilePtr);

	// Write overall crc to ldr file
	strcpy(line_1, ":04000000");
	strncat(line_1, convertingFunctions_convertAddressInLongintoASCII(convertingFunctions_mapToLDRFormat(overAllChecksum & 0x0000ffff)),8);
	addChecksumToString(&line_1[0], strlen(line_1));
	fputs  (line_1, LDR_OutputFilePtr);
	fputs  ("\n", LDR_OutputFilePtr);

	// Write start/stop address from L1_Code to ldr file
	strcpy(line_1, ":08000400");
	strcat(line_1, convertingFunctions_convertAddressInLongintoASCII(convertingFunctions_mapToLDRFormat(g_section_L1_Code.StartAddressMemorySegment)));
	strcat(line_1, convertingFunctions_convertAddressInLongintoASCII(convertingFunctions_mapToLDRFormat(g_section_L1_Code.StopAddressMemorySegment)));
	addChecksumToString(&line_1[0], strlen(line_1));
	fputs  (line_1, LDR_OutputFilePtr);
	fputs  ("\n", LDR_OutputFilePtr);

	// Write start/stop address from L1_Data_Const to ldr file
	strcpy(line_1, ":08000C00");
	strcat(line_1, convertingFunctions_convertAddressInLongintoASCII(convertingFunctions_mapToLDRFormat(g_section_L1_Data_Const.StartAddressMemorySegment)));
	strcat(line_1, convertingFunctions_convertAddressInLongintoASCII(convertingFunctions_mapToLDRFormat(g_section_L1_Data_Const.StopAddressMemorySegment)));
	addChecksumToString(&line_1[0], strlen(line_1));
	fputs  (line_1, LDR_OutputFilePtr);
	fputs  ("\n", LDR_OutputFilePtr);

	// Write start/stop address from Sdram0_Bank0 to ldr file
	strcpy(line_1, ":08001400");
	strcat(line_1, convertingFunctions_convertAddressInLongintoASCII(convertingFunctions_mapToLDRFormat(g_Section_Sdram0_Bank0.StartAddressMemorySegment)));
	strcat(line_1, convertingFunctions_convertAddressInLongintoASCII(convertingFunctions_mapToLDRFormat(g_Section_Sdram0_Bank0.StopAddressMemorySegment)));
	addChecksumToString(&line_1[0], strlen(line_1));
	fputs  (line_1, LDR_OutputFilePtr);
	fputs  ("\n", LDR_OutputFilePtr);

	// Write end of file to ldr file
	strncpy(line_1, ":00000001FF",LINE_LEN);
	fputs  (line_1, LDR_OutputFilePtr);
	fputs  ("\n", LDR_OutputFilePtr);

	// Write overall crc on the console
	fprintf(stdout, "**************************************************\n");
	fprintf(stdout, "***  Checksum for Code Authentication: 0x");
	fprintf(stdout,"%x  ***\n", overAllChecksum);
	fprintf(stdout, "**************************************************\n");
	fprintf(stdout,"\n");
	
	// Close all files
	fclose (DXE_FilePtr);
	fclose (LDR_InputFilePtr);
	fclose (LDR_OutputFilePtr);

}


/***************************************************************************
  Function:		main()	
  Description:	Calls the function patchCRCIntoLdr if the arguments are
				valid
  Inputs:		arc: Number of arguments
				arv_dxe: Pointer to dxe file name string
				arv_ldr: Pointer to ldr file name string
  Outputs:		None
  Return:		None	
  Globals:		None		
  Important:	
****************************************************************************/
void main(int argc, char *argv[])
{

	// Enough arguments? 
	if (argc < 3)           
    {
		fprintf(stderr,"\nERROR - Not enough arguments \n\n");
		printHelpText();
		exit(1);
    }

	patchCRCIntoLdr(argc, argv[1], argv[2]);
	exit(0);

}   
