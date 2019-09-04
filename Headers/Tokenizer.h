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

void dev_appendToAlph(struct AlphList *data, struct AlphList **head);
int skipchar(char *ptr, char until, int limit);
struct AlphList *dev_newLexeme(char *id, int idsize,
	struct MemBlock *mem);
struct AlphList *newAlphabet(char *grammar, int gramsize,
	struct MemBlock *mem);
char *dev_alphabetContains(char *str, struct AlphList *tokenizer);
void dev_printAlphabet(struct AlphList *tokenizer);

#endif
