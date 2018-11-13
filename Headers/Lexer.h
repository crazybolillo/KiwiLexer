#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"

#define SPACE ' '
#define TOKEN_SEP_COMMA ','
#define TOKEN_SEP_TAB '\t'
#define TOKEN_SEP_NLINE '\n'


struct TokenVal {
	char *type;
	char *value;
};

int skipUntilLimit(char *ptr, int limit);
struct TokenVal createTokenVal(char *type, char *value, int valsize);
struct TokenVal tokenizeWord(char *word, int size, 
	struct Tokenizer tokenizer);
struct TokenVal *tokenizeAll(char *sentence, int sensize, int *size,
	struct Tokenizer tokenizer);
	
	
void destroyTokenStream(struct TokenVal *token, int size);
void debug_print_tokens(struct TokenVal *tokens, int size);


#endif
