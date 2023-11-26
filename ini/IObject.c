#include "IObject.h"

struct IObject
{
  IMemoryPool *pool;
};

IObject *
IObjectNew ()
{
  IObject *obj = (IObject *)calloc (sizeof (IObject *), 1);
  if (!obj)
    return NULL;

  obj->pool = IMNew ();

  return obj;
}

IMemoryPool *
IObjectGetMemoryPool (IObject *o)
{
  if (!o || !o->pool)
    return NULL;

  return o->pool;
}

void
IObjectFree (IObject *o)
{
  if (!o)
    return;

  IMClean (o->pool);
  free (o);
}
