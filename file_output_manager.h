
/*
=====================================================================================================================
FileOutputManager - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for Output proccess manager
=====================================================================================================================
*/

/*-----------------
--- Local const ---
-------------------*/

/*Defining the extension for various output files*/
#define		OBJECT_FILE_EXT		".ob"
#define		ENTRY_FILE_EXT		".ent"
#define		EXTERN_FILE_EXT		".ext"

/*Defining the mode to open a file for various output files*/
#define		MODE_WRITE_ONLY		"w"

/*Special Base8 parametrs*/
#define		BASE_NUM		8
#define		SPECIAL_BASE8_DIGITS	"!@#$%^&*"

/*------------------------------
--- Declaration of Functions ---
--------------------------------*/


void fileOutputmanager(RunStatus *runStatus, MemoryDump *memStatus, char *fileName);
void exportExtern(RunStatus *runStatus, char *fileName);
void exportEntry(RunStatus *runStatus, char *fileName);
void exportObject(RunStatus *runStatus, MemoryDump *memStatus, char *fileName);
void specialBase8Print(FILE *extFile, int memAddress);
void specialBase8DefinedSizePrint(FILE *extFile, int memAddress);
void specialBase8ConvertInt(int num, char *buffer);


