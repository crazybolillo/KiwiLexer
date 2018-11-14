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
exit --> Exits the shell.\n"
#define FILE_ERR_MSG "\nCould not open file.\n"
#define NO_DATA_MSG "\nNothing to print...\n"
#define NOT_RECOG_MSG "\nCommand not recognized\n"

#define HELP_CMD "help"
#define TOKEN_CMD "tokenize"
#define PRNT_TOK_CMD "printtok"
#define LEX_CMD "lex"
#define PRNT_LEX_CMD "printlex"
#define EXIT_CMD "exit"


#define BUF_SIZE 156

static char input[BUF_SIZE];
static struct TokenStream *tokenStream = NULL;
static struct LinkedToken *tokenizer = NULL;

void getInput(void);
void clearscr(void);
void freeptr(void);

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
			FILE *fl = fopen(input + strlen(TOKEN_CMD), "r");
			if (fl != NULL) {
				int size = 0;
				char *alphabet = readAll(fl, &size, 256);
				tokenizer = createTokenizer(alphabet);
				free(alphabet);
				fclose(fl);
			}
			else
				printf(FILE_ERR_MSG);
		}
		else if (strncmp(input, PRNT_TOK_CMD, strlen(input)) == 0) {
			if (tokenizer != NULL) {
				printf("\n");
				printTokenizer(tokenizer);
			}
			else
				printf(NO_DATA_MSG);
		}
		else if (strncmp(input, LEX_CMD, strlen(LEX_CMD)) == 0) {
			if (tokenStream != NULL) {
				destroyTokenStream(tokenStream);
				tokenStream = NULL;
			}
			FILE *fl = fopen(input + strlen(LEX_CMD), "r");
			if (fl != NULL) {
				unsigned int size = 0;
				char *lex = readAll(fl, &size, 256);
				tokenStream = tokenizeAll(lex, size, tokenizer);
				free(lex);
				fclose(fl);
				continue;
			}
			printf(FILE_ERR_MSG);
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
