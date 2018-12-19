#ifndef TOKENIZER_HEADER_GUARD
#define TOKENIZER_HEADER_GUARD

#include "Memory.h"
#include "Data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TOKEN_LIMIT '"'
#define TOKEN_SEPARATOR ','


char *readAll(FILE *fl, unsigned int *sizeread, int sector);
int skipchar(char *ptr, char until, int limit);

struct LinkList *newLexeme(char *id, int idsize,
	struct MemBlock *mem);
struct LinkList *newAlphabet(char *grammar, int gramsize,
	struct MemBlock *mem);
char *alphabetContains(char *val, int valsz, 
	struct LinkList *tokenizer);
char *nul_alphabetContains(char *str, struct LinkList *tokenizer);
void printAlphabet(struct LinkList *tokenizer);

#endif
