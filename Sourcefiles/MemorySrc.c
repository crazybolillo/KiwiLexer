#include "Memory.h"

/*
Tries to add the value to memory. Returns a pointer to the starting
index of the allocated value or NULL if the allocation failed.
*/
void *kimalloc(size_t size, struct MemBlock *mem)
{
	if (size + mem->used >= mem->memsize) {
		return NULL;
	}
	else {
		void *retval = mem->memory;
		mem->used += size;
		mem->memory += size + 1;
		return retval;
	}
}

void *kicalloc(size_t size, struct MemBlock *mem)
{
	if (size + mem->used >= mem->memsize) {
		return NULL;
	}
	else {
		void *retval = mem->memory;
		for (size_t x = 0; x < size; x++, mem->memory++) {
			*mem->memory = 0x00;
		}
		mem->memory++;
		mem->used += size;
		return retval;
	}
}

/*
Resets the byte offset to zero to override current data inside 
the memory.
*/
void resetBlock(struct MemBlock *mem) {
	mem->memory -= mem->used;
	mem->used = 0;
}