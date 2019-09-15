#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"
#include "Memory.h"
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

#define PROD_SIGNAL "->"
#define PROD_END ";"

extern struct AlphList dev_parsertok;
extern struct AlphList dev_alphparser;

#define BUILT_IN_AMNT 4
extern char **BUILT_IN_TYPES[];

#define DOUBLE_TYPE 2
#define INT_TYPE 1

#define SKIP_NUL_LIMIT(value, limit) while((*value != 0x00) &&\
						(value <= limit)) value++;

#define NUL_OR_LIMIT(value, limit) (*(value + 1) == 0x00) \
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

char isNumber(char *val, int size);
char isString(char *val, int size);
char isLiteral(char *val, int size);
char *dev_builtInMatch(char *value, int size);

int skipWhiteSpace(char **str, int limit);

struct Token dev_newValToken(char *type, char *value, int valsize,
	struct MemBlock *mem);
struct Token dev_newTypeToken(char *type);
void printTokenStream(struct TokenArray *token, char format);
char *dev_symbolTableContains(char *value, size_t size, 
	struct MemBlock *mem);
char dev_appendToken(struct TokenArray *stream, struct Token node,
	struct MemBlock *mem);
char *dev_tokenOnlyMatch(char *word, int wrdize, 
	struct AlphList *tokenizer);
char* dev_parsetokenOnlyMatch(char* word, int wrdsize,
	struct AlphList* tokenizer);
struct Token lexNext(struct KiwiInput *input,
	struct AlphList *tokenizer, struct MemBlock *mem, 
	char *(*matcher)(char *word, int wrdsize, struct AlphList *alph));
struct TokenArray *lexAll(struct KiwiInput *input,
	struct AlphList *tok, struct MemBlock *tokenmem, 
	struct MemBlock *symbolmem,
	char *(*matcher)(char *word, int wrdsize, struct AlphList *alph));

#endif
