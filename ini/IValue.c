#include "IValue.h"
#include "IBuffer.h"
#include "IMemoryPool.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct IValueList
{
  IValue **ptr;
  int size;
  int cap;
  IMemoryPool *blk;
};

struct IValue
{
  union
  {
    float number;
    float numberf;
    char *string;
    IValueList *list;
    char *id;
  };

  IValueKind kind;
};

IValue *
IValueNew (IObject *blk)
{
  IValue *v = (IValue *)IMAlloc (IObjectGetMemoryPool (blk), sizeof (IValue));

  v->kind = Number;

  return v;
}

IValue *
IValueFromString (IObject *blk, char *str)
{
  IValue *v = IValueNew (blk);

  // lexically analyze string
  if (str[0] == '(')
    {
      v->kind = List;
      v->list = IValueListNew (blk);

      int depth = 0;
      IBuffer *buf = IBufferNew (blk);

      for (int i = 1; i < strlen (str); ++i)
        {
          printf ("depth = %d\n", depth);
          if (str[i] == '(')
            {
              ++depth;
              IBufferAppend (buf, str[i]);
            }
          else if (str[i] == ')' && depth > 0)
            {
              --depth;
              IBufferAppend (buf, str[i]);
            }
          else if (str[i] == ',' && depth == 0 && str[i - 1] != '\\')
            {
              IValueListAppend (v->list,
                                IValueFromString (blk, IBufferCopy (buf)));
              IBufferClear (buf);
            }
          else if (str[i] == ')' && depth == 0)
            {
              IValueListAppend (v->list,
                                IValueFromString (blk, IBufferCopy (buf)));

              IBufferClear (buf);
            }
          else
            {
              if (!isspace (str[i]))
                {
                  IBufferAppend (buf, str[i]);
                }
            }
        }

      return v;
    }
  else if (str[0] == '\\')
    { // an interpolation \(previous_value)
      v->kind = Interpolation;
      v->string = str;

      return v;
    }
  else if (str[0] == '"')
    {
      v->kind = String;
      v->string = str;

      return v;
    }
  else if (isdigit (str[0]))
    {
      v->kind = Number;
      v->number = strtof (str, NULL);

      return v;
    }
  else
    {
      v->kind = Identifier;
      v->id = str;

      return v;
    }

  return v;
}

IValueList *
IValueListNew (IObject *blk)
{
  IValueList *vl = (IValueList *)IMAlloc (IObjectGetMemoryPool (blk),
                                          sizeof (IValueList));

  vl->ptr = (IValue **)IMAlloc (IObjectGetMemoryPool (blk),
                                sizeof (IValue *) * MEMORY_POOL_INCREMENT);

  vl->size = 0;
  vl->cap = MEMORY_POOL_INCREMENT;

  return vl;
}

void
IValueListAppend (IValueList *vl, IValue *v)
{
  if (!vl || !v)
    {
      return;
    }

  if (vl->size >= vl->cap)
    {
      int previous_capacity = vl->cap;
      vl->cap += MEMORY_POOL_INCREMENT;
      vl->ptr = (IValue **)IMRealloc ((vl->blk), vl->ptr,
                                      sizeof (IValue) * vl->cap);
      if (!vl->ptr)
        {
          printf ("%s: unable to reallocate memory pool\nyou probably should "
                  "stop.\n(note) previous size = %d",
                  __func__, previous_capacity);
          return;
        }
    }

  vl->ptr[vl->size] = v;
  vl->size++;
}

IValueKind
IValueGetKind (IValue *v)
{
  if (!v)
    {
      return Nil;
    }

  return v->kind;
}

int
IValueToInt (IValue *v)
{
  if (!v)
    return -1;

  return v->number;
}

float
IValueToFloat (IValue *v)
{
  if (!v || !(v->kind == Number || v->kind == Float))
    return -1.0f;
  return v->numberf;
}

char *
IValueToString (IValue *v)
{
  if (!v)
    return NULL;
  return v->string;
}

char *
IValueToIdentifier (IValue *v)
{
  if (!v)
    return NULL;
  return v->id;
}

IValueList *
IValueToList (IValue *v)
{
  if (!v)
    return NULL;
  return v->list;
}

void
IValuePrintAsString (IValue *v)
{
  if (!v)
    {
      printf ("NULL Value\n");
      return;
    }

  switch (v->kind)
    {
    case Number:
      printf ("%f\n", v->number);
      break;
    case String:
      printf ("%s\n", v->string);
      break;
    case Identifier:
      printf ("%s\n", v->id);
      break;
    case List:
      printf ("<list>\n");
      for (int i = 0; i < v->list->size; ++i)
        {
          IValuePrintAsString (v->list->ptr[i]);
        }
      break;
    case Interpolation:
      printf ("<ref %s>\n", v->string);
      break;
    default:
      printf ("Unknown value\n");
      break;
    }
}
