#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"
#include <ctype.h>

#define SPACE ' '
#define COMMA ','
#define TAB '\t'
#define NLINE '\n'

#define DOUBLE_ID "DBL"
#define INT_ID "INT"
#define STRING_ID "STR"
#define MIXED_STRING_ID "MIX"

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

int skipUntilLimit(char *ptr, int limit);
struct Token createToken(char *type, char *value, int valsize);
struct Token createTypeToken(char *type);
struct Token tokenizeWord(char *word, int size, 
	struct LinkedToken *tokenizer);
struct TokenStream *tokenizeAll(char *sentence, int sensize,
	struct LinkedToken *tokenizer);

int isNumber(char *val, int size);
int isString(char *val, int size);
	
	
void destroyTokenStream(struct TokenStream *token);
void printTokenStream(struct TokenStream *token);


#endif
