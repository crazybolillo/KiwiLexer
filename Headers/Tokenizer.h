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
#define TOKEN_END '`'

/*
Data structure for alphabets where Tokens can have
several lexemes. Each Token has its own LinkedList
of lexemes.
*/
struct LinkLex {
	char *value;
	struct LinkLex *next;
};

struct LinkToken {
	char *id;
	struct LinkLex *sons;
	struct LinkToken *next;
};

/*
Memory efficient data model used for alphabets where
Tokens are their own Lexems. This is the case  for alphabets 
like the C language where keywords are their own tokens. All 
methods that manage these memory efficient data models have "mem"
as a prefix. This struct saves 8-4 bytes per node depending on the
pointer size the platform uses.
*/
struct mem_LinkToken {
	char *id;
	struct mem_LinkToken *next;
};

char *readAll(FILE *fl, unsigned int *sizeread, int sector);
int skipchar(char *ptr, char until);

//General functions. 
struct LinkToken *newLinkToken(char *id, int idsize);
void insertToken(struct LinkToken *branch, struct LinkToken **root);
void insertLexeme(char *word, int wordsize, struct LinkToken *branch);
struct LinkToken *parseToken(char **grammar);
struct LinkToken *createTokenizer(char *grammar);
void destroyTokenizer(struct LinkToken *head);
void printTokenizer(struct LinkToken *tokenizer);

//More memory efficient versions of previous functions.
struct mem_LinkToken *mem_newLinkToken(char *id, int idsize);
void mem_insertToken(struct mem_LinkToken *brnach, 
	struct mem_LinkToken **root);
struct mem_LinkToken *mem_parseToken(char **grammar);
struct mem_LinkToken *mem_createTokenizer(char *grammar);
void mem_printTokenizer(struct mem_LinkToken *tokenizer);
void mem_destroyTokenizer(struct mem_LinkToken *head);

#endif
