#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"
#include <ctype.h>

#define SPACE ' '
#define COMMA ','
#define TAB '\t'
#define NLINE '\n'

extern char *DOUBLE_ID;
extern char *INT_ID;
extern char *STRING_ID;
extern char *MIX_STRING_ID;


#define DOUBLE_TYPE 2
#define INT_TYPE 1

struct Token {
	char *type;
	char *value;
};

struct TokenStream {
	struct Token *tokens;
	int size;
};

/*
Memory independent functions. 
*/
int isNumber(char *val, int size);
int isString(char *val, int size);

int skipUntilLimit(char *ptr, int limit);
struct Token newUnknownToken(char *value, int valsize);
struct Token newToken(char *type, char *value);
struct Token newValToken(char *type, char *value, int valsize);
struct Token newTypeToken(char *type);
void destroyTokenStream(struct TokenStream *token);
void printTokenStream(struct TokenStream *token);

/*More memory consuming functions where the tokenizer can have lexemes.*/
struct Token lexWord(char *word, int size,  struct LinkToken *tokenizer);
struct TokenStream *LexInput(char *sentence, int sensize, 
	struct LinkToken *tokenizer);

/*Less memory consuming functions where the tokenizer does not have lexemes.*/
struct Token mem_lexWord(char *word, int size, 
	struct mem_LinkToken *tokenizer);
struct TokenStream *mem_LexInput(char *sentence, int sensize, 
	struct mem_LinkToken *tokenizer);


#endif
