// $Id: IMemoryPool.h

#ifndef __IMEMORYPOOL_H
#define __IMEMORYPOOL_H

// defines memory pooling system

#define MEMORY_POOL_INCREMENT 45

#include <stdlib.h>

typedef struct IMemoryPool IMemoryPool;

// allocates a new memory pool, default set to MEMORY_POOL_INCREMENT
// note for future usage - do NOT use multiple memory pools, share one across
// multiple objects
IMemoryPool *IMNew ();

// allocates memory and adds it to the memory pool.
void *IMAlloc (IMemoryPool *p, size_t nmemb);

// reallocates PTR and adds it to the memory pool, to save time and headaches
// this function does not free NOR does it equal-override the existing pointer,
// instead creates a new one thats the reallocated block.
void *IMRealloc (IMemoryPool *p, void *ptr, int size);

// hidden function to add a pointer to the memory pool, the average caller
// should not use this unless they're a developer for this section.
void __IMAppendPointer (IMemoryPool *p, void *ptr);

// goes through the memory pool frees all pointers
void IMClean (IMemoryPool *p);

#endif
