/*
=====================================================================================================================
Global Functions - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for Global Functions
=====================================================================================================================
*/


/*-----------------
--- Local const ---
-------------------*/

#define		EXIT_ERROR		exit(0)

/*------------------------------
--- Declaration of Functions ---
--------------------------------*/


int isLineEmpty(RunStatus *runStatus);
int isLineComment(RunStatus *runStatus);
char *getCharAfterSpace(char *p);
void skipSpaces();
int isLineWithTag(RunStatus *runStatus);
void getLabel(RunStatus *runStatus, char *label);
int isRegister(char *str);
void addLabel(RunStatus *runStatus, char *label);
void addDirData(RunStatus *runStatus, int num);
void getLabelReference(RunStatus *runStatus, char *label);
void addEntryDir(RunStatus *runStatus, char *label);
void addExternDir(RunStatus *runStatus, char *label);


