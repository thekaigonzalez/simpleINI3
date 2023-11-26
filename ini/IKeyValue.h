// $Id: IKeyValue.h

#ifndef __IKEYVALUE_H
#define __IKEYVALUE_H

#include "IObject.h"
#include "IValue.h"

// defines key-value pairs

typedef struct IKeyValue IKeyValue;

// allocates a new key-value pair onto an object's memory pool
IKeyValue *IKeyValueNew (IObject *blk);

// set the key
void IKeyValueSetKey (IKeyValue *kv, char *key);

// set the value
void IKeyValueSetValue (IKeyValue *kv, IValue *value);

// get the key
char *IKeyValueGetKey (IKeyValue *kv);

// get the value
IValue *IKeyValueGetValue (IKeyValue *kv);

#endif
