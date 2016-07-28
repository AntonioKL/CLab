/*
=====================================================================================================================
global_functions
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Stores functions that used in many modules
=====================================================================================================================
*/

#include "main_header.h"


/*Command list*/
const Command globalCommands[] = 
/*	Name | Number of Parametrs Required | Opcode */
{
	{ "mov" , 2, 0  },
	{ "cmp" , 2, 1  },
	{ "add" , 2, 2  },
	{ "sub" , 2, 3  },
	{ "not" , 1, 4  },
	{ "clr" , 1, 5  },
	{ "lea" , 2, 6  },
	{ "inc" , 1, 7  },
	{ "dec" , 1, 8  },
	{ "jmp" , 1, 9  },
	{ "bne" , 1, 10 },
	{ "red" , 1, 11 },
	{ "prn" , 1, 12 },
	{ "jsr" , 1, 13 },
	{ "rst" , 0, 14 },
	{ "stop", 0, 15 },
	{NULL} /*Null will be used as an end of the array*/
};

const Directive globalDirective[] = 
/* Name 	| 	Parse Function */
{	
	{ ".data", parseDataDirective } ,
	{ ".string", parseStringDirective } ,
	{ ".entry", parseEntryDirective },
	{ ".extern", parseExternDirective },
	{ NULL } /*Null will be used as an end of the array*/
};


int isLineEmpty(RunStatus *runStatus)
{
	skipSpaces(runStatus);
	if(*(runStatus->line)==EOF || *(runStatus->line)=='\n')
	{
        	return TRUE;
   	}
 	return FALSE;
}

int isLineComment(RunStatus *runStatus)
{
	if (*(runStatus -> originalLine) == ';')
	{
		return TRUE;
	}
	return FALSE;
		
}

char *getCharAfterSpace(char *p)
{
	while(isspace(*p))
	{
		if (*p == EOF || *p== '\n')
		{
			return p;
		}
		p++;
	}

	return p;
}

void skipSpaces(RunStatus *runStatus)
{
	runStatus -> line = getCharAfterSpace(runStatus -> line);
}


void getLabel(RunStatus *runStatus, char *label)
{
	char temp_label[MAX_LABEL_LEN]="\0";

	int i = 0; /*Counter for the label Chars*/
	char *c = runStatus-> originalLine;

	
	if (! strchr(runStatus-> originalLine, ':') )
		return ; /*It is not a label*/

	if (!isalpha(*c))
	{
		printf("ERROR: Line #%d, Invalid Label Name - Label should start with Letter at the first collumn.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		runStatus -> isLineErr = TRUE;
		return ;
	}
	
	while (!isspace(*c) && *c != '\n' && *c != EOF && *c != ':')
	{
		if (! isalnum(*c))
		{
			printf("ERROR: Line #%d, Invalid Label Name - Label should contain only Letters and Numbers.\n", runStatus -> lineCount);
			runStatus -> errNum ++;
			runStatus -> isLineErr = TRUE;
			return ;
		}
		i++;
		c++;
	}

	if (isspace(*c) || *c == '\n' || *c == EOF )
	{
		printf("ERROR: Line #%d, Invalid Label Name - Label should contain only one word, without spaces.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		runStatus -> isLineErr = TRUE;
		return ;
	}
	if (*c == ':')
	{
		if (i >= MAX_LABEL_LEN)
		{
			printf("ERROR: Line #%d, Invalid Label Name - Label should contain no more than %d chars.\n", runStatus -> lineCount, MAX_LABEL_LEN);
			runStatus -> errNum ++;
			runStatus -> isLineErr = TRUE;
			return ;
		}
		strncpy(temp_label, runStatus -> originalLine,i); /*We don't want to copy the ":" itself*/
		if(isRegister(temp_label))
		{
			printf("ERROR: Line #%d, Invalid Label Name - Illegal Name , You cannot use Register Name.\n", runStatus -> lineCount);
			runStatus -> errNum ++;
			runStatus -> isLineErr = TRUE;
			return ;
		}
		i++;
        	runStatus -> line += i;
		strcpy(label, temp_label);
	}


}

int isRegister(char *str)
{
	if (str[0] == 'r'  && str[1] >= '0' && str[1] - '0' <= MAX_REGISTERS && str[2] == '\0') 
	{
		return TRUE;
	}
	return FALSE;
}

void addLabelData(RunStatus *runStatus, char *label)
{
	runStatus -> labelArray = realloc(runStatus -> labelArray, (runStatus -> labelCount + 1) * sizeof(Label));
	if (! (runStatus -> labelArray) )
	{
		printf("Fatal ERROR: Fail to reallocate space for Label Array\n");
		runStatus -> flagFatalErr = EXIT_ERROR;
		return ;
	}
	strcpy (runStatus -> labelArray[runStatus -> labelCount].name , label);
	runStatus -> labelArray[runStatus -> labelCount].memAddress = runStatus -> dataCount;
	runStatus -> labelCount ++;
}

void addLabelFinal(RunStatus *runStatus, char *label, int mem, int isData)
{
	runStatus -> finalLabelArray = realloc(runStatus -> finalLabelArray, (runStatus -> finalLabelCount + 1) * sizeof(Label));
	if (! (runStatus -> finalLabelArray) )
	{
		printf("Fatal ERROR: Fail to reallocate space for Final Label Array\n");
		runStatus -> flagFatalErr = EXIT_ERROR;
		return ;
	}
	strcpy (runStatus -> finalLabelArray[runStatus -> finalLabelCount].name , label);
	runStatus -> finalLabelArray[runStatus -> finalLabelCount].memAddress = mem + FIRST_MEM_ADDR;
	runStatus -> finalLabelArray[runStatus -> finalLabelCount].isData = isData;
	runStatus -> finalLabelCount ++;
	if (!isData)
	{
		runStatus -> finalLabelArray[runStatus -> finalLabelCount].word = getCommandWord(runStatus, runStatus -> lineCount -1);
	}
}

void addDirData(RunStatus *runStatus, int num)
{
	runStatus -> dataArray = realloc(runStatus -> dataArray, (runStatus -> dataCount + 1) * sizeof(int));
	if (! (runStatus -> dataArray) )
	{
		printf("Fatal ERROR:Fail to reallocate space for Data Array\n");
		runStatus -> flagFatalErr = EXIT_ERROR;
	}
	runStatus -> dataArray[runStatus -> dataCount] = num;
	runStatus -> dataCount ++;
}

void getLabelReference(RunStatus *runStatus, char *label)
{
	int i = 0;
	char temp_label[MAX_LABEL_LEN]="\0";
	char *label_start = runStatus -> line;
	
	if (!isalpha(*(runStatus -> line)))
	{
		printf("ERROR: Line #%d, Invalid Label Reference - Label should start with Letter.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		return ;
	}
	
	while (! (isspace(*(runStatus -> line)) || *(runStatus -> line) == EOF || *(runStatus -> line) == '\n' ))
	{
		if (! isalnum(*(runStatus -> line)))
		{	
			printf("ERROR: Line #%d, Invalid Label Reference - Label should contain only Letters and Numbers.\n", runStatus -> lineCount);
			runStatus -> errNum ++;
			return ;
		}
		i++;
		runStatus -> line ++;
	}
	skipSpaces(runStatus);

	if (! (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n' ))
	{
		printf("ERROR: Line #%d, Invalid Label Reference - Directive doesn't hold a valid Label refernce, there is a space in the name.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		return ;
	}

	if (i >= MAX_LABEL_LEN)
	{
		printf("ERROR: Line #%d, Invalid Label Reference Name - Label should contain no more than %d chars.\n", runStatus -> lineCount, MAX_LABEL_LEN);
		runStatus -> errNum ++;
		return ;
	}

	strncpy(temp_label, label_start,i);

	if(isRegister(temp_label))
	{
		printf("ERROR: Line #%d, Invalid Label Name - Illegal Name , You cannot access Register Name.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		return ;
	}

	strcpy(label, temp_label);
	
	
}


void addEntryDir(RunStatus *runStatus, char *label)
{
	runStatus -> entryArray = realloc(runStatus -> entryArray, (runStatus -> entryCount + 1) * sizeof(Entry));
	if (! (runStatus -> entryArray) )
	{
		printf("Fatal ERROR:Fail to reallocate space for Entry Array\n");
		runStatus -> flagFatalErr = EXIT_ERROR;
	}
	strcpy(runStatus -> entryArray[runStatus -> entryCount].name, label);
	runStatus -> entryCount ++;
}

void addExternDir(RunStatus *runStatus, char *label)
{
	runStatus -> externArray = realloc(runStatus -> externArray, (runStatus -> externCount + 1) * sizeof(Extern));
	if (! (runStatus -> externArray) )
	{
		printf("Fatal ERROR:Fail to reallocate space for Extern array\n");
		runStatus -> flagFatalErr = EXIT_ERROR;
	}
	strcpy(runStatus -> externArray[runStatus -> externCount].name, label);
	runStatus -> externCount ++;
}

void addExternFile(RunStatus *runStatus, char *label, int memaddr)
{
	runStatus -> externFileArray = realloc(runStatus -> externFileArray, (runStatus -> externFileCount + 1) * sizeof(Extern));
	if (! (runStatus -> externFileArray) )
	{
		printf("Fatal ERROR:Fail to reallocate space for Extern File array\n");
		runStatus -> flagFatalErr = EXIT_ERROR;
	}
	strcpy(runStatus -> externFileArray[runStatus -> externFileCount].name, label);
	runStatus -> externFileArray[runStatus -> externFileCount].memAddress = memaddr + FIRST_MEM_ADDR; 
	runStatus -> externFileCount ++;
}

int getCommandId(RunStatus *runStatus)
{
	int i = 0;
	int count = 0;
	char cmd[MAX_LINE_LENGTH]="\0";

	while (!isspace(*(runStatus->line)))
	{
		cmd[count]=*(runStatus->line);
		runStatus->line++;
		count++;
	}
	
	if (!count)
	{
		i = -2;
		return -2;
	}

	while (globalCommands[i].name)
	{
		if (! (strcmp(globalCommands[i].name, cmd)))
		{
			return i;
		}
		i++;
	}
	
	i=-1; /*We didn't found the opCode for the operation*/
	return i;
	
}

void getOperand(RunStatus *runStatus, char *op)
{
	int count = 0;
	char temp_op[MAX_LABEL_LEN]="\0";

	while (!isspace(*(runStatus->line)) && (*(runStatus->line)!=','))
	{
		temp_op[count]=*(runStatus->line);
		runStatus->line++;
		count++;
	}

	strcpy(op, temp_op);
}


void parseOp(RunStatus *runStatus, char *opStr, Operand *op)
{
	strcpy(op -> str, opStr);

	if (isDynamicParam(op))
	{
		if (op -> type == INVAL)
		{
			printf("ERROR: Line #%d, Illegal Operand \"%s\" invalid syntax for range .\n", runStatus -> lineCount, op -> str);
			op -> type = INVAL;
			runStatus -> errNum ++;
			return ;
		}
		if (! checkDynamicRange(op -> up , op -> down))
		{
			printf("ERROR: Line #%d, Illegal Operand \"%s\" invalid range .\n", runStatus -> lineCount, op -> str);
			op -> type = INVAL;
			runStatus -> errNum ++;
			return ;
		}
		if (! isValidLabel (runStatus, op -> label))
		{
			op -> type = INVAL;
			return ;
		}
	}
	else if (isRegister(opStr))
	{
		op -> type = REGISTER;
		op -> val = opStr[1] - '0';
	}
	else if (*opStr == '#')
	{
		opStr++;
		if (isspace(*opStr))
		{
			printf("ERROR: Line #%d, Illegal Operand \"%s\" it has blank space after # .\n", runStatus -> lineCount, op -> str);
			op -> type = INVAL;
			runStatus -> errNum ++;
			return ;
		}
		else
		{
			if (isLegalNumber(runStatus, opStr, MEM_WORD_SIZE - 2, op))
			{
				op -> type = NUMBER;
			}
			else
			{
				printf("ERROR: Line #%d, Invalid Number \"%s\".\n", runStatus -> lineCount, op -> str);
				op -> type = INVAL;
				runStatus -> errNum ++;
				return ;
			}
		}
	}
	else if (isValidLabel (runStatus, opStr))
	{
		op -> type = DIRECT;
		strcpy(op -> label, opStr);
	}
	else
	{
		printf("ERROR: Line #%d, Invalid Operand \"%s\".\n", runStatus -> lineCount, op -> str);
		op -> type = INVAL;
		runStatus -> errNum ++;
		return ;
	}
}

int isDynamicParam(Operand *op)
{
	char temp_str[MAX_LABEL_LEN]="\0";
	char *start;
	char *end;
	char *dash;

	strcpy(temp_str, op->str);
	
	if (! (start = strchr(temp_str, '[')))
	{
		return FALSE;
	}

	
	end = strchr(temp_str, ']');
	dash = strchr(temp_str, '-');
	if (!(end && dash))
	{
		op -> type = INVAL;
		return TRUE;
	}

	*start = '\0';
	start ++;
	*end = '\0';
	*dash = '\0';
	dash++;
	end++;

	if (*end !='\0')
	{
		op -> type = INVAL;
		return TRUE;
	}

	op -> down = atoi(start);
	op -> up = atoi(dash);
	op -> type = DYNAMIC;
	strcpy(op -> label, temp_str);
	
	return TRUE;

}

int checkDynamicRange(int up, int down)
{
	if (down < 1 || up > 15 || up - down > 13 || up - down < 0 )
	{
		return FALSE;
	}
	return TRUE;
}

int isValidLabel(RunStatus *runStatus, char *label)
{
	char *temp_label = label;
	int i = 0;	
	
	if (!isalpha(*(temp_label)))
	{
		printf("ERROR: Line #%d, Invalid Operand Label - Label \"%s\" should start with Letter.\n", runStatus -> lineCount, label);
		return FALSE;
	}
	temp_label++;
	while (! (isspace(*(temp_label)) || *(temp_label) == EOF || *(temp_label) == '\n' || *(temp_label) == '\0'))
	{
		if (! isalnum(*(temp_label)))
		{	
			printf("ERROR: Line #%d, Invalid Operand Label - Label \"%s\" should contain only Letters and Numbers.\n", runStatus -> lineCount, label);
			runStatus -> errNum ++;
			return FALSE;
		}
		i++;
		temp_label++;
	}


	if (i >= MAX_LABEL_LEN)
	{
		printf("ERROR: Line #%d, Invalid Operand Label - Label \"%s\" contain no more than %d chars.\n", runStatus -> lineCount, label, MAX_LABEL_LEN);
		runStatus -> errNum ++;
		return FALSE;
	}

	return TRUE;
}

int isLegalNumber(RunStatus *runStatus, char *str, int max_size, Operand *op)
{
	char *end;
	int value;
	int maxNumber = (1 << max_size) -1; 

	value = strtol (str,&end, 10);
	if ( *end )
	{
		return FALSE;
	}

	if ( value > maxNumber || value < -maxNumber)
	{
		return FALSE;
	}

	op -> val = value;
	return TRUE;
	 
	
}

void increaseIC(RunStatus *runStatus)
{
		if (runStatus-> ic + runStatus-> dataCount < MAX_DATA_SIZE)
		{
			runStatus-> ic ++;
			runStatus -> lineArray[runStatus -> lineCount -1 ].addressSize ++;
		}
		else if (!runStatus -> flagFatalErr)
		{
			printf("ERROR: Not enough Memory to run\n");
			runStatus -> flagFatalErr = EXIT_ERROR;
		}
}


WordMemory getCommandWord(RunStatus *runStatus, int lineNum)
{
	WordMemory word = {0};
	int op1type = 0;
	int op2type = 0;
	Operand *op1 = runStatus->lineArray[lineNum].op1;
	Operand *op2 = runStatus->lineArray[lineNum].op2;
	int opcode = runStatus -> lineArray[lineNum].cmdId;
	int group = globalCommands[opcode].paramNum;

	if (op1 -> type != INVAL)
	{
		op1type = (int)op1 -> type;
	}
	if (op2 -> type != INVAL)
	{
		op1type = (int)op2 -> type;
	}

	word.eraBits = (eraBit)ABSOLUTE;
	word.wordBits.commandBits.dstMethod = op2type;
	word.wordBits.commandBits.srcMethod = op1type;
	word.wordBits.commandBits.opcode = opcode;
	word.wordBits.commandBits.group = group;
	word.wordBits.commandBits.unused = 5;
	
	return word;
}

int getIntFromWord(WordMemory word)
{
	unsigned int mask = ~0;
	int res;

	mask >>= (BITS_IN_BYTE * sizeof(int) - MEM_WORD_SIZE);
	res = mask & ((word.wordBits.dataBits << 2) + word.eraBits);
	return res;

}


WordMemory getOperandWord(RunStatus *runStatus, int isSrcOperand, Operand *op)
{
	char *str = op -> label;
	int i;
	int isLabelExtern;

	WordMemory word = {0};

	if (op -> type == REGISTER)
	{
		word.eraBits = (eraBit)ABSOLUTE;

		if (isSrcOperand)
		{
			word.wordBits.registerBits.srcReg = op -> val;
		}
		else
		{
			word.wordBits.registerBits.dstReg = op -> val;
		}
	}
	else
	{
		for ( i = 0; i < runStatus -> externCount; i++)
		{
			if (!strcmp(str, runStatus -> externArray[i].name))
			{
				isLabelExtern = TRUE;
			}
		}

		if (op -> type == DIRECT && isLabelExtern)
		{
			word.eraBits = (eraBit)EXTERNAL;
		}
		else
		{
			if (op -> type == DYNAMIC || op -> type == NUMBER)
			{
				word.eraBits = (eraBit)EXTERNAL;
			}
			else
			{
				word.eraBits = (eraBit)REALOCATBLE;
			}
		}

		word.wordBits.dataBits = op -> val;
	}
	return word;
}









