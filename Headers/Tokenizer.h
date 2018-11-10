#ifndef TOKENIZER_HEADER_GUARD
#define TOKENIZER_HEADER_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/*
Header destined to manage the data structure the tokenizer depends on. LinkedLists.
Tokens consist of a "LinkedParent" with its ID, the next Token in the list and 
a "LinkedChild" which consists of the words or symbols which form part of that token. 

A "LinkedChild" consists of just a char pointer terminated with a NUL character, basically
a string and a pointer to the next LinkedChild in the list. 

LinkedLists were chosen since they allow for easy looping and insertion of new data.
*/

struct TokenValue {
	char *value;
	struct TokenValue *next;
};

struct Token {
	char *id;
	struct TokenValue *sons;
	struct Token *next;
};

struct Token *createToken(char *id, int idsize);

int tokenIndex(char *id, int idsize, struct Token root);
int insertToken(struct Token *branch, struct Token *root);

int insertTokenValue(char *word, int wordsize, struct Token *branch);

char *readAll(FILE *fl, int *size);
int skipchar(char *ptr, char until);
struct Token *createTokenizer(char *grammar);
void destroyTokenizer(struct Token *head);

void debug_print_token(struct Token *tokenizer);

#endif
