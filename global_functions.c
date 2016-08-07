/*
=====================================================================================================================
global_functions
	Author: Anton Kuligin 
	Git Project: https://github.com/AntonioKL/CLab
	Description: Stores functions that used in many modules
=====================================================================================================================
*/

#include "main_header.h"

/* Constants*/

/*Command list*/
const Command globalCommands[] = 
/*	Name | Number of Parameters Required | Opcode */
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

/* Functions*/

/*
Function that check if the line is empty or not
Input: 
	RunStatus Struct
Output: True if the line is empty and False otherwise
*/
int isLineEmpty(RunStatus *runStatus)
{
	/*Skipping all tabs and spaces*/
	skipSpaces(runStatus);
	if(*(runStatus->line)==EOF || *(runStatus->line)=='\n' || *(runStatus -> line) == '\0')
	{
        	return TRUE;
   	}
 	return FALSE;
}

/*
Function that check if the line is comment or not
Input: 
	RunStatus Struct
Output: True if the line is comment and False otherwise
*/
int isLineComment(RunStatus *runStatus)
{
	/*Comment is the line that starts with ;*/
	if (*(runStatus -> originalLine) == ';')
	{
		return TRUE;
	}
	return FALSE;
		
}

/*
Function that returns if the next char after the spaces and tabs
Input: 
	String
Output: Place after all Spaces 
*/
char *getCharAfterSpace(char *p)
{
	while(isspace(*p))
	{
		if (*p == EOF || *p == '\n' || *p == '\0')
		{
			return p;
		}
		p++; /* It is a space , move to next char*/
	}

	return p;
}

/*
Function that skip spaces at the start for a specific string 
Input: 
	RunStatus Struct
Output: -
*/
void skipSpaces(RunStatus *runStatus)
{
	runStatus -> line = getCharAfterSpace(runStatus -> line);
}

/*
Function that gets the label for the specific line of assembler code
Input: 
	RunStatus Struct
	label - that assigned it function if there is a label
Output: -
*/
void getLabel(RunStatus *runStatus, char *label)
{
	char temp_label[MAX_LABEL_LEN]="\0";

	int i = 0; /*Counter for the label Chars*/
	char *c = runStatus-> originalLine;

	
	if (! strchr(runStatus-> originalLine, ':') )
		return ; /*It is not a label*/

	if (!isalpha(*c))
	{
		printf("ERROR: Line #%d, Invalid Label Name - Label should start with Letter at the first column.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		runStatus -> isLineErr = TRUE;
		return ;
	}
	
	/*Check every char in the suspected label*/
	while (!isspace(*c) && *c != '\n' && *c != EOF && *c != '\0' && *c != ':')
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

	if (isspace(*c) || *c == '\n' || *c == EOF || *c == '\0' )
	{
		printf("ERROR: Line #%d, Invalid Label Name - Label should contain only one word, without spaces.\n", runStatus -> lineCount);
		runStatus -> errNum ++;
		runStatus -> isLineErr = TRUE;
		return ;
	}
	/*: char is not a part of the label but indicates that we've finished*/
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

		/*Label cannot be a register name*/
		if(isRegister(temp_label))
		{
			printf("ERROR: Line #%d, Invalid Label Name - Illegal Name , You cannot use Register Name as Label.\n", runStatus -> lineCount);
			runStatus -> errNum ++;
			runStatus -> isLineErr = TRUE;
			return ;
		}

		if(isCommand(temp_label))
		{
			printf("ERROR: Line #%d, Invalid Label Name - Illegal Name , You cannot use Command Name as Label.\n", runStatus -> lineCount);
			runStatus -> errNum ++;
			runStatus -> isLineErr = TRUE;
			return ;
		}

		i++;
        	runStatus -> line += i;
		strcpy(label, temp_label); /*copy to passed value*/
	}

}

/*
Function that checks if the passed string is a matching the register name or not
Input: 
	label string
Output: TRUE if it matches the label, FALSE otherwise.
*/
int isRegister(char *str)
{
	if (str[0] == 'r'  && str[1] >= '0' && str[1] - '0' <= MAX_REGISTERS && str[2] == '\0') 
	{
		return TRUE;
	}
	return FALSE;
}

/*
Function that checks if the passed string is a matching the command name or not
Input: 
	label string
Output: TRUE if it matches the label, FALSE otherwise.
*/
int isCommand(char *str)
{
	int i = 0;

	while (globalCommands[i].name)
	{
		if (! (strcmp(globalCommands[i].name, str)))
		{
			return TRUE;
		}
		i++;
	}
	return FALSE;
}


/*
Function that adds the label to data label struct
Input: 
	RunStatus struct
	label string
Output: -
*/
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

/*
Function that adds the label to "table of signs"
Input: 
	RunStatus struct
	label string
	memory address
	TRUE/FALSE - If the label refers to data or now
Output: -

*/
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
	
	if (!isData)	/*Adding word for data*/
	{
		runStatus -> finalLabelArray[runStatus -> finalLabelCount].word = getCommandWord(runStatus, runStatus -> lineCount -1);
	}
	runStatus -> finalLabelCount ++;
}

/*
Function that adds the data to data array
Input: 
	RunStatus struct
	number / char to add
Output: -
*/
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

/*
Function that parses the label name and checks that name of the label is valid
Input: 
	RunStatus struct
	label string
Output: -
*/
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
	
	while (! (isspace(*(runStatus -> line)) || *(runStatus -> line) == EOF || *(runStatus -> line) == '\n' || *(runStatus -> line) == '\0' ))
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

	if (! (*(runStatus -> line) == EOF || *(runStatus -> line) == '\n' || *(runStatus -> line) == '\0' ))
	{
		printf("ERROR: Line #%d, Invalid Label Reference - Directive doesn't hold a valid Label reference, there is a space in the name.\n", runStatus -> lineCount);
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

/*
Function that adds the label to entry labels struct
Input: 
	RunStatus struct
	label string
Output: -
*/
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

/*
Function that adds the label to extern definition of labels struct
Input: 
	RunStatus struct
	label string
Output: -
*/
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

/*
Function that adds the label to extern labels references - addresses 
Input: 
	RunStatus struct
	label string
	integer memory address
Output: -
*/
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

/*
Function that return the id of the command on the line
Input: 
	RunStatus struct
Output: integer command ID
*/
int getCommandId(RunStatus *runStatus)
{
	int i = 0;
	int count = 0;
	char cmd[MAX_LINE_LENGTH]="\0";
	
	/*getting the command*/
	while (! (isspace(*(runStatus->line)) || *(runStatus -> line) == EOF || *(runStatus -> line) == '\n' || *(runStatus -> line) == '\0' ))
	{
		cmd[count]=*(runStatus->line);
		runStatus->line++;
		count++;
	}
	
	/*Not a command at all, wrong line*/
	if (!count)
	{
		i = -2;
		return -2;
	}

	/*searching for opcode int the commands array*/
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

/*
Function that places the string of the operand in the provided variable
Input: 
	RunStatus struct
	operand Name
Output: -
*/
void getOperand(RunStatus *runStatus, char *op)
{
	int count = 0;
	char temp_op[MAX_LABEL_LEN]="\0";

	/*Searching for end of line or , next operand*/
	while (! (isspace(*(runStatus->line)) || *(runStatus -> line) == EOF || *(runStatus -> line) == '\n' || *(runStatus -> line) == '\0' ) && (*(runStatus->line)!=','))
	{
		temp_op[count]=*(runStatus->line);
		runStatus->line++;
		count++;
	}

	strcpy(op, temp_op);
}

/*
Function that parses the provided operand string and stores it inside provided Operand struct
Input: 
	RunStatus struct
	operand string
	Operand struct
Output: -
*/
void parseOp(RunStatus *runStatus, char *opStr, Operand *op)
{
	strcpy(op -> str, opStr);

	/*Check operand type*/
	if (isDynamicParam(op))
	{
		/*Validations on Dynamic reference*/
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
			/* Parsing Number*/
			if (isLegalNumber(runStatus, opStr, MEM_WORD_SIZE - 2, op) && ! (*opStr == EOF || *opStr == '\n' || *opStr== '\0') ) 
			{
				op -> type = NUMBER;
			}
			else
			{
				printf("ERROR: Line #%d, Invalid Number  \"%s\" for %d bits.\n", runStatus -> lineCount, op -> str, (MEM_WORD_SIZE - 2));
				op -> type = INVAL;
				runStatus -> errNum ++;
				return ;
			}
		}
	}
	else if (isValidLabel (runStatus, opStr)) /*Label reference */
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

/*
Function that parses the provided Operand struct and gets the up and down ranges( If it is possible)
Input: 
	Operand struct
Output: TRUE if the range exists and is passable , FALSE otherwise
*/
int isDynamicParam(Operand *op)
{
	char temp_str[MAX_LABEL_LEN]="\0";
	char *start;
	char *end;
	char *dash;

	strcpy(temp_str, op->str);
	
	/*Checks if there is a range in [*/
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
	
	/*Splitting the string to multiple based on their purpose*/
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
	
	/*Setting up/down bits*/
	op -> down = atoi(start);
	op -> up = atoi(dash);
	op -> type = DYNAMIC;
	strcpy(op -> label, temp_str);
	
	return TRUE;

}

/*
Function that checks if the provided range is valid
Input: 
	int high place
	int low place
Output: TRUE if in range , FALSE otherwise
*/
int checkDynamicRange(int up, int down)
{
	if (down < 0 || up > 14 || up - down >= 13 || up - down < 0 )
	{
		return FALSE;
	}
	return TRUE;
}

/*
Function that checks if the provided label is valid
Input: 
	RunStatus struct
	label string
Output: TRUE if it is valid , FALSE otherwise
*/
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
	
	/*Checking every char*/
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

	/*Check that the size of the label doesn't exceed maximum allowed value*/
	if (i >= MAX_LABEL_LEN)
	{
		printf("ERROR: Line #%d, Invalid Operand Label - Label \"%s\" contain no more than %d chars.\n", runStatus -> lineCount, label, MAX_LABEL_LEN);
		runStatus -> errNum ++;
		return FALSE;
	}

	return TRUE;
}

/*
Function that checks if the provided number is valid
Input: 
	RunStatus struct
	number string
	maximum allowed size for number
	Operand struct
Output: TRUE if it is valid , FALSE otherwise
*/
int isLegalNumber(RunStatus *runStatus, char *str, int maxSize, Operand *op)
{
	char *end;
	int value;
	int maxNumber = (1 << (maxSize -1)); /*Setting Number range*/

	value = strtol (str,&end, 10);/*Convert to Integer*/
	if ( *end )
	{
		return FALSE;
	}

	/*Check that the number is in allowed range*/
	if ( value > maxNumber-1 || value < (-1) * maxNumber)
	{
		return FALSE;
	}

	op -> val = value;
	return TRUE;
}

/*
Function that increase IC by 1 in RunStatus struct
Input: 
	RunStatus struct
Output: -
*/
void increaseIC(RunStatus *runStatus)
{
		if (runStatus-> ic + runStatus-> dataCount < MAX_DATA_SIZE)
		{
			runStatus-> ic ++;
		}
		else if (!runStatus -> flagFatalErr)
		{
			printf("ERROR: Not enough Memory to run\n");
			runStatus -> flagFatalErr = EXIT_ERROR;
		}
}

/*
Function that creates a language word from the command line
Input: 
	RunStatus struct
	integer line Number
Output: WordMemory created by passed command
*/
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
		op2type = (int)op2 -> type;
	}

	/*Setting the bits according to values*/
	word.eraBits = (eraBit)ABSOLUTE;
	word.wordBits.commandBits.dstMethod = op2type;
	word.wordBits.commandBits.srcMethod = op1type;
	word.wordBits.commandBits.opcode = opcode;
	word.wordBits.commandBits.group = group;
	word.wordBits.commandBits.unused = 5; /*Reserved Value and It should be 101 = 5 */
	
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

/*
Function that creates a language word from the passed operand struct
Input: 
	RunStatus struct
	TRUE/FALSE if the operand is source or not ( destination)
	Operand struct
Output: WordMemory created by passed operand struct
*/
WordMemory getOperandWord(RunStatus *runStatus, int isSrcOperand, Operand *op)
{
	char *str = op -> label;
	int i;
	int isLabelExtern = FALSE;

	WordMemory word = {0};

	/*Checking operand type*/
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
		/*Checking if the Label is extern*/
		for ( i = 0; i < runStatus -> externCount && ! isLabelExtern ; i++)
		{
			if (!strcmp(str, runStatus -> externArray[i].name))
			{
				isLabelExtern = TRUE;
			}
		}

		/*parsing operands and assigning ERA + value*/
		if (op -> type == DIRECT && isLabelExtern)
		{
			word.eraBits = (eraBit)EXTERNAL;
		}
		else
		{
			if (op -> type == DYNAMIC || op -> type == NUMBER)
			{
				word.eraBits = (eraBit)ABSOLUTE;
			}
			else
			{
				word.eraBits = (eraBit)REALOCATBLE;
			}
			word.wordBits.dataBits = op -> val;
		}
	}
	return word;
}


