
/*
=====================================================================================================================
Main headers file
	Author: Anton Kuligin 
	Description: Headers declaration for the whole project
=====================================================================================================================
*/


/*-------------------------------
--- Include C lib declaration ---
---------------------------------*/

/*C libraries include*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*--------------------------------------
--- Declaration of Global Constants  ---
----------------------------------------*/

/*We were told that we are starting to run from a specific memory location*/
#define 	FIRST_MEM_ADDR			100

/*Minimun number of argumnets that should be passed to the ass. compiler*/
#define		MIN_ARGS			2

/*Maximum file name length, without extension, allowed*/
#define		MAX_FILENAME_LEN		100

/*Maximum file extension length allowed*/
#define		MAX_FILE_EXTENSION_LEN		5

/*Maximum length of the tag allowed*/
#define		MAX_TAG_LEN			30

/*Maximum numbers of the file lines allowed*/
#define		MAX_FILE_LINES			10000

/*Input Files Extension*/
#define 	INPUT_FILE_EXTENSION		".as"

/*TRUE/False definitions*/
#define		TRUE				1
#define		FALSE				0

/*------------------------
--- Struct declaration ---
--------------------------*/

typedef struct{
	char name[MAX_TAG_LEN];
	int address;
	/*int ext_flag;
	int cmd;*/
} Label;

typedef struct{
	int lineCount;
	char *line;
	char *originalLine;
	int errNum;
	int ic;

	Label *labelArray;
		

} RunStatus;


/*-------------------------------
--- Include module declaration ---
---------------------------------*/


#include "file_proccess.h"
#include "first_read_manager.h"
#include "global_functions.h"










