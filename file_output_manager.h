
/*
=====================================================================================================================
FileOutputManager - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for Output proccess
=====================================================================================================================
*/

/*-----------------
--- Local const ---
-------------------*/

#define		OBJECT_FILE_EXT		".obj"
#define		ENTRY_FILE_EXT		".ent"
#define		EXTERN_FILE_EXT		".ext"

#define		MODE_WRITE_ONLY		"w"

#define		BASE_NUM		8
#define		SPECIAL_BASE8_DIGITS	"!@#$%^&*"

/*------------------------------
--- Declaration of Functions ---
--------------------------------*/


void fileOutputmanager(RunStatus *runStatus, MemoryDump *memStatus, char *fileName);
void exportExtern(RunStatus *runStatus, char *fileName);
void exportEntry(RunStatus *runStatus, char *fileName);
void specialBase8Print(FILE *extFile, int memAddress);
void specialBase8ConvertInt(int num, char *buf, int index);


