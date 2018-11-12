#include "Tokenizer.h"
#include "Lexer.h"

void main()
{
	FILE *fl = fopen("C:/Users/Antonio/source/repos/KiwiLexer/KiwiLexer/KiwiLexer/KiwiLexer/Sourcefiles/grammarOne.txt", "r");
	if(fl == NULL){
		return;	
	}
	
    int size = 0; 
	char *grammar = readAll(fl, &size);
	fclose(fl);
	struct LinkedToken *tokenizer = createTokenizer(grammar);
    free(grammar);
	debug_print_token(tokenizer);

	int toksize = 0;
	char *testString = "\t\ttaco,\nrainy bluekiwi, hello, cloudy \t rainy";
	struct Token *tokenstream = tokenizeAll(testString, strlen(testString), &toksize,
		tokenizer);

	debug_print_tokens(tokenstream, toksize);

	destroyTokenizer(tokenizer);
	destroyTokenStream(tokenstream, toksize);
}
