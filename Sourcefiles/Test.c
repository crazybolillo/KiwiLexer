#include "Tokenizer.h"
#include "Lexer.h"

void main()
{
	FILE *fl = fopen("KiwiLexer/Sourcefiles/grammarOne.txt", "r");
	if(fl == NULL){
		return;	
	}
	
    int size = 0; 
	char *grammar = readAll(fl, &size);
	fclose(fl);
	struct Tokenizer *tokenizer = createTokenizer(grammar);
    free(grammar);
	printTokenizer(tokenizer);


	int toksize = 0;
	char *testString = "\t\t20 + \n50 * 10 \n\n + 30 / 10";
	struct TokenVal *tokenstream = tokenizeAll(testString, strlen(testString), &toksize,
		*tokenizer);

	debug_print_tokens(tokenstream, toksize);

	destroyTokenizer(tokenizer);
	destroyTokenStream(tokenstream, toksize);
}
