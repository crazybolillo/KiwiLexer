#include "Memory.h"

/*
Initializes a memory block. All memory blocks must be initalized before
being passed around in functions.
*/
void initMemory(struct MemBlock *mem, char *ptr, size_t size)
{
	mem->memory = ptr;
	mem->memsize = size;
	mem->used = 0;
}

/*
Requests memory to be allocated from the memory block passed trough the
parameters. Returns a NULL pointer if the memory could not be allocated
otherwise it returns a pointer to the first element in the allocated block
of memory. Memory allocated is NOT cleaned so it may be full of trash.
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
Requests memory to be allocated from the memory block passed trough the
parameters. Returns a NULL pointer if the memory could not be allocated
otherwise it returns a pointer to the first element in the allocated block
of memory. Memory allocade is GUARANTEED to be set to 0x00.
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
Resets the memory block so that memory can be reused.
It does NOT delete any information stored.
*/
void freeMemory(struct MemBlock *mem) 
{
	mem->memory -= mem->used;
	mem->used = 0;
}

/*
Resets the memory block so that memory can be reused. It does DELETE previous
information that was stored and sets everything to 0x00.
*/
void freeCleanMemory(struct MemBlock *mem)
{
	memset(mem->memory, 0x00, mem->used);
	freeMemory(mem);
}

/*
Resets part of the memory block from right to left (newest memory is freed
first). The amount of memory to be resetted has to be passed trough the
parameters.
*/
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
