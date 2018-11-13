#include "Tokenizer.h"
#include "Lexer.h"

void main()
{

	FILE *fl = fopen("KiwiLexer/Sourcefiles/grammarOne.txt", "r");
	if(fl == NULL){
		return;	
	}
	
    int size = 0; 
	char *grammar = readAll(fl, &size, 256);
	fclose(fl);
	struct LinkedToken *tokenizer = createTokenizer(grammar);
    free(grammar);


	int toksize = 0;
	char *testString = "\t\t20 + \n50 * 10 \n\n + 30 / 10";
	struct Token *tokenstream = tokenizeAll(testString, strlen(testString), &toksize,
		tokenizer);

	printTokenizer(tokenizer);
	printTokenStream(tokenstream, toksize);
	destroyTokenizer(tokenizer);
	destroyTokenStream(tokenstream, toksize);
}
