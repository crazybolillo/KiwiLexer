#ifndef TOKENIZER_HEADER_GUARD
#define TOKENIZER_HEADER_GUARD

#include "Memory.h"
#include "Data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_LIMIT '"'
#define TOKEN_SEPARATOR ','


char *readAll(FILE *fl, unsigned int *sizeread, int sector);
int skipchar(char *ptr, char until, int limit);

struct LinkList *mem_newLinkToken(char *id, int idsize,
	struct MemBlock *mem);
struct LinkList *mem_createTokenizer(char *grammar, int gramsize,
	struct MemBlock *mem);
char *contains(char *val, int valsz, struct LinkList *tokenizer);
void mem_printTokenizer(struct LinkList *tokenizer);

#endif
