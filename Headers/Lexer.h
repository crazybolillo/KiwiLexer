#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"
#include <ctype.h>

extern char *DOUBLE_ID;
extern char *INT_ID;
extern char *STRING_ID;
extern char *LIT_STRING_ID;
extern char *MIX_STRING_ID;

extern char *NO_MATCH_ERR;

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
int isLiteral(char *val, int size);

struct Token newUnknownToken(char *value, int valsize);
struct Token newToken(char *type, char *value);
struct Token newValToken(char *type, char *value, int valsize);
struct Token newTypeToken(char *type);
void destroyToken(struct Token *token);
void destroyTokenStream(struct TokenStream *token);
void printTokenStream(struct TokenStream *token, char format);

/*Less memory consuming functions where the tokenizer does not have lexemes.*/
struct Token mem_tokenOnlyMatch(char *stream, int strsize, struct mem_LinkToken *tokenizer);
struct TokenStream *mem_lexInput(char *word, int wrdsize,
	struct mem_LinkToken *tok);


#endif
