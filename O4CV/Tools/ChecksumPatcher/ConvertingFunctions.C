/*-------------------------------------------------------------------------*
 * $Workfile	: ConvertingFunctions.c $
 * Part of		: ETL600 / O4CV
 * Language		: C
 * Created by	: Allen Notter (AN)
 * Remarks		:  
 * Purpose		: Format converting
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/Tools/ChecksumPatcher/ConvertingFunctions.C $ 
 * 
 * 1     07-08-30 10:39 Chalnot
 * Initial version.
 * 
 * 
 *-------------------------------------------------------------------------*/      

/* INCLUDE FILES *************************************************************/

#include <stdio.h>
#include "Patch_CRC_Into_Loaderfile.h"
#include "Elf32_Definitions.h"
#include "ConvertingFunctions.h"

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/  

/***************************************************************************
  Function:		convertingFunctions_areStringsEqual()		
  Description:	Compares 2 strings and returns 1 if they are equal
  Inputs:		CharInArray1: Pointer to first string
				CharInArray2: Pointer to second string
				LengthToCompare: Number of chars to compare
  Outputs:		None	
  Return:		1 if strings are equal	
  Globals:		None		
  Important:	
****************************************************************************/
long convertingFunctions_areStringsEqual(char *CharInArray1, char *CharInArray2, long LengthToCompare)
{
	long LocalIndex;

	for (LocalIndex = 0; LocalIndex < LengthToCompare; LocalIndex++)
	{
		if (CharInArray1[LocalIndex] != CharInArray2[LocalIndex]) return 0;

	}
	return 1;
}

/***************************************************************************
  Function:		convertingFunctions_convertBinaryintoASCII()		
  Description:	Converts a binary value into an ascii value
  Inputs:		binaryValue: One byte binary value
  Outputs:		None	
  Return:		The ascii value	
  Globals:		None		
  Important:	
****************************************************************************/
char convertingFunctions_convertBinaryintoASCII(char binaryValue)
{
	char ReturnValue = 0x00;

	switch(binaryValue)
	{

	case 0x0:
		ReturnValue = '0';
		break;

	case 0x1:
		ReturnValue = '1';
		break;

	case 0x2:
		ReturnValue = '2';
		break;

	case 0x3:
		ReturnValue = '3';
		break;

	case 0x4:
		ReturnValue = '4';
		break;

	case 0x5:
		ReturnValue = '5';
		break;

	case 0x6:
		ReturnValue = '6';
		break;

	case 0x7:
		ReturnValue = '7';
		break;

	case 0x8:
		ReturnValue = '8';
		break;

	case 0x9:
		ReturnValue = '9';
		break;

	case 0xa:
		ReturnValue = 'A';
		break;

	case 0xb:
		ReturnValue = 'B';
		break;

	case 0xc:
		ReturnValue = 'C';
		break;

	case 0xd:
		ReturnValue = 'D';
		break;

	case 0xe:
		ReturnValue = 'E';
		break;

	case 0xf:
		ReturnValue = 'F';
		break;
		
	default:
		ReturnValue = 0x00;
		break;
	}

	return ReturnValue;
}


/***************************************************************************
  Function:		convertingFunctions_convertASCIIintoBinary()		
  Description:	Converts an ascii value into a binary value
  Inputs:		ASCIIcharacter: Ascii value
  Outputs:		None	
  Return:		The binary value	
  Globals:		None		
  Important:	
****************************************************************************/
char convertingFunctions_convertASCIIintoBinary(char ASCIIcharacter)
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


/***************************************************************************
  Function:		convertingFunctions_convertAddressInLongintoASCII()		
  Description:	Converts a long value into 8 ascii characters
  Inputs:		Value: Long value
  Outputs:		None	
  Return:		Pointer to 8 ascii characters	
  Globals:		None		
  Important:	
****************************************************************************/
char* convertingFunctions_convertAddressInLongintoASCII(unsigned long Value)
{
	static char AddressInASCII[8];

	AddressInASCII[7] = convertingFunctions_convertBinaryintoASCII((char) (Value	   & 0xf));
	AddressInASCII[6] = convertingFunctions_convertBinaryintoASCII((char) (Value >> 4  & 0xf));
	AddressInASCII[5] = convertingFunctions_convertBinaryintoASCII((char) (Value >> 8  & 0xf));
	AddressInASCII[4] = convertingFunctions_convertBinaryintoASCII((char) (Value >> 12 & 0xf));
	AddressInASCII[3] = convertingFunctions_convertBinaryintoASCII((char) (Value >> 16 & 0xf));
	AddressInASCII[2] = convertingFunctions_convertBinaryintoASCII((char) (Value >> 20 & 0xf));
	AddressInASCII[1] = convertingFunctions_convertBinaryintoASCII((char) (Value >> 24 & 0xf));
	AddressInASCII[0] = convertingFunctions_convertBinaryintoASCII((char) (Value >> 28 & 0xf));

	return &AddressInASCII[0];
}

/***************************************************************************
  Function:		convertingFunctions_convertAddressInASCIIintoLong()		
  Description:	Converts 8 ascii characters into a long value 
  Inputs:		CharArr: Pointer to 8 ascii characters	
  Outputs:		None	
  Return:		Long value	
  Globals:		None		
  Important:	
****************************************************************************/
long convertingFunctions_convertAddressInASCIIintoLong(char *CharArr)
{
	char MagicWordHex[] = "0x";
	unsigned long Value = 0;

	if (CharArr[0] == MagicWordHex[0])
	{
		if (CharArr[1] == MagicWordHex[1])
		{
			Value = ((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[9]));
			Value = Value | (((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[8])) << 4);
			Value = Value | (((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[7])) << 8);
			Value = Value | (((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[6])) << 12);
			Value = Value | (((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[5])) << 16);
			Value = Value | (((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[4])) << 20);
			Value = Value | (((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[3])) << 24);
			Value = Value | (((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[2])) << 28);
		}
	}
	else		// It is Width and a decimal value
	{
		Value = 10 * ((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[0]));
		Value = Value + ((unsigned long)convertingFunctions_convertASCIIintoBinary(CharArr[1]));
	}
	return Value;
}


/***************************************************************************
  Function:		convertingFunctions_mapToLDRFormat()		
  Description:	Changes the bytes to a special order
  Inputs:		Value: Long value	
  Outputs:		None	
  Return:		Formated long value	
  Globals:		None		
  Important:	
****************************************************************************/
long convertingFunctions_mapToLDRFormat(long Value)
{
	long TempValue = 0;

	TempValue = ((Value << 24) & 0xff000000) | ((Value << 8) & 0x00ff0000) | ((Value >> 8) & 0x0000ff00) | ((Value >> 24) & 0x000000ff);
	
	return TempValue;
}

