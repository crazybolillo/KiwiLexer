#include "Tokenizer.h"
#include "Lexer.h"

#define CLEAR_COMAND "clear"

#if defined(_WIN32) || defined(_WIN64)
    #undef CLEAR_COMAND
	#define CLEAR_COMAND "cls"
#endif

#define INIT_MSG "KiwiLexer Testing Shell. \"help\" will show available commands.\n"
#define HELP_MSG "\ntokenize --> Reads an alphabet from the text file and loads it into memory.\n\
print tok --> Prints the current tokens  and lexems from the tokenizer\n\
lex --> Reads a file and lexes it, printing a stream of tokens.\n\
print lex --> Prints the current token stream from the last file that was lexed.\n\
exit --> Exits the shell.\n\
You can add \"mem\" before \"tokenize\" or \"lex\" to use their memory efficient versions.\n"
#define FILE_ERR_MSG "\nCould not open file.\n"
#define NO_DATA_MSG "\nNothing to print...\n"
#define NOT_RECOG_MSG "\nCommand not recognized\n"

#define HELP_CMD "help"
#define TOKEN_CMD "tokenize"
#define M_TOKEN_CDM "memtokenize"
#define PRNT_TOK_CMD "printtok"
#define M_PRNT_TOK_CMD "memprinttok"
#define LEX_CMD "lex"
#define M_LEX_CMD "memlex"
#define PRNT_LEX_CMD "printlex"
#define EXIT_CMD "exit"


#define BUF_SIZE 156

static char input[BUF_SIZE];
static struct TokenStream *tokenStream = NULL;
static struct LinkToken *tokenizer = NULL;
static struct mem_LinkToken *mem_tokenizer = NULL;

void getInput(void);
void clearscr(void);
void freeptr(void);
char *readfile(char *filename, unsigned int *fsize, int sector);

void main()
{
	printf(INIT_MSG);
	while (1) {
		getInput();
		if (strncmp(input, HELP_CMD, strlen(input)) == 0) {
			printf(HELP_MSG);
		}
		else if (strncmp(input, TOKEN_CMD, strlen(TOKEN_CMD)) == 0) {
			freeptr();
			int size = 0;
			char *alphabet = readfile(input + strlen(TOKEN_CMD), &size, 256);
			if (alphabet != NULL) {
				tokenizer = createTokenizer(alphabet);
				free(alphabet);
			}
		}
		else if (strncmp(input, M_TOKEN_CDM, strlen(M_TOKEN_CDM)) == 0) {
			freeptr();
			int size = 0;
			char *alphabet = readfile(input + strlen(M_TOKEN_CDM), &size, 256);
			if (alphabet != NULL) {
				mem_tokenizer = mem_createTokenizer(alphabet);
				free(alphabet);
			}
		}
		else if (strncmp(input, PRNT_TOK_CMD, strlen(input)) == 0) {
			if (tokenizer != NULL) {
				printf("\n");
				printTokenizer(tokenizer);
			}
			else
				printf(NO_DATA_MSG);
		}
		else if (strncmp(input, M_PRNT_TOK_CMD, strlen(input)) == 0) {
			if (mem_tokenizer != NULL) {
				printf("\n");
				mem_printTokenizer(mem_tokenizer);
				printf("\n");
			}
			else
				printf(NO_DATA_MSG);
		}
		else if (strncmp(input, LEX_CMD, strlen(LEX_CMD)) == 0) {
			if (tokenStream != NULL) {
				destroyTokenStream(tokenStream);
				tokenStream = NULL;
			}
			unsigned int size = 0;
			char *lex = readfile(input + strlen(LEX_CMD), &size, 256);
			tokenStream = LexInput(lex, size, tokenizer);
			free(lex);
		}
		else if (strncmp(input, M_LEX_CMD, strlen(M_LEX_CMD)) == 0) {
			if (tokenStream != NULL) {
				destroyTokenStream(tokenStream);
				tokenStream = NULL;
			}
			unsigned int size = 0;
			char *lex = readfile(input + strlen(M_LEX_CMD), &size, 256);
			tokenStream = mem_LexInput(lex, size, mem_tokenizer);
			free(lex);
		}
		else if (strncmp(input, PRNT_LEX_CMD, strlen(input)) == 0) {
			if (tokenStream != NULL) {
				printf("\n");
				printTokenStream(tokenStream);
				printf("\n");
			}
			else
				printf(NO_DATA_MSG);
		}
		else if (strncmp(input, CLEAR_COMAND, strlen(input)) == 0) {
			clearscr();
		}
		else if (strncmp(input, EXIT_CMD, strlen(input)) == 0) {
			freeptr();
			exit(0);
		}
		else
			printf(NOT_RECOG_MSG);
	}
}

void getInput() 
{	
	memset(input, 0x00, BUF_SIZE);
	printf("\n$> ");
	fgets(input, BUF_SIZE - 1, stdin);
	for (int x = 0; x < BUF_SIZE - 1; x++) {
		if (isspace(*(input + x)) != 0) {
			for (int z = x; z < BUF_SIZE - 1; z++) {
				*(input + z) = *(input + (z + 1));
			}
			continue;
		}
		else if (*(input + x) == '\n') {
			*(input + x) = '\0';
			return;
		}
	}
}

void clearscr() 
{
	system(CLEAR_COMAND);
	printf(INIT_MSG);
}

void freeptr() {
	if (tokenizer != NULL) {
		destroyTokenizer(tokenizer);
		tokenizer = NULL;
	}
	if (tokenStream != NULL) {
		destroyTokenStream(tokenStream);
		tokenStream = NULL;
	}
}

char *readfile(char *filename, unsigned int *fsize, int sector) {
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