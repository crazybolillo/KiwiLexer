#ifndef MEMORY_HEADER_GUARD
#define MEMORY_HEADER_GUARD
#include <stdlib.h>

struct MemBlock {
	char *memory;
	size_t memsize;
	size_t used;
};


void *kimalloc(size_t size, struct MemBlock *mem);
void *kicalloc(size_t size, struct MemBlock *mem);
void resetBlock(struct MemBlock *mem);

#endif