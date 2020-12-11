/*-------------------------------------------------------------------------*
 * $Workfile	: ConvertingFunctions.h $
 * Part of		: ETL600 / O4CV
 * Language		: C
 * Created by	: Allen Notter (AN)
 * Remarks		:  
 * Purpose		: Format converting 
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/Tools/ChecksumPatcher/ConvertingFunctions.H $
 * 
 * 1     07-08-30 10:39 Chalnot
 * Initial version.
 * 
 * 
 *-------------------------------------------------------------------------*/

#ifndef CONVERTINGFUNCTIONS_H
#define CONVERTINGFUNCTIONS_H

/* INCLUDE FILES **************************************************************/

/* PUBLIC CONSTANT DECLARATIONS ***********************************************/

/* PUBLIC TYPE DECLARATIONS ***************************************************/

/* PUBLIC MACRO DEFINITIONS ***************************************************/

/* PUBLIC FUNCTIONS ***********************************************************/

long convertingFunctions_areStringsEqual(char *CharInArray1, char *CharInArray2, long LengthToCompare);
char convertingFunctions_convertASCIIintoBinary(char ASCIIcharacter);
char convertingFunctions_convertBinaryintoASCII(char binaryValue);
long convertingFunctions_convertAddressInASCIIintoLong(char *CharArr);
char* convertingFunctions_convertAddressInLongintoASCII(unsigned long Value);
long convertingFunctions_mapToLDRFormat(long Value);

#endif


