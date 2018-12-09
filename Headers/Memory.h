#ifndef MEMORY_HEADER_GUARD
#define MEMORY_HEADER_GUARD
#include <stdlib.h>

struct MemBlock {
	char *memory;
	size_t memsize;
	size_t used;
};

struct MemBlock initMemory(char *ptr, size_t size);
void *kimalloc(size_t size, struct MemBlock *mem);
void *kicalloc(size_t size, struct MemBlock *mem);
void freeMemory(struct MemBlock *mem);
void popMemory(struct MemBlock *mem, size_t size);

#endif