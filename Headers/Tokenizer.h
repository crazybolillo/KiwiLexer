#ifndef TOKENIZER_HEADER_GUARD
#define TOKENIZER_HEADER_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Header destined to manage the data structure the tokenizer depends on. LinkedLists.
Tokens consist of a "LinkedToken" with its ID, the next Token in the list and 
a "LinkedLexeme" which consists of the words or symbols which form part of that token. 

A "LinkedLexeme" consists of just a char pointer terminated with a NUL character, basically
a string and a pointer to the next LinkedChild in the list. 
*/

#define TOKEN_LIMIT '~'
#define TOKEN_SEPARATOR ','
#define TOKEN_END '`'

struct LinkedLexeme {
	char *value;
	struct LinkedLexeme *next;
};

struct LinkedToken {
	char *id;
	struct LinkedLexeme *sons;
	struct LinkedToken *next;
};

struct LinkedToken *createLinkedToken(char *id, int idsize);
struct LinkedLexeme *createLexeme(char *value);

int insertToken(struct LinkedToken *branch, struct LinkedToken *root);
int insertTokenValue(char *word, int wordsize, struct LinkedToken *branch);

char *readAll(FILE
	*fl, int *size);
int skipchar(char *ptr, char until);
struct LinkedToken *parseToken(char **grammar);
struct LinkedToken *createTokenizer(char *grammar);
void destroyTokenizer(struct LinkedToken *head);

void printTokenizer(struct LinkedToken *tokenizer);

#endif
