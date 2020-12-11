/*-------------------------------------------------------------------------*
 * $Workfile	: main.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Checksum calculation of a prom-file
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/Tools/PromChecksumCalculator/main.c $
 * 
 * 1     07-10-11 15:07 Chalnot
 * Initial Version.
 * 
 *
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

/* IMPLEMENTATION ************************************************************/
 
int main(int argc, char* argv[]) {
	FILE         *fid;
	unsigned int  checksum, next_byte;


	// check for one exactely one input argument or for help arg:
	if ( (argc != 2) || (strcmp("-h", argv[1]) == 0) ) {
		printf("\nProgram to calculate the flash programer checksum of a binary \n");
		printf("flash file:\n\n");
		printf("  checksum [options] [<flash_file>]:\n\n");
		printf("    checksum -h:\n");
		printf("        prints this screen\n\n");
		printf("    checksum <flash_file>:\n");
		printf("        calculates the checksum of <flash_file>\n\n");
		printf("The checksum is calculated by adding each byte of the file to a zero\n");
		printf("initialised 32bit checksum.\n\n");
		exit(0);
	}

	// try to open the file specified as input arg:
	fid = fopen(argv[1], "rb");
	if (fid == NULL) {
		printf("Error, can't open specified input file\n");
		exit(0);
	}

	// calculate the checksum, simple addition of every byte into a 
	// 32bit checksum:
	checksum  = 0;
	next_byte = fgetc(fid);
	while (next_byte != EOF) {
		checksum += (next_byte & 0x000000FF);
		next_byte = fgetc(fid);
	}
	
	// close input file and print checksum:
	fclose(fid);
	fprintf(stdout, "\n**************************************************\n");
	fprintf(stdout, "\n\t\tChecksum: %08X\n", checksum );
	fprintf(stdout, "\n**************************************************\n");

	return 1;
}
