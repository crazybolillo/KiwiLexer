#include "Tokenizer.h"
#include "Lexer.h"
#include "Parser.h"
#include <time.h>
#include <stdio.h>

//Define clear command for both Windows and Linux platforms.
#define CLEAR_COMAND "clear"
#if defined(_WIN32) || defined(_WIN64)
#undef CLEAR_COMAND
#define CLEAR_COMAND "cls"
#endif

#define INIT_MSG "KiwiCalculator. All numbers MUST have a decimal point.\n"
#define NOT_RECOG_MSG "\nCommand not recognized\n"

#define TOK_SIZE 128
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

char *alphabet = "\"+\" \"-\" \"*\" \"/\"";
char* productions = "SUM->DBL,+,DBL;"
					"SUB->DBL,-,DBL;"
					"MUL->DBL,*,DBL;"
					"DIV->DBL,/,DBL;";
struct KiwiInput prodtxt;

double numOne;
double numTwo;

void getInput(void);
void clearscr(void);
void convertNumbers(void);

void main()
{
	initMemory(&tokblock, tokmemory, TOK_SIZE);	
	initMemory(&lexblock, lexmemory, LEX_TOK_SIZE);
	initMemory(&symbolblock, symbolmemory, LEX_SYM_SIZE);
	initMemory(&parseblock, parsememory, PARSE_SIZE);
	printf(INIT_MSG);

	tokenizer = newAlphabet(alphabet, strlen(alphabet), &tokblock);

	prodtxt.text = productions;
	prodtxt.textSize = strlen(productions);
	prodtxt.readSize = 0;
	parser = newParser(&prodtxt, tokenizer, &parseblock, &symbolblock);

	/*MAIN PROGRAM LOOP. PROGRAM STARTS HERE*/
	while (1) 
	{
		getInput();
		struct Match match = parseNext(parser, tokens);
		if (strcmp(match.id, "SUM") == 0)
		{
			convertNumbers();
			printf(" = %f\n", numOne + numTwo);
		}
		else if (strcmp(match.id, "SUB") == 0)
		{ 
			convertNumbers();
			printf(" = %f\n", numOne - numTwo);
		}
		else if (strcmp(match.id, "DIV") == 0)
		{
			convertNumbers();
			printf(" = %f\n", numOne / numTwo);
		}
		else if (strcmp(match.id, "MUL") == 0)
		{
			convertNumbers();
			printf(" = %f\n", numOne * numTwo);
		}
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
	tokens = lexAll(&inputkiwi, tokenizer, &lexblock, &symbolblock,
		&dev_tokenOnlyMatch);
}

void clearscr() 
{
	system(CLEAR_COMAND);
	printf(INIT_MSG);
}

void convertNumbers()
{
	numOne = atof(tokens->token[0].value);
	numTwo = atof(tokens->token[2].value);
}

