
/*
=====================================================================================================================
Main headers file
	Author: Anton Kuligin
	Git Project: https://github.com/AntonioKL/CLab 
	Description: Headers declaration for the whole project
=====================================================================================================================
*/


/*------------------------
--- Includes of C libs ---
--------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


/*--------------------------------------
--- Declaration of Global Constants  ---
----------------------------------------*/

/*We were told that we are starting to run from a specific memory location*/
#define 	FIRST_MEM_ADDR			100

/*Given maximum data size*/
#define		MAX_DATA_SIZE			1000

/*Minimum number of arguments that should be passed to the ass. compiler*/
#define		MIN_ARGS			2

/* Word size in the processor */

#define		MEM_WORD_SIZE			15

/* Boundariy values for DATA*/
#define		DATA_MAX_INT		pow(2, ((MEM_WORD_SIZE) -1)) -1
#define		DATA_MIN_INT		(-1) * (pow(2, ((MEM_WORD_SIZE) -1)))

/*Bit that used to represent and 8 base special char*/
#define		NUM_BITS_BASE8			3

/*Maximum file name length, without extension, allowed*/
#define		MAX_FILENAME_LEN		100

/*Maximum file extension length allowed*/
#define		MAX_FILE_EXTENSION_LEN		5

/*Maximum length of the label allowed*/
#define		MAX_LABEL_LEN			30

/*Maximum numbers of the file lines allowed*/
#define		MAX_FILE_LINES			10000

/*Input Files Extension*/
#define 	INPUT_FILE_EXTENSION		".as"

/*TRUE and False definitions, to be more clear when writing code*/
#define		TRUE				1
#define		FALSE				0

/*Maximum number of registers*/
#define 	MAX_REGISTERS			7


/*------------------------
--- Struct declaration ---
--------------------------*/

typedef struct{ 						/* We need a 15 integers for the word, we can use the struct since minimum size for int is 16 */
	
	unsigned int eraBits : 2; 				/* First two are reserved for E.R.A bits*/
	
	union							/* Union struct since the word can be a command or data*/
	{
		int dataBits			: 13;		

		struct{
			unsigned int dstMethod	: 2;		/* Destination Operand Address Method ID */
			unsigned int srcMethod	: 2;		/* Source Operand Address Method ID */
			unsigned int opcode	: 4;		/* Command ID */
			unsigned int group	: 2;		/* Number of params passed to command ID*/
			unsigned int unused	: 3;		/* Unused Bit, it is always 101 */
		} commandBits;
		
		struct{						/* Operation between two registers requires only one word*/
			unsigned int dstReg	: 6; 		/* Destination register Number*/
			unsigned int srcReg	: 6; 		/* Source register Number*/
			unsigned int unused 	: 1;		/* Unused Bit, it is always 0 */
		} registerBits;

	}wordBits;
	
} WordMemory; /* Holds a 15 bits word*/

typedef struct{
	char name[MAX_LABEL_LEN];	/* Label Name*/
	int memAddress;			/* Label Address in memory*/
	int isData;			/* If the label points to data or command*/
	WordMemory word;		/* Word Translation to base8*/
} Label;				/* Holds the Labels representation*/

typedef struct{
	char name[MAX_LABEL_LEN];	/* Entry Name*/
	int memAddress;			/* Entry Address in memory*/
} Entry;				/* Holds the Entries representation*/

typedef struct{
	char name[MAX_LABEL_LEN];	/* Extern Name*/
	int memAddress;			/* Extern Address in memory*/
} Extern;				/* Holds the Extern representation*/

/* Holds a different types of Operand allowed*/
typedef enum { INVAL = -1, NUMBER = 0, DIRECT = 1,  DYNAMIC = 2,  REGISTER = 3 } operandType;

typedef struct
{
	operandType type;				/* Type of the operand*/
	int memAddress;					/* The address of the operand inside memory */
	unsigned int val;				/* Value of the operand */
	char str[MAX_LABEL_LEN];			/* operandString */
	char label[MAX_LABEL_LEN];			/* The label for dynamic addressing */
	int up;						/* The high value for dynamic addressing */
	int down;			 		/* The low value for dynamic addressing */
} Operand;

typedef struct{
	int address;					/* Address*/
	int cmdId;					/* Command ID*/
	int numOperands;				/* Number of Operands in line*/
	Operand *op1;					/* Pointer to Info about Destination operand*/
	Operand *op2;					/* Pointer to Info about Source operand*/
} Lines;						/* Holds information about the line*/

typedef struct{
	char *line;					/* Holds the line for parsing, can be changed*/
	char *originalLine;				/* Holds the original line , cannot be changed*/

	int flagFatalErr;				/* Flag to be raised in case of fail memory allocation*/
	int errNum;					/* Counter for Errors*/
	int isLineErr;					/* Flag to set if there is an error on the line*/
	int ic;						/* Instruction Counter */
	
	Lines *lineArray;				/* Holds an array of Information about the whole parsed lines*/
	int lineCount;					/* Line counter*/

	Label *finalLabelArray;				/* Holds the Final Label - table of signs*/
	int finalLabelCount;				/* Counter for Final Labels*/

	Label *labelArray;				/* Holds the Labels only for Data*/
	int labelCount;					/* Counter for Data Labels*/

	Entry *entryArray;				/* Holds the entry references*/
	int entryCount;					/* Counter for Entries*/

	Extern *externArray;				/* Holds the extern definitions*/
	int externCount;				/* Counter for Extern*/

	Extern *externFileArray;			/* Holds the Extern address references*/
	int externFileCount;				/* Count for Extern address references*/

	int *dataArray; 				/* Holds the data, We can store chars as integer*/
	int dataCount;					/* Counter for Data*/
} RunStatus;						/* Main Variable , hold the whole information about running state and parsing of the file*/

typedef struct{
	char *name;					/* Directive Name*/
	void (*parsingFunc)();				/* Corresponding parsing function*/
} Directive;						/* Holds information about directives*/

typedef struct{
	char *name;					/* Command name*/
	unsigned int paramNum;				/* Number of parameters passed to this commands*/
	unsigned int opCode;				/* Command ID*/
} Command;						/* Hold information about the command*/

/* Holds a different E.R.A bits allowed*/
typedef enum { ABSOLUTE = 0 , EXTERNAL = 1 , REALOCATBLE = 2 } eraBit;

typedef struct{
	int memArray[MAX_DATA_SIZE];			/* Array of memory words for .ob file in base10 */
	int wordCount;					/* Count of the numbers in Array*/
} MemoryDump;						/* Holds a dump of the memory before printing the files*/

/*-------------------------------
--- Include module declaration ---
---------------------------------*/


#include "file_proccess.h"
#include "first_read_manager.h"
#include "second_read_manager.h"
#include "global_functions.h"
#include "file_output_manager.h"









