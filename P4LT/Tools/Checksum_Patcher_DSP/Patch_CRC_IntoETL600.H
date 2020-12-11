/*-------------------------------------------------------------------------*
 * $Workfile: Patch_CRC_IntoETL600.H $
 * Part of      : ETL600
 * Language     : C
 * Created by   : Bruno Busslinger, PTUKT2
 * Remarks      : 
 * Purpose      : 
 * Copyright    : ABB Switzerland Ltd., 2004
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 $Log: /Source/P4LT Prototype 2/Tools/Checksum_Patcher_DSP/Patch_CRC_IntoETL600.H $
 * 
 * 1     9.05.05 16:25 Chbrbus
 * 
 * 
 *-------------------------------------------------------------------------*/

#ifndef PATCH_CRC_INTO_ETL600
#define PATCH_CRC_INTO_ETL600


typedef struct {
	unsigned char DataTypeFlagMSB_Zero;
	unsigned char DataTypeFlag;
	unsigned char Zero1;
	unsigned char Zero2;
	unsigned char Zero3;
	unsigned char Zero4;
	unsigned char LengthMSB;
	unsigned char LengthLSB;
	unsigned char AddressLSB_MSB;
	unsigned char AddressLSB_LSB;
	unsigned char AddressMSB_MSB;
	unsigned char AddressMSB_LSB;
} ETL600_BootFileHeaderType;


typedef struct {
	long NumberOfSectionHeaders;
	long NumberOfConfirmedSectionHeaders;
	long ELF_FileHeaderSize;
	long SectionHeaderOffset;
	long SectionHeaderSize;
	long SectionAddressOffset[400];
	long SectionStartAddressAbsolute[400];	// 400 are enough
	long PhysicalAddressSharc[400];
} ELF_DecodedFileHeaderType;

typedef struct {
	long StartAdressMemorySegment;
	long StopAdressMemorySegment;
	long WidthMemorySegment;
} SegmentMemoryInformationType;

typedef struct {
	unsigned long CRC16_ChecksumIfProgramMemory;
	SegmentMemoryInformationType LDF_MemorySegmentDefinitions;
	SegmentMemoryInformationType ELF_MemorySegmentBoundaries;
	char *SegmentNameInLDF;
	long StopAdressMemorySegmentLinkedVersion;
} SHARC_MemorySegmentInformationType;


#endif

