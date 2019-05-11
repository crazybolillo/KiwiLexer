#ifndef TOKENIZER_HEADER_GUARD
#define TOKENIZER_HEADER_GUARD

#include "Memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TOKEN_LIMIT '"'
#define TOKEN_SEPARATOR ','


/*
A linked list structure. Used to store the alphabet used by the lexer. Only
used by the tokenizer.
*/
struct AlphList {
	char *value;
	struct AlphList *next;
};


/*
Creates a list node. Sets its value to that passed on the parameters and 
its next node as a NULL pointer by default. 
*/
struct AlphList *newAlph(void *value, struct MemBlock *mem);

/*
Adds the node to the list. It adds it to the end of the list.
*/
void appendToAlph(struct AlphList *data, struct AlphList **head);

/*
Skips chars (moves the pointer) in a char array until it finds the char
passed trough the parameters.
*/
int skipchar(char *ptr, char until, int limit);

struct AlphList *newLexeme(char *id, int idsize,
	struct MemBlock *mem);
struct AlphList *newAlphabet(char *grammar, int gramsize,
	struct MemBlock *mem);
char *alphabetContains(char *val, int valsz, 
	struct AlphList *tokenizer);
char *nul_alphabetContains(char *str, struct AlphList *tokenizer);
void printAlphabet(struct AlphList *tokenizer);

#endif
