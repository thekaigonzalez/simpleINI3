#include "IBuffer.h"

#include <stdio.h>
#include <string.h>

struct IBuffer
{
  IObject *blk;
  char *ptr;
  int size;
  int cap;
};

IBuffer *
IBufferNew (IObject *blk)
{
  IBuffer *buf
      = (IBuffer *)IMAlloc (IObjectGetMemoryPool (blk), sizeof (IBuffer));

  if (!buf)
    {
      printf ("%s: unable to allocate enough memory for buffer\n", __func__);
      return NULL;
    }

  buf->blk = (blk);

  buf->ptr = (char *)IMAlloc (IObjectGetMemoryPool (blk),
                              sizeof (char) * MEMORY_POOL_INCREMENT);
  buf->size = 0;
  buf->cap = MEMORY_POOL_INCREMENT;

  if (!buf->ptr)
    {
      printf ("%s: unable to allocate enough memory for buffer->ptr.\nyou "
              "probably should stop here.\n",
              __func__);
      IMClean ((IMemoryPool *)blk);
      return NULL;
    }

  return buf;
}

void
IBufferAppend (IBuffer *buf, char c)
{
  if (!buf)
    {
      return;
    }

  if (buf->size >= buf->cap)
    {
      buf->cap += MEMORY_POOL_INCREMENT;
      buf->ptr = (char *)IMRealloc (IObjectGetMemoryPool (buf->blk), buf->ptr,
                                    buf->cap * sizeof (char));
    }

  buf->ptr[buf->size] = c;
  buf->ptr[buf->size + 1] = 0;
  buf->size++;
}

int
IBufferGetSize (IBuffer *buf)
{
  if (!buf)
    {
      return -1;
    }
  return buf->size;
}

int
IBufferGetCapacity (IBuffer *buf)
{
  if (!buf)
    {
      return -1;
    }
  return buf->cap;
}

char
IBufferGet (IBuffer *buf, int index)
{
  if (!buf || index < 0 || index >= buf->size)
    {
      return 0;
    }
  return buf->ptr[index];
}

void
IBufferClear (IBuffer *buf)
{
  if (!buf)
    {
      return;
    }

  buf->size = 0;
  memset (buf->ptr, 0, buf->size * sizeof (char));
}

char *
IBufferGetBuffer (IBuffer *buf)
{
  if (!buf)
    {
      return NULL;
    }
  return buf->ptr;
}

char *
IBufferCopy (IBuffer *buf)
{
  if (!buf)
    {
      return NULL;
    }

  char *ptr = (char *)IMAlloc (IObjectGetMemoryPool (buf->blk), buf->size + 1);

  if (!ptr)
    {
      return NULL;
    }

  memcpy (ptr, buf->ptr, buf->size);
  ptr[buf->size] = 0;

  return ptr; /* magic memory woowwie */
}
