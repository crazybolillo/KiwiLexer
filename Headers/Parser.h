#include "Lexer.h"
#include "Tokenizer.h"
#include "Memory.h"


struct Production {
	char *name;
	char **rules;
	struct Production *next;
	uint8_t rulesize;
};

struct Match {
	char *id;
	uint8_t size;
};

struct Production *dev_newProHead(char *prodname, int namesize,
	struct MemBlock *mem);
int dev_addRule(char *val, struct MemBlock *mem, 
	struct Production *prod);
void dev_addProduction(struct Production **head, 
	struct Production *node);
struct Production *dev_newProduction(struct KiwiInput *input,
	struct AlphList *alphabet, struct MemBlock *parsemem, 
	struct MemBlock *lexmem);
struct Production *newParser(struct KiwiInput *input,
	struct AlphList *alphabet, struct MemBlock *parsemem,
	struct MemBlock *symbolmem);
struct Match parseNext(struct Production *parser, 
	struct TokenArray *tokens);