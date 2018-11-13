#ifndef TOKENIZER_HEADER_GUARD
#define TOKENIZER_HEADER_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TOKEN_LIMIT '~'
#define TOKEN_SEP_COMMA ','
#define TOKEN_END '`'


struct Token {
	char *id;
	int size;
	char **words;
};

struct Tokenizer {
	int tokenCount;
	struct Token *tokens;
};

struct Tokenizer *createTokenizer(char *grammar);
struct Token createToken(char *id, int idsize);
struct Token parseToken(char **grammar);
void addWord(struct Token *token, char *lex, int lexsize);
void destroyTokenizer(struct Tokenizer *tokenizer);
void printTokenizer(struct Tokenizer *tokenizer);
char *readAll(FILE *fl, int *size);
int skipchar(char *ptr, char until);

#endif
