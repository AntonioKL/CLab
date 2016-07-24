
/*
=====================================================================================================================
Main headers file
	Author: Anton Kuligin
	Git Project: https://github.com/AntonioKL/CLab 
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

/*Given maximum data size*/
#define		MAX_DATA_SIZE			1000

/*Minimun number of argumnets that should be passed to the ass. compiler*/
#define		MIN_ARGS			2

/*Maximum file name length, without extension, allowed*/
#define		MAX_FILENAME_LEN		100

/*Maximum file extension length allowed*/
#define		MAX_FILE_EXTENSION_LEN		5

/*Maximum length of the tag allowed*/
#define		MAX_LABEL_LEN			30

/*Maximum numbers of the file lines allowed*/
#define		MAX_FILE_LINES			10000

/*Input Files Extension*/
#define 	INPUT_FILE_EXTENSION		".as"

/*TRUE/False definitions*/
#define		TRUE				1
#define		FALSE				0

/*Maximum number of registrers*/
#define 	MAX_REGISTERS			7

/*------------------------
--- Struct declaration ---
--------------------------*/

typedef struct{
	char name[MAX_LABEL_LEN];
	int mem_address;
} Label;

typedef struct{
    char name[MAX_LABEL_LEN];
} Entry;

typedef struct{
    char name[MAX_LABEL_LEN];
} Extern;

typedef enum { INVAL = -1, NUMBER = 0, DIRECT = 1,  DYNAMIC = 2,  REGISTER = 3 } operandType;

typedef struct
{
	int value;				/* Value */
	char *str;				/* String */
	operandType type;			/* Type */
	int isDynamic;				/* 2nd addressing method flag */
	int address;				/* The adress of the operand in the memory */
	char *label;				/* The label in case of dynamic addresing */
	int upperRange;				/* The low end of the range for dynamic addressing */
	int lowerRange;				/* The high end of the range for dynamic addressing */
} Operand;

typedef struct{
	int address;
	int addressSize;
	int cmd;
	int numOperands;
	Operand op1;
	Operand op2;
} Lines;

typedef struct{

	char *line;
	char *originalLine;

	int errNum;
	int ic;
	
	Lines *lineArray;
	int lineCount;

	Label *labelArray;
	int labelCount;

	Entry *entryArray;
	int entryCount;

	Extern *externArray;
	int externCount;

	int * dataArray; /*We can store chars as integer array*/
	int dataCount;
	

} RunStatus;

typedef struct{
	char *name;
	void (*parsingFunc)();
} Directive;

typedef struct{
	char *name;
	unsigned int paramNum;
	unsigned int opCode;
} Command;

/*-------------------------------
--- Include module declaration ---
---------------------------------*/


#include "file_proccess.h"
#include "first_read_manager.h"
#include "global_functions.h"










