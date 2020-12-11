/*-------------------------------------------------------------------------*
 * $Workfile: ConvertingFunctions.C $
 * Part of	:	ETL600
 * Language	:	C
 * Created by:	Bruno Busslinger PTUKT2
 * Remarks	:   
 * Purpose	:	Inserting CRC Checksums into firmware on ETL600
 *-------------------------------------------------------------------------*/
                                 
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Tools/Checksum_Patcher_DSP/ConvertingFunctions.C $ 
 * 
 * 3     19.01.06 16:35 Chmibuh
 * 
 * 2     18.05.05 10:07 Chbrbus
 * excluded first 8 adresses in segment seg_rth (bootloader is changing
 * the adress 0x40005) 
 * 
 * 1     9.05.05 16:23 Chbrbus
 *  
 * 
 *-------------------------------------------------------------------------*
 */      

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "Patch_CRC_IntoETL600.h"
#include "Elf32_DefinitionsForSharcCRC.h"
#include "MagicNumbersForMemoryCheck.h"
//#include "ConvertingFunctions.h"

extern const unsigned short crc16_table[];

//******************************************************************************
//CRC16 : CRC-16 computation algorithm
//******************************************************************************

unsigned short crc16(unsigned short reg, unsigned char message)
{
	static unsigned short reg_temp;
	
	reg_temp = (reg >> 8) ^ crc16_table[message ^ (reg & 0x00ff)];    
	
	return reg_temp;
}

void PrintHelpText(void)
{
	fprintf(stdout,"ETL600 DSP CRC calculation program for SHARC *.dxe files\n");
	fprintf(stderr,"\nERROR - Illegal option -\n");
	fprintf(stdout,"USAGE: CRC_ChecksumPatcherDSP.exe -p <input_file.dxe> <architecture file.ldf>\n");				
}

long DecodeTagNumberInLDR(unsigned char Tag)
{
	if (Tag == 0x01) return 0;
	if (Tag == 0x02) return 0;
	if (Tag == 0x03) return 0;
	if (Tag == 0x04) return 2;
	if (Tag == 0x05) return 6;
	if (Tag == 0x06) return 5;
	if (Tag == 0x07) return 0;
	if (Tag == 0x08) return 0;
	if (Tag == 0x09) return 0;
	if (Tag == 0x0A) return 0;
	if (Tag == 0x0B) return 2;
	if (Tag == 0x0C) return 4;
	if (Tag == 0x0D) return 5;
	if (Tag == 0x0E) return 6;
	else return 0;
}

long StringArrayCompare(char *CharInArray1, char *CharInArray2, long LengthToCompare)
{
	long LocalIndex;

	for (LocalIndex = 0; LocalIndex < LengthToCompare; LocalIndex++)
	{
		if (CharInArray1[LocalIndex] != CharInArray2[LocalIndex]) return 0;

	}
	return 1;
}


char convertASCIIintoBinary(char ASCIIcharacter)
{
	char ReturnValue = 0x00;

	switch(ASCIIcharacter)
	{

	case '0':
		ReturnValue = 0x00;
		break;

	case '1':
		ReturnValue = 0x01;
		break;

	case '2':
		ReturnValue = 0x02;
		break;

	case '3':
		ReturnValue = 0x03;
		break;

	case '4':
		ReturnValue = 0x04;
		break;

	case '5':
		ReturnValue = 0x05;
		break;

	case '6':
		ReturnValue = 0x06;
		break;

	case '7':
		ReturnValue = 0x07;
		break;

	case '8':
		ReturnValue = 0x08;
		break;

	case '9':
		ReturnValue = 0x09;
		break;

	case 'a':
	case 'A':
		ReturnValue = 0x0a;
		break;

	case 'b':
	case 'B':
		ReturnValue = 0x0b;
		break;

	case 'c':
	case 'C':
		ReturnValue = 0x0c;
		break;

	case 'd':
	case 'D':
		ReturnValue = 0x0d;
		break;

	case 'e':
	case 'E':
		ReturnValue = 0x0e;
		break;

	case 'f':
	case 'F':
		ReturnValue = 0x0f;
		break;
		
	default:
		ReturnValue = 0x00;
		break;
	}


	return ReturnValue;
}

long ConvertAddressInASCIIintoLong(char *CharArr)
{
	char MagicWordHex[] = "0x";
	unsigned long Value = 0;

	if (CharArr[0] == MagicWordHex[0])
	{
		if (CharArr[1] == MagicWordHex[1])
		{
			Value = ((unsigned long)convertASCIIintoBinary(CharArr[9]));
			Value = Value | (((unsigned long)convertASCIIintoBinary(CharArr[8])) << 4);
			Value = Value | (((unsigned long)convertASCIIintoBinary(CharArr[7])) << 8);
			Value = Value | (((unsigned long)convertASCIIintoBinary(CharArr[6])) << 12);
			Value = Value | (((unsigned long)convertASCIIintoBinary(CharArr[5])) << 16);
			Value = Value | (((unsigned long)convertASCIIintoBinary(CharArr[4])) << 20);
			Value = Value | (((unsigned long)convertASCIIintoBinary(CharArr[3])) << 24);
			Value = Value | (((unsigned long)convertASCIIintoBinary(CharArr[2])) << 28);
		}
	}
	else		// It is Width and a decimal value
	{
		Value = 10 * ((unsigned long)convertASCIIintoBinary(CharArr[0]));
		Value = Value + ((unsigned long)convertASCIIintoBinary(CharArr[1]));
	}
	return Value;
}

long MapCRC_To_DXE_Format(long Value)
{
	long TempValue = 0;

	TempValue = ((Value << 24) & 0xff000000) | ((Value << 8) & 0x00ff0000) | ((Value >> 8) & 0x0000ff00) | ((Value >> 24) & 0x000000ff);
	return TempValue;
}

void WriteLongValueIntoDXE_File(FILE *DXE_File, long AdressOffset, long ValueToWrite)
{
	long TempValueDXE_Order = 0;

	fseek(DXE_File, AdressOffset, SEEK_SET);
	TempValueDXE_Order = MapCRC_To_DXE_Format(ValueToWrite);
	fwrite((char*)&(TempValueDXE_Order), sizeof(char), sizeof(TempValueDXE_Order), DXE_File);

}

void WriteCommentaryOfSucessfulWritePM(long WordWritten, long AdressOverwritten)
{
	fprintf(stdout, "SHARC dxe file overwritten adress 0x");
	fprintf(stdout,"%x.", AdressOverwritten);
	fprintf(stdout, ". CRC16 Internal Program Code written: 0x");
	fprintf(stdout,"%x\n", WordWritten);
}

void WriteCommentaryOfSucessfulWriteDM_START(long WordWritten, long AdressOverwritten, char MemorySegment[])
{
	fprintf(stdout, "SHARC dxe file overwritten adress 0x");
	fprintf(stdout,"%x", AdressOverwritten);
	fprintf(stdout, ". Start adress of memory segment ");
	fprintf(stdout,MemorySegment);
	fprintf(stdout, ": 0x");
	fprintf(stdout,"%x\n", WordWritten);
}

void WriteCommentaryOfSucessfulWriteDM_STOP(long WordWritten, long AdressOverwritten, char MemorySegment[])
{
	fprintf(stdout, "SHARC dxe file overwritten adress 0x");
	fprintf(stdout,"%x", AdressOverwritten);
	fprintf(stdout, ". End adress of memory segment ");
	fprintf(stdout,MemorySegment);
	fprintf(stdout, ": 0x");
	fprintf(stdout,"%x\n", WordWritten);
}


