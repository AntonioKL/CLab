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

#define		EXIT_ERROR		exit(1)


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
void addLabelData(RunStatus *runStatus, char *label);
void addDirData(RunStatus *runStatus, int num);
void addLabelFinal(RunStatus *runStatus, char *label, int mem);
void getLabelReference(RunStatus *runStatus, char *label);
void addEntryDir(RunStatus *runStatus, char *label);
void addExternDir(RunStatus *runStatus, char *label);
int getCommandId(RunStatus *runStatus);
void getOperand(RunStatus *runStatus, char *op);
int isValidOperand(RunStatus *runStatus, char *op);
int getOpAddressMethod(RunStatus *runStatus, char *op);
void parseOp(RunStatus *runStatus, char *opStr, Operand *op);
int isDynamicParam(Operand *op);
int checkDynamicRange(int up, int down);
int isValidLabel(RunStatus *runStatus, char *label);
int isLegalNumber(RunStatus *runStatus, char *str, int max_size, Operand *op);





