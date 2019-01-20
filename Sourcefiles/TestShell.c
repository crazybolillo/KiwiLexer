#include "Tokenizer.h"
#include "Lexer.h"
#include "Parser.h"
#include <time.h>
#include <stdio.h>

#define CLEAR_COMAND "clear"
#if defined(_WIN32) || defined(_WIN64)
    #undef CLEAR_COMAND
	#define CLEAR_COMAND "cls"
#endif

#define INIT_ERROR "I CAN'T CODE. ERROR WHILE INITAITING SHELL.\
PLEASE FIX ME"
#define INIT_MSG "KiwiLexer Testing Shell. \"help\" will show available commands.\n"
#define FILE_ERR_MSG "\nCould not open file.\n"
#define NO_DATA_MSG "\nNothing to print...\n"
#define NOT_RECOG_MSG "\nCommand not recognized\n"

#define HELP_CMD "help"
#define TOKEN_CMD "alph"
#define PRNT_TOK_CMD "printtok"
#define LEX_CMD "lex"
#define PRNT_LEX_CMD "printlex"
#define EXIT_CMD "exit"

/*Oh boy...*/
#define MEM_DECL(name, memname, size) char name[size]; \
struct MemBlock memname;

/*Memory blocks that will be used troughout the application are
declared here. The memory blocks used by the test shell itself to 
parse commands are all prefixed by "dev_". All the other memory
blocks are used to lex and parse user files.*/
#define DEV_TOK_SIZE 256
#define DEV_LEX_SIZE 256
#define DEV_PARS_SIZE 256
#define BUF_SIZE 128

/*All shell commands*/
char *shellalph = "\"exit\"\
\"tokenize\" \"lex\" \"printtok\" \"printlex\"";

/*Productions for the shell commands.*/
char *parsetxt = "alph->tokenize, CONST_STR; lex->lex, CONST_STR; \
printtok->printtok; printlex->printlex, INT; exit->exit;";
struct KiwiInput parseinput;

/*Declares variables in an horrendous way with macros. It creates
a char array with its identifier as the first argument and a 
memory block (struct) to go with it with its identifier as the 
second argument. The array's size is the third argument.*/
MEM_DECL(dev_tokmemory, dev_tokmem, DEV_TOK_SIZE)
struct LinkList *dev_tokenizer = NULL;

MEM_DECL(dev_lexmemory, dev_lexmem, DEV_LEX_SIZE)
MEM_DECL(dev_symbolmemory, dev_symmem, DEV_LEX_SIZE)
struct TokenArray *dev_tokens;

MEM_DECL(dev_parsmemory, dev_parsmem, DEV_PARS_SIZE)
struct Production *dev_parser = NULL;

char dev_input[BUF_SIZE];
struct KiwiInput dev_inputkiwi;
struct Match dev_parsematch;

#define TOK_SIZE 1440
#define LEX_TOK_SIZE 5120
#define LEX_SYM_SIZE 1024
#define PARSE_SIZE 2048

MEM_DECL(tokmemory, tokmem, TOK_SIZE)
struct LinkList *mem_tokenizer = NULL;

MEM_DECL(lexmemory, lexmem, LEX_TOK_SIZE)
MEM_DECL(symbolmemory, symbolmem, LEX_TOK_SIZE)
struct TokenArray *tokens = NULL;

MEM_DECL(parsememory, parsmem, PARSE_SIZE)
struct LinkList *parser = NULL;

struct KiwiInput fileinput;

void getInput(void);
void clearscr(void);
void freeptr(void);
char *readfile(char *filename, unsigned int *fsize, int sector);

void main()
{
	/*Initialize the shell memory blocks and its alphabet and parser.
	Boilerplate warning.*/
	initMemory(&dev_tokmem, dev_tokmemory, DEV_TOK_SIZE);
	initMemory(&dev_lexmem, dev_lexmemory, DEV_TOK_SIZE);
	initMemory(&dev_symmem, dev_symbolmemory, DEV_TOK_SIZE);
	initMemory(&dev_parsmem, dev_parsmemory, DEV_TOK_SIZE);

	dev_tokenizer = newAlphabet(shellalph, strlen(shellalph),
		&dev_tokmem);
	if (dev_tokenizer == NULL) {
		printf("TOKENIZER ERROR. FIX ME\n");
	}
	else {}
	parseinput.text = parsetxt;
	parseinput.readSize = 0;
	parseinput.textSize = strlen(parsetxt);
	dev_parser = newParser(&parseinput, dev_tokenizer, &dev_parsmem,
		&dev_symmem);
	if (dev_parser == NULL) {
		printf("PARSER ERROR. FIX ME\n");
	}
	else {}

	/*Initialize memory blocks that will be used when lexing and
	parsing the test input.*/
	initMemory(&tokmem, tokmemory, TOK_SIZE);	
	initMemory(&lexmem, lexmemory, LEX_TOK_SIZE);
	initMemory(&symbolmem, symbolmemory, LEX_SYM_SIZE);
	initMemory(&parsmem, parsememory, PARSE_SIZE);
	printf(INIT_MSG);

	/*MAIN PROGRAM LOOP. PROGRAM STARTS HERE*/
	while (1) {
		getInput();
		dev_parsematch = parseNext(dev_parser, dev_tokens);

		if (dev_parsematch.id == NULL) {
			printf(NOT_RECOG_MSG);
		}
		else if (strcmp(dev_parsematch.id, TOKEN_CMD) == 0) {
			freeptr();
			int size = 0;
			char *alphabet = readfile((dev_tokens->token + 1)->value,
				&size, 256);
			if (alphabet != NULL) {
				mem_tokenizer = newAlphabet(alphabet, size,
					&tokmem);
				free(alphabet);
			}
		}
		else if (strcmp(dev_parsematch.id, PRNT_TOK_CMD) == 0) {
			if (mem_tokenizer != NULL) {
				printf("\n");
				printAlphabet(mem_tokenizer);
				printf("\n");
			}
			else
				printf(NO_DATA_MSG);
		}
		else if (strcmp(dev_parsematch.id, LEX_CMD) == 0) {
			if (tokens != NULL) {
				freeMemory(&lexmem);
				tokens = NULL;
			}
			clock_t initTime = clock();
			unsigned int size = 0;
			char *lex = readfile((dev_tokens->token + 1)->value,
				&size, 256);
			if (lex != NULL) {
				fileinput.text = lex;
				fileinput.textSize = size;
				fileinput.readSize = 0;
			    printf("\nLexing file... %u bytes read.\n", size);
				tokens = lexAll(&fileinput, mem_tokenizer, &lexmem, 
					&symbolmem);
				clock_t stopTime = clock();
				printf("Lexing completed. %d tokens generated\
Took: %f seconds.\n", tokens->size, 
					(double)(stopTime - initTime) / CLOCKS_PER_SEC);
			}
		}
		else if (strcmp(dev_parsematch.id, PRNT_LEX_CMD) == 0) {
			if (tokens != NULL) {
				printf("\n");
				printTokenStream(tokens, *((dev_tokens->token + 1)->value));
				printf("\n");
			}
			else
				printf(NO_DATA_MSG);
		}
		else if (strcmp(dev_parsematch.id, CLEAR_COMAND) == 0) {
			clearscr();
		}
		else if (strcmp(dev_parsematch.id, EXIT_CMD) == 0) {
			exit(0);
		}
		else {
			printf(NOT_RECOG_MSG);
		}

	}
}

/*Reads input from stdin and lexes it all so it can later be parsed. Resets
the pointer to the shell input as it is modified during lexing. */
void getInput() 
{	
	printf("\n$> ");
	fgets(dev_input, BUF_SIZE, stdin);
	dev_inputkiwi.text = dev_input;
	for (int x = 0; x < BUF_SIZE; x++) {
		if (*(dev_input + x) == '\n') {
			dev_inputkiwi.textSize = x + 1;
			dev_inputkiwi.readSize = 0;
			break;
		}
	}
	freeMemory(&dev_tokmem);
	freeCleanMemory(&dev_symmem);
	dev_tokens = lexAll(
		&dev_inputkiwi, dev_tokenizer, &dev_lexmem, &dev_symmem);
}

void clearscr() 
{
	system(CLEAR_COMAND);
	printf(INIT_MSG);
}

void freeptr() 
{
	if (tokens != NULL) {
		tokens = NULL;
		freeMemory(&lexmem);
	}
	if (mem_tokenizer != NULL) {
		mem_tokenizer = NULL;
		freeMemory(&tokmem);
	}
}

char *readfile(char *filename, unsigned int *fsize, int sector) 
{
	FILE *fl = fopen(filename, "r");
	if (fl != NULL) {
		int count = 1;
		char *retval = NULL;
		do {
			fseek(fl, 0, SEEK_SET);
			retval = realloc(retval, count * sector);
			*fsize = fread(retval, 1, sector * count, fl);
			count++;
		} while (feof(fl) == 0);
		fclose(fl);
		retval = realloc(retval, *fsize);
		return retval;
	}
	else
		printf(FILE_ERR_MSG);
		return NULL;
}
