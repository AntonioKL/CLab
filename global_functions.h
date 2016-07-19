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
void addDirective(RunStatus *runStatus, int num);

