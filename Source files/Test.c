#include "Tokenizer.h"

void main()
{
    FILE *fl = fopen("grammarOne.txt", "r");
	if(fl == NULL){
		return;	
	}
	
    int size = 0; 
	char *grammar = readAll(fl, &size);
	fclose(fl);
	struct Token *tokenizer = createTokenizer(grammar);
    free(grammar);
	debug_print_token(tokenizer);
    destroyTokenizer(tokenizer);
}
