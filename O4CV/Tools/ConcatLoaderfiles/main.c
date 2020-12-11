/*-------------------------------------------------------------------------*
 * $Workfile	: main.c $
 * Part of      : ETL600 / O4CV
 * Language     : C
 * Created by   : Allen Notter (AN)
 * Remarks      :  
 * Purpose      : Concatenates 2 loaderfiles
 * Copyright    : ABB Switzerland Ltd., 2007
 *-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*
 * $Log: /branches/Source O4CV/O4CV/Tools/main.c $
 * 
 * 1     07-10-11 15:16 Chalnot
 * Initial Version.
 * 
 *
 *-------------------------------------------------------------------------*/

/* INCLUDE FILES **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* LOCAL CONSTANT DECLARATIONS ***********************************************/

#define  LINE_LEN  1024

/* LOCAL TYPE DECLARATIONS ***************************************************/

/* LOCAL VARIABLES ***********************************************************/

/* LOCAL MACRO DEFINITIONS ***************************************************/

/* LOCAL FUNCTIONS ***********************************************************/

FILE* open_file(char* file_name, char* mode);

/* IMPLEMENTATION ************************************************************/

int main(int argc, char* argv[]) {
	FILE *fid_in1         , *fid_in2        , *fid_out;
	char  line_1[LINE_LEN], line_2[LINE_LEN], *result ;


	// check for exactely 3 input arguments:
	if (argc != 4) {
		printf("\nProgram to concatenate two intel hex format loader files into one intel\n");
		printf("hex loader file:\n\n");
		printf("  concat_ldr [options] [<input_file1> <input_file2> <output_file>]:\n\n");
		printf("    checksum -h:\n");
		printf("        prints this screen\n\n");
		printf("    checksum <input_file1> <input_file2> <output_file>:\n");
		printf("        concatenates input_file1 and input_file2 and writes result\n");
		printf("        to output_file\n\n");
		exit(0);
	}


	// try to open in- and output files:
	fid_in1 = open_file(argv[1], "rb");
	fid_in2 = open_file(argv[2], "rb");
	fid_out = open_file(argv[3], "wb!");

	// read input_file1 line by line and write each line except the last one to the output_file:
	result = fgets(line_1, LINE_LEN, fid_in1);
	if (result == NULL){
		printf("Error, file %s is empty\n", argv[1]);
		exit(0);
	}
	result = fgets(line_2, LINE_LEN, fid_in1);

	do {
		fputs  (line_1, fid_out);
		strncpy(line_1, line_2,LINE_LEN);
		result = fgets(line_2, LINE_LEN, fid_in1);
	} while (result != NULL);

	// read input_file2 line by line and write each line without excepttion to the output_file:
	result = fgets(line_1, LINE_LEN, fid_in2);

	if (result == NULL){								// if empty, teminate!
		printf("Warning, file %s is empty\n", argv[2]);
		fputs (line_2, fid_out);
	}

	while (result != NULL) {
		fputs (line_1, fid_out);
		result = fgets(line_1, LINE_LEN, fid_in2);
	}

	// cloase all files and quit:
	fclose(fid_in1);
	fclose(fid_in2);
	fclose(fid_out);

	printf("success, file %s written\n", argv[3]);

	return 1;
}


FILE* open_file(char* filename, char* mode){
	FILE* fid;

	fid = fopen(filename, mode);
	if (fid == NULL) {
		printf("Error, can't open file %s\n", filename);
		exit(0);
	}

	return fid;
}
