#include "ISection.h"

#include <stdio.h>
#include <string.h>

struct ISection
{
  IObject *blk;     // parent
  IKeyValue **list; // list of key-value pairs
  int size;         // number of key-value pairs
  int cap;          // capacity

  char *hdr; // section header
};

ISection *
ISectionNew (IObject *blk)
{
  ISection *sec
      = (ISection *)IMAlloc (IObjectGetMemoryPool (blk), sizeof (ISection));

  if (!sec)
    {
      printf ("%s: unable to allocate enough memory for section\n", __func__);
      return NULL;
    }

  sec->blk = (blk);
  sec->list = IMAlloc (IObjectGetMemoryPool (blk),
                       sizeof (IKeyValue *) * MEMORY_POOL_INCREMENT);

  if (!sec->list)
    {
      printf ("%s: unable to allocate enough memory for sec->list, sec->blk "
              "exhausted\n",
              __func__);
      return NULL;
    }
  sec->size = 0;
  sec->cap = MEMORY_POOL_INCREMENT;

  sec->hdr = NULL;

  return sec;
}

void
ISectionAppend (ISection *sec, IKeyValue *kv)
{
  if (!sec || !kv)
    {
      return;
    }

  if (sec->size >= sec->cap)
    {
      int previous_capacity = sec->cap;
      sec->cap += MEMORY_POOL_INCREMENT;
      sec->list = IMRealloc (IObjectGetMemoryPool (sec->blk), sec->list,
                             sizeof (IKeyValue *) * sec->cap);

      if (!sec->list)
        {
          printf ("%s: unable to reallocate memory pool\nyou probably should "
                  "stop.\n(note) previous size = %d",
                  __func__, previous_capacity);
          return;
        }
    }

  sec->list[sec->size] = kv;
  sec->size++;
}

void
ISectionSetHeader (ISection *sec, char *hdr)
{
  if (!sec)
    return;

  /* note: copy the header to prevent pointer weirdness */

  char *hdr_cp
      = (char *)IMAlloc (IObjectGetMemoryPool (sec->blk), strlen (hdr) + 1);

  strcpy (hdr_cp, hdr);

  sec->hdr = hdr_cp;
}

char *
ISectionHeader (ISection *sec)
{
  if (!sec)
    {
      return NULL;
    }
  return sec->hdr;
}

void
ISectionPrint (ISection *sec, char separator)
{
  if (!sec)
    {
      return;
    }

  if (sec->hdr)
    {
      printf ("%s\n", sec->hdr);
    }

  for (int i = 0; i < sec->size; i++)
    {
      printf ("  %s %c ", IKeyValueGetKey (sec->list[i]), separator);
      IValuePrintAsString (IKeyValueGetValue (sec->list[i]));
    }
}
