#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"
#include "Memory.h"
#include "Data.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

extern char *DOUBLE_ID;
extern char *INTEGER_ID;
extern char *STRING_ID;
extern char *CONST_STRING_ID;
extern char *MIX_STRING_ID;

extern char *ERR_ID;
extern char *EOF_ID;

#define BUILT_IN_AMNT 4
extern char **BUILT_IN_TYPES[];

#define DOUBLE_TYPE 2
#define INT_TYPE 1

#define SKIP_NULL_LIMIT(value, limit) while((*value != 0x00) &&\
						(value <= limit)) value++;

#define NULL_OR_LIMIT(value, limit) (*(value + 1) == 0x00) \
					|| (value == limit)

struct KiwiInput {
	char *text;
	uint32_t textSize;
	uint32_t readSize;
};

struct Token {
	char *type;
	char *value;
};

struct TokenArray {
	struct Token *token;
	uint16_t size;
};

struct LexerMemory {
	struct MemBlock *tokenTable;
	struct MemBlock *symbolTable;
};

int isNumber(char *val, int size);
int isString(char *val, int size);
int isLiteral(char *val, int size);
char *builtInMatch(char *value, int size);

int skipWhiteSpace(char **str, int limit);

struct Token newToken(char *type, char *value);
struct Token newValToken(char *type, char *value, int valsize,
	struct MemBlock *mem);
struct Token newTypeToken(char *type);
void printTokenStream(struct TokenArray *token, char format);
char *symbolTableContains(char *value, size_t size, struct MemBlock *mem);
char appendToken(struct TokenArray *stream, struct Token node,
	struct MemBlock *mem);
char *tokenOnlyMatch(char *word, int wrdize, 
	struct LinkList *tokenizer);
struct Token lexNext(struct KiwiInput *input,
	struct LinkList *tokenizer, struct MemBlock *mem);
struct TokenArray *lexAll(struct KiwiInput *input,
	struct LinkList *tok, struct MemBlock *tokenmem, 
	struct MemBlock *symbolmem);

#endif
