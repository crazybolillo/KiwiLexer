#include "Tokenizer.h"
#include "Lexer.h"
#include "Parser.h"
#include <time.h>
#include <stdio.h>
#include <math.h>

//Define clear command for both Windows and Linux platforms.
#define CLEAR_COMAND "clear"
#if defined(_WIN32) || defined(_WIN64)
#undef CLEAR_COMAND
#define CLEAR_COMAND "cls"
#endif

#define INIT_MSG "KiwiCalculator. All numbers MUST have a decimal point.\n"
#define NOT_RECOG_MSG "\nCommand not recognized\n"

#define TOK_SIZE 152
#define LEX_TOK_SIZE 512
#define LEX_SYM_SIZE 512
#define PARSE_SIZE 512

#define BUF_SIZE 256
char input[BUF_SIZE];
struct KiwiInput inputkiwi;

char tokmemory[TOK_SIZE];
struct MemBlock tokblock;
struct AlphList *tokenizer = NULL;

char lexmemory[LEX_TOK_SIZE];
struct MemBlock lexblock;
struct TokenArray *tokens = NULL;

char symbolmemory[LEX_TOK_SIZE];
struct MemBlock symbolblock;

char parsememory[PARSE_SIZE];
struct MemBlock parseblock;
struct Production *parser = NULL;

char *alphabet = "\"+\" \"-\" \"*\" \"/\" \"^\" \"&\" \"clear\"";
char* productions = "SUM->DBL, +;"
					"SUB->DBL, -;"
					"DIV->DBL, /;"
					"MUL->DBL, *;"
					"POW->DBL, ^;"
					"SQR->DBL, &;"
					"NUM->DBL;"
					"clear->clear;";
struct KiwiInput prodtxt;

double numOne;
double numTwo;
double numThree;

void getInput(void);
void clearscr(void);

/*-----------
Items dedicated to processing Matches in order to respect
the operations hierarchy
--------------*/
#define MATCH_SIZE 512
char matchmem[MATCH_SIZE];
struct MemBlock matchblock;

struct Number
{
	double value;
	char operation;
};

struct NumberArray
{
	struct Number *num;
	uint8_t size;
};

const uint8_t MAX_POS = 3; //^ and sqrt
const uint8_t MED_POS = 2; // * and /
const uint8_t MIN_POS = 1; // + and -

uint8_t getHierarchy(char operation);
struct Number joinNumber(struct Number *num, struct Number *numtw);
struct Number *convertToNumber(struct Match match, struct MemBlock *mem);
void simplifyExpression(struct NumberArray *nums);
void joinArray(struct NumberArray *nums, uint8_t index);

void main()
{
	//Iniitalize memory blocks
	initMemory(&tokblock, tokmemory, TOK_SIZE);	
	initMemory(&lexblock, lexmemory, LEX_TOK_SIZE);
	initMemory(&symbolblock, symbolmemory, LEX_SYM_SIZE);
	initMemory(&parseblock, parsememory, PARSE_SIZE);
	initMemory(&matchblock, matchmem, MATCH_SIZE);

	//Load alphabet and parser
	tokenizer = newAlphabet(alphabet, strlen(alphabet), &tokblock);

	prodtxt.text = productions;
	prodtxt.textSize = strlen(productions);
	prodtxt.readSize = 0;
	parser = newParser(&prodtxt, tokenizer, &parseblock, &lexblock);

	/*MAIN PROGRAM LOOP. PROGRAM STARTS HERE*/
	printf(INIT_MSG);
	while (1) 
	{
		struct Match match;
		struct NumberArray numarray;

		__start__:
		getInput();
		numarray.num = (struct Number*)matchmem;
		numarray.size = 0;

		while(tokens->size > 0)
		{
			match = parseNext(parser, tokens);
			if(strcmp(match.id, "clear") == 0)
			{
				clearscr();
				goto __start__;
			}
			else if (strcmp(match.id, "NUM") == 0)
			{
				struct Number *num = kimalloc(sizeof(struct Number),
					&matchblock);
				num->value = atoi(tokens->token->value);
				num->operation = '0';
			}
			else if (strcmp(match.id, ERR_ID) != 0)
			{
				convertToNumber(match, &matchblock);
			}
			else {
				puts("Command not recognized");
				goto __start__;
			}
			tokens->token += match.size;
			tokens->size -= match.size;
			numarray.size++;
		}
		for (int x = 0; x < numarray.size; x++)
		{
			printf("\n%f %c ", numarray.num[x].value, 
				numarray.num[x].operation);
		}
		puts("");
		simplifyExpression(&numarray);
		printf("RESULT ---> %f\n", numarray.num->value);
	}
}

/*Reads input from stdin and lexes it all so it can later be parsed. Resets
the pointer to the shell input as it is modified during lexing. */
void getInput() 
{	
	printf("\n$> ");
	fgets(input, BUF_SIZE, stdin);
	inputkiwi.text = input;
	for (int x = 0; x < BUF_SIZE; x++) 
	{
		if (input[x] == '\n') 
		{
			inputkiwi.textSize = x;
			inputkiwi.readSize = 0;
			break;
		}
	}
	freeMemory(&lexblock);
	freeMemory(&symbolblock);
	freeMemory(&matchblock);
	tokens = lexAll(&inputkiwi, tokenizer, &lexblock, &symbolblock,
		&dev_tokenOnlyMatch);
}

void clearscr() 
{
	system(CLEAR_COMAND);
	printf(INIT_MSG);
}

//--------
//FUNCTIONS RELATED TO PROCESSING MATCHES
//--------
uint8_t getHierarchy(char operation)
{
	if (operation == '-' || operation == '+')
	{
		return MIN_POS;
	}
	else if (operation == '*' || operation == '/')
	{
		return MED_POS;
	}
	else if (operation == '^' || operation == '&') //& = sqrt
	{
		return MAX_POS;
	}
	else return 0;
}

/*
BOILER PLATE AHEAD
Joins two numbers by performing the operation of the first number
and leaving the result with the operation of the second number.
Eg: 5 x 7 x 9
Joining the first two numbers == 35 x 9*/
struct Number joinNumber(struct Number *num, struct Number *numtw)
{
	struct Number retval;
	retval.operation = numtw->operation;
	if(num->operation == '+') 
	{
		retval.value = num->value += numtw->value;
	}
	else if(num->operation == '-')
	{
		retval.value = num->value -= numtw->value;
	}
	else if(num->operation == '*')
	{
		retval.value = num->value *= numtw->value;
	}
	else if(num->operation == '/')
	{
		retval.value = num->value /= numtw->value;
	}
	else if(num->operation == '^')
	{
		retval.value = pow(num->value, numtw->value);
	}
	else if(num->operation == '&')
	{
		retval.value = pow(num->value, (1.0 / numtw->value));
	}
	return retval;
}

struct Number *convertToNumber(struct Match match, struct MemBlock *mem)
{
	struct Number *retval = kimalloc(sizeof(struct Number), mem);
	if (retval != NULL) 
	{ 
		retval->value = atoi(tokens->token->value);
		retval->operation = *(tokens->token[1].value);
	}
	return retval;
}

void simplifyExpression(struct NumberArray *nums)
{
	uint8_t index = 0;
	struct Number numOne;
	uint8_t posOne;

	struct Number numTwo;
	uint8_t posTwo;

	while((index + 1) < nums->size)
	{
		numOne = nums->num[index];
		numTwo = nums->num[index + 1];
		
		posOne = getHierarchy(numOne.operation);
		posTwo = getHierarchy(numTwo.operation);

		if(posOne >= posTwo)
		{
			numOne = joinNumber(&numOne, &numTwo);
			nums->num[index] = numOne;
			joinArray(nums, index);
			index = 0;
		}
		else {
			index++;
		}
	}
}

void joinArray(struct NumberArray *nums, uint8_t index)
{
	for(uint8_t x = index + 2; x < nums->size; x++) 
	{
		nums->num[x - 1] = nums->num[x];
	}
	nums->size--;
}

