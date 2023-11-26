#include "IKeyValue.h"

struct IKeyValue
{
  IObject *blk;
  char *key;
  IValue *value;
};

IKeyValue *
IKeyValueNew (IObject *blk)
{
  IKeyValue *kv
      = (IKeyValue *)IMAlloc (IObjectGetMemoryPool (blk), sizeof (IKeyValue));

  kv->blk = (blk);

  return kv;
}

void
IKeyValueSetKey (IKeyValue *kv, char *key)
{
  if (!kv || !key)
    return;
  kv->key = key;
}

void
IKeyValueSetValue (IKeyValue *kv, IValue *value)
{
  if (!kv || !value)
    return;
  kv->value = value;
}

char *
IKeyValueGetKey (IKeyValue *kv)
{
  if (!kv)
    return NULL;
  return kv->key;
}

IValue *
IKeyValueGetValue (IKeyValue *kv)
{
  if (!kv || !kv->value)
    return NULL;
  return kv->value;
}
