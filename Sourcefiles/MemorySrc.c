#include "Memory.h"

struct MemBlock initMemory(char *ptr, size_t size)
{
	struct MemBlock retval;
	retval.memory = ptr;
	retval.memsize = size;
	retval.used = 0;
	return retval;
}

/*
Returns a pointer to the starting index of the allocated 
value or NULL if the allocation failed.
*/
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

/*
Returns a pointer to the starting index of the allocated value and
sets all the memory to 0x00 or returns NULL if the allocation failed.
*/
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

/*
Resets the byte offset to zero to override current data inside 
the memory.
*/
void freeMemory(struct MemBlock *mem) 
{
	mem->memory -= mem->used;
	mem->used = 0;
}

/*
Frees the last element that was allocated.
*/
void popMemory(struct MemBlock *mem, size_t size)
{
	if ((size > mem->memsize) || (size > mem->used)) {
		freeMemory(mem);
	}
	else if (size > 0) {
		mem->memory -= (size + 1);
		mem->used -= size;
	}
}