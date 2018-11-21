#ifndef TOKENIZER_HEADER_GUARD
#define TOKENIZER_HEADER_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
This header manages the LinkedLists that create the Tokenizer
the lexer depends on. It also contains the very simple methods
used to parse the alphabet into a LinkedList data structure. And yes,
I do know typedefs exist. 
*/

#define TOKEN_LIMIT '"'
#define TOKEN_SEPARATOR ','

struct mem_LinkToken {
	char *id;
	struct mem_LinkToken *next;
};

char *readAll(FILE *fl, unsigned int *sizeread, int sector);
int skipchar(char *ptr, char until, int limit);

struct mem_LinkToken *mem_newLinkToken(char *id, int idsize);
void mem_insertToken(struct mem_LinkToken *brnach, 
	struct mem_LinkToken **root);
struct mem_LinkToken *mem_createTokenizer(char *grammar, int gramsize);
void mem_printTokenizer(struct mem_LinkToken *tokenizer);
void mem_destroyTokenizer(struct mem_LinkToken *head);

#endif
