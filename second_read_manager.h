
/*
=====================================================================================================================
SecondReadManager - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for second read of the file
=====================================================================================================================
*/


/*-----------------
--- Local const ---
-------------------*/


/*----------------------------
--- Declaration of Structs ---
------------------------------*/

/*------------------------------
--- Declaration of Functions ---
--------------------------------*/

int SecondReadManager(RunStatus *runStatus, MemoryDump *memStatus);
void checkEntryLabels(RunStatus *runStatus);
int dumpLine(int lineNum, MemoryDump *memStatus, RunStatus *runStatus);
int dumpData(int dataArray,  MemoryDump *memStatus);
int updateOperandLabelAddress(Operand *op, RunStatus *runStatus, int lineNum);
int isLabelExists(RunStatus *runStatus, char *str);
int getLabelAddress(RunStatus *runStatus, Operand *op);



