
/*
=====================================================================================================================
File proccessing - Headers file
	Author: Anton Kuligin 
	Description: Headers for reading the input files
=====================================================================================================================
*/

/*C libraries include*/
#include "used_c_lib.h"

/*------------------------------
--- Declaration of Constants ---
--------------------------------*/

/*Import Global constansts*/
#include "constants.h"

/*Local const*/
#define INPUT_FILE_EXTENSION		".as"

/*------------------------------
--- Declaration of Functions ---
--------------------------------*/

void parseFile(char* fileName);
FILE* openInputFile(char *fileName, char *extension, const char *openMode)
