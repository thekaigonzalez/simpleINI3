#include "IMemoryPool.h"

#include <stdio.h>

struct IMemoryPool
{
  void **ptr; // keep track of memory pointers
  int size;   // number of pointers
  int cap;    // capacity
};

IMemoryPool *
IMNew ()
{
  IMemoryPool *pool = (IMemoryPool *)calloc (sizeof (IMemoryPool), 1);

  if (!pool)
    {
    death:
      printf ("%s: unable to allocate memory pool, probably out of memory\ni "
              "don't want to continue.\n",
              __func__);
      exit (1);
    }

  pool->size = 0;
  pool->ptr = (void **)calloc (sizeof (void *), MEMORY_POOL_INCREMENT);
  pool->cap = MEMORY_POOL_INCREMENT;

  if (!pool->ptr)
    goto death;

  return pool;
}

void *
IMAlloc (IMemoryPool *p, size_t nmemb)
{
  void *mem = malloc (nmemb);

  if (mem)
    {
      __IMAppendPointer (p, mem);
    }
  else
    {
      printf (
          "%s: unable to allocate memory\nit's not a good idea to go through "
          "with this\n",
          __func__);
      return NULL;
    }
  return mem;
}

void *
IMRealloc (IMemoryPool *p, void *ptr, int size)
{
  void *mem = realloc (ptr, size);

  if (mem)
    {
      __IMAppendPointer (p, mem);
    }

  return mem;
}

void
__IMAppendPointer (IMemoryPool *p, void *ptr)
{
  if (p)
    {
      if (p->size >= p->cap)
        {
          p->cap += MEMORY_POOL_INCREMENT;
          p->ptr = (void **)realloc (p->ptr, sizeof (void *) * p->cap);

          if (!p->ptr)
            {
              printf ("%s: unable to reallocate memory pool\nyou probably "
                      "should stop "
                      "using this.\n",
                      __func__);
            }
        }

      p->ptr[p->size] = ptr;
      p->size++;
    }
}

void
IMClean (IMemoryPool *p)
{
  if (p)
    {
      for (int i = 0; i < p->size; i++)
        {
          free (p->ptr[i]);
        }

      free (p->ptr);
      free (p);
    }
}
