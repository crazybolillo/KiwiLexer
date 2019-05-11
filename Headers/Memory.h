#ifndef MEMORY_HEADER_GUARD
#define MEMORY_HEADER_GUARD

#include <string.h>
#include <stdlib.h>

/*
No memory is allocated dinamically. All memory is declared as arrays
and initialized, assigned and cleared with the functions included in this
header. 
*/


/*
This struct is used all troughout the functions of this 
header. It keeps track of the memory size and memory usage.
*/
struct MemBlock {
	char *memory;
	size_t memsize;
	size_t used;
};

/*
Initializes a memory block. All memory blocks must be initalized before
being passed around in functions.
*/
void initMemory(struct MemBlock *mem, char *ptr, size_t size);

/*
Requests memory to be allocated from the memory block passed trough the 
parameters. Returns a NULL pointer if the memory could not be allocated 
otherwise it returns a pointer to the first element in the allocated block
of memory. Memory allocated is NOT cleaned so it may be full of trash. 
*/
void *kimalloc(size_t size, struct MemBlock *mem);

/*
Requests memory to be allocated from the memory block passed trough the 
parameters. Returns a NULL pointer if the memory could not be allocated 
otherwise it returns a pointer to the first element in the allocated block
of memory. Memory allocade is GUARANTEED to be set to 0x00.
*/
void *kicalloc(size_t size, struct MemBlock *mem);

/*
Resets the memory block so that memory can be reused. 
It does NOT delete any information stored.
*/
void freeMemory(struct MemBlock *mem);

/*
Resets the memory block so that memory can be reused. It does DELETE previous
information that was stored and sets everything to 0x00.
*/
void freeCleanMemory(struct MemBlock *mem);

/*
Resets part of the memory block from right to left (newest memory is freed
first). The amount of memory to be resetted has to be passed trough the 
parameters.
*/
void popMemory(size_t size, struct MemBlock *mem);

#endif