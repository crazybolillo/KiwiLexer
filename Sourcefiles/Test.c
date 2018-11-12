#include "Tokenizer.h"
#include "Lexer.h"

void main()
{
	FILE *fl = fopen("grammarOne.txt", "r");
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
	char *testString = "windy, dog, taco   , blue, hello,sunny, cloudy, c,\
	\t unknown, \t\t\n rainy";
	struct Token *tokenstream = tokenizeAll(testString, strlen(testString), &toksize,
		tokenizer);

	debug_print_tokens(tokenstream, toksize);

	destroyTokenizer(tokenizer);
	destroyTokenStream(tokenstream, toksize);
}
