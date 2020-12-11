/*-------------------------------------------------------------------------*
 * $Workfile: ConvertLDR_ETL600.cpp $
 * Part of	: ETL600
 * Language	: C/C++
 * Created by	:  Buhl, Michael
 * Remarks	:  
 * Purpose	: Conversion of *.ldr Files
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /Source/P4LT/Tools/ConvertDSP_Loader/ConvertLDR_ETL600.cpp $
 * 
 * 1     11.07.04 18:59 Buhl01
 * initial
 *-------------------------------------------------------------------------*/


//
// Aufrufbeispiel:	>ConvertLDR NSD570_Analog_Version100.ldr NSD570_Analog_V100_forELCOTEQ.ldr
//								Source File					 Destination File
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include <stdafx.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

 
#define BYTE_PER_LINE      				10  // 0x00000000  = 30 78 30 30 30 30 30 30 30 30 = 10 Byte

int main(int argc, char* argv[])
{
	int c;
	unsigned int bytectr = 0;

	FILE *source;
	FILE *desti;

	char *prog = argv[0];

	
	if(argc == 1)   // No Argument
	{
		printf("\nUsage: ConvertLDR [Source File] [Destination File]");
		return 1;
	}
	else
	{
		if((source = fopen(*++argv, "rb")) == NULL)
		{
			fprintf(stderr, "\nERROR  !!!");
			fprintf(stderr, "\n%s: can't open %s\n",prog, *argv);
			return 1;
		}
		else
		{
			desti = fopen(*++argv, "wb");


			bytectr = BYTE_PER_LINE;
			while((c = getc(source)) != EOF)
			{
				if(--bytectr)
				{
					putc((c),desti);     // write data to destination
				}
				else
				{
					putc((c),desti);     // write data to destination

					putc((0x2C),desti);		 // insert comma
					bytectr = BYTE_PER_LINE + 2;
				}
			}
			fclose(source);
			fclose(desti);	

			printf("\n Conversion Successful !!!\n Output: %s", *argv);

		}
	}
	return 0;
}

