#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"

#define LIMIT_ONE ' '
#define LIMIT_TWO ','


struct Token {
	char *type;
	char *value;
};

int skipchar_two(char *ptr, char untilon, char untiltw, int limit);
struct Token createToken(char *type, char *value, int valsize);
struct Token tokenizeWord(char *word, int size, 
	struct LinkedToken *tokenizer);
struct Token *tokenizeAll(char *sentence, int sensize, int *size,
	struct LinkedToken *tokenizer);
	
	
void destroyTokenStream(struct Token *token, int size);
void debug_print_tokens(struct Token *tokens, int size);


#endif
