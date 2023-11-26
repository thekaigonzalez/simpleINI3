// $Id: IBuffer.h

#ifndef __IBUFFER_H
#define __IBUFFER_H

// defines a simple list of characters that can grow and shrink as needed,
// primarily for parsing purposes

#include "IMemoryPool.h"
#include "IObject.h"

typedef struct IBuffer IBuffer;

// allocates a new buffer
IBuffer *IBufferNew (IObject *blk);

// appends a character to the buffer
void IBufferAppend (IBuffer *buf, char c);

// returns the size of the buffer
int IBufferGetSize (IBuffer *buf);

// returns the capacity of the buffer
int IBufferGetCapacity (IBuffer *buf);

// returns the buffer at the given index
char IBufferGet (IBuffer *buf, int index);

// clears the buffer
void IBufferClear (IBuffer *buf);

// returns a pointer to the buffer
char *IBufferGetBuffer (IBuffer *buf);

// returns a copy of the buffer
char *IBufferCopy (IBuffer *buf);

#endif
