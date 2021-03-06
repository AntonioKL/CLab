
/*
=====================================================================================================================
SecondReadManager - Headers file
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Headers for second read of the file
=====================================================================================================================
*/


/*------------------------------
--- Declaration of Functions ---
--------------------------------*/

int SecondReadManager(RunStatus *runStatus, MemoryDump *memStatus);
void checkEntryLabels(RunStatus *runStatus);
void dumpLine(int lineNum, MemoryDump *memStatus, RunStatus *runStatus);
void dumpData(int dataArray,  MemoryDump *memStatus);
int updateOperandLabelAddress(Operand *op, RunStatus *runStatus, int lineNum);
int isLabelExists(RunStatus *runStatus, char *str);
int getLabelAddress(RunStatus *runStatus, Operand *op, int checkExtern);
int getRequiredBitsFromLabel(int val, int up , int down);
void addToMem(MemoryDump *memStatus, WordMemory word);

