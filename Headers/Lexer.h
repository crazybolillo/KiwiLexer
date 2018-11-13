#ifndef LEXER_HEADER_GUARD
#define LEXER_HEADER_GUARD

#include "Tokenizer.h"

#define SPACE ' '
#define COMMA ','
#define TAB '\t'
#define NLINE '\n'


struct Token {
	char *type;
	char *value;
};

struct TokenStream {
	struct Token *tokens;
	int size;
};

int skipUntilLimit(char *ptr, int limit);
struct Token createToken(char *type, char *value, int valsize);
struct Token tokenizeWord(char *word, int size, 
	struct LinkedToken *tokenizer);
struct Token *tokenizeAll(char *sentence, int sensize, int *size,
	struct LinkedToken *tokenizer);
	
	
void destroyTokenStream(struct Token *token, int size);
void printTokenStream(struct Token *tokens, int size);


#endif
