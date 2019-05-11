#include "Memory.h"

void initMemory(struct MemBlock *mem, char *ptr, size_t size)
{
	mem->memory = ptr;
	mem->memsize = size;
	mem->used = 0;
}

void *kimalloc(size_t size, struct MemBlock *mem)
{
	if (size + mem->used > mem->memsize) {
		return NULL;
	}
	else {
		void *retval = mem->memory;
		mem->used += size;
		mem->memory += size;
		return retval;
	}
}

void *kicalloc(size_t size, struct MemBlock *mem)
{
	if (size + mem->used > mem->memsize) {
		return NULL;
	}
	else {
		void *retval = mem->memory;
		for (size_t x = 0; x < size; x++, mem->memory++) {
			*mem->memory = 0x00;
		}
		mem->used += size;
		return retval;
	}
}

void freeMemory(struct MemBlock *mem) 
{
	mem->memory -= mem->used;
	mem->used = 0;
}

void freeCleanMemory(struct MemBlock *mem)
{
	memset(mem->memory, 0x00, mem->used);
	freeMemory(mem);
}

void popMemory(size_t size, struct MemBlock *mem)
{
	if ((size > mem->memsize) || (size > mem->used)) {
		freeMemory(mem);
	}
	else if (size > 0) {
		mem->memory -= size;
		mem->used -= size;
	}
}
