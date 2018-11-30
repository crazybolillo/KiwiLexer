#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"
#include <ctype.h>

extern char *DOUBLE_ID;
extern char *INTEGER_ID;
extern char *STRING_ID;
extern char *CONST_STRING_ID;
extern char *MIX_STRING_ID;

extern char *NO_MATCH_ERR;
extern char *EOF_ID;

#define DOUBLE_TYPE 2
#define INT_TYPE 1

struct KiwiInput {
	char *text;
	unsigned int textSize;
	unsigned int readSize;
};

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

int skipWhiteSpace(char **str, int limit);

struct Token newUnknownToken(char *value, int valsize);
struct Token newToken(char *type, char *value);
struct Token newValToken(char *type, char *value, int valsize);
struct Token newTypeToken(char *type);
void destroyToken(struct Token *token);
void destroyTokenStream(struct TokenStream *token);
void printTokenStream(struct TokenStream *token, char format);
void appendToken(struct TokenStream *stream, struct Token value);

struct Token mem_tokenOnlyMatch(char *word, int wrdize, 
	struct mem_LinkToken *tokenizer);
struct TokenStream *mem_lexAll(struct KiwiInput *input,
	struct mem_LinkToken *tok);
struct Token mem_lexNext(struct KiwiInput *input,
	struct mem_LinkToken *tokenizer);

#endif
