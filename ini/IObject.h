// $Id: IObject.h

#ifndef __IOBJECT_H
#define __IOBJECT_H

// defines objects

#include "IMemoryPool.h"

// objects contain their own memory pool

typedef struct IObject IObject;

// allocates a new object
IObject *IObjectNew ();

// accesses an object's memory pool
IMemoryPool *IObjectGetMemoryPool (IObject *o);

// frees an object, and its memory pool
void IObjectFree (IObject *o);

#endif
