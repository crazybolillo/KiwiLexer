#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"
#include "Memory.h"
#include "Data.h"
#include <ctype.h>

extern char *DOUBLE_ID;
extern char *INTEGER_ID;
extern char *STRING_ID;
extern char *CONST_STRING_ID;
extern char *MIX_STRING_ID;

extern char *NO_MATCH_ERR;
extern char *EOF_ID;

extern char **BUILT_IN_TYPES[];

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
	struct Token *next;
};

/*
Memory independent functions. 
*/
int isNumber(char *val, int size);
int isString(char *val, int size);
int isLiteral(char *val, int size);
char *builtInMatch(char *value, int size);

int skipWhiteSpace(char **str, int limit);

struct Token *newToken(char *type, char *value, struct MemBlock *mem);
struct Token *newValToken(char *type, char *value, int valsize,
	struct MemBlock *mem);
struct Token *newTypeToken(char *type, struct MemBlock *mem);
void printTokenStream(struct Token *token, char format);
void appendToken(struct Token **head, struct Token *node);

char *tokenOnlyMatch(char *word, int wrdize, 
	struct LinkList *tokenizer);
struct Token *lexNext(struct KiwiInput *input,
	struct LinkList *tokenizer, struct MemBlock *mem);
struct Token *lexAll(struct KiwiInput *input,
	struct LinkList *tok, struct MemBlock *mem);

#endif
