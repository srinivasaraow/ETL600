/*-------------------------------------------------------------------------*
 * $Workfile: GenerateLoaderFiles.cpp $
 * Part of	: ETL600
 * Language	: C/C++
 * Created by	:  Buhl, Michael
 * Remarks	:  
 * Purpose	: 
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT Prototype 2/Tools/GenerateLoaderFiles/GenerateLoaderFiles.cpp $
 * 
 * 2     6.06.05 20:35 Chmibuh
 * Size check of Emergency FW only
 * 
 * 1     11.12.04 18:55 Chmibuh
 * initial
 *-------------------------------------------------------------------------*/


//
// Aufrufbeispiel:	>GenerateLoaderFiles dummy_build.ldr 
//										 Source File					 
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include <stdafx.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 
#define BOOT_ADRESS_OFFSET					0x000EA6
#define	ADR_LENGTH							8
#define	MAX_PRG_SIZE						0x00010000   // 64 KByte
#define BOOTADDRESS_OF_MASTER				0x00810000
#define BOOTADDRESS_OF_EMERGENCY			0x00800000


//int defauftAdr[ADR_LENGTH] = {0x30,0x30,0x30,0x30,0x38,0x31,0x30,0x30};  // 00008100 = 0x00810000 = BOOTADDRESS_OF_MASTER with bootloader
int defauftAdr[ADR_LENGTH] = {0x32,0x41,0x30,0x36,0x38,0x31,0x30,0x30};  // 2A068100 = 0x0081062A = BOOTADDRESS_OF_MASTER after bootloader

unsigned long adressholder;

int readAdr[ADR_LENGTH];


bool translateAdr(void)
{	
	// e.g. received in readAdr['2','A','0','6','8','1','0','0'] 
	// translate now to long adressholder = 0x0081062A
	
	int tempHelp;

	adressholder = 0;

	tempHelp = readAdr[0];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp << 4);

	tempHelp = readAdr[1];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp);

	tempHelp = readAdr[2];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp << 12);

	tempHelp = readAdr[3];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp << 8);

	tempHelp = readAdr[4];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp << 20);

	tempHelp = readAdr[5];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp << 16);

	tempHelp = readAdr[6];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp << 28);

	tempHelp = readAdr[7];
	if((tempHelp >= 'A') && (tempHelp <= 'F'))
	{
		tempHelp = tempHelp - 55;
	}
	else 
	{
		tempHelp = tempHelp - 48;
	}	
	adressholder |= (tempHelp << 24);


	if(adressholder > BOOTADDRESS_OF_MASTER)
	{

		return (0);
	}
	return (1);
}




int main(int argc, char* argv[])
{
	int c;
	unsigned int bytectr = 0;
	unsigned int adressCounter = 0;

	FILE *source;


	char *prog = argv[0];
////
	if((source = fopen(*++argv, "rb")) == NULL)
	{
		fprintf(stderr, "\nERROR  !!!");
		fprintf(stderr, "\n%s: can't open %s\n",prog, *argv);
		fclose(source);
		return 1;

	}
	else
	{		
		while((c = getc(source)) != EOF)
		{
			
			++bytectr;

			if(bytectr <= BOOT_ADRESS_OFFSET)
			{

			}
			else if(bytectr <= (BOOT_ADRESS_OFFSET + ADR_LENGTH))
			{				
				readAdr[adressCounter] = c;
				adressCounter++;
				if(adressCounter == ADR_LENGTH)
				{
					if(!translateAdr())   // translate and check tool-generated address < 64 KByte
					{
						fprintf(stderr, "\nERROR  !!!");
						fprintf(stderr, "\nEMERGENCY FW greater than 64 KByte !!!  (%i Bytes)", (adressholder - BOOTADDRESS_OF_MASTER));
						//// normally return 1 - break!!!!!!!!!
					}
					else
					{
						fprintf(stderr, "\nEMERGENCY FW size OK  (%i Bytes)", (adressholder - BOOTADDRESS_OF_EMERGENCY));
					}
				}
			}
		}
		fclose(source);
	}
	return 0;
}

