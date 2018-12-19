#ifndef MEMORY_HEADER_GUARD
#define MEMORY_HEADER_GUARD

#include <string.h>
#include <stdlib.h>

struct MemBlock {
	char *memory;
	size_t memsize;
	size_t used;
};


struct KiwiMemory {
	struct LinkList *tokenizer;
	struct LexMemBlock *lexer;
	struct MemBlock *parser;
};

void initMemory(struct MemBlock *mem, char *ptr, size_t size);
void *kimalloc(size_t size, struct MemBlock *mem);
void *kicalloc(size_t size, struct MemBlock *mem);
void freeMemory(struct MemBlock *mem);
void freeCleanMemory(struct MemBlock *mem);
void popMemory(size_t size, struct MemBlock *mem);

#endif