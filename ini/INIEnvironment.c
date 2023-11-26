#include "INIEnvironment.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct INIEnvironment
{
  IObject *root;

  ISection **list;
  int size;
  int cap;
};

INIEnvironment *
INIEnvironmentNew ()
{
  IObject *root = IObjectNew ();
  INIEnvironment *env = (INIEnvironment *)IMAlloc (IObjectGetMemoryPool (root),
                                                   sizeof (INIEnvironment));

  env->root = root;
  env->size = 0;
  env->cap = MEMORY_POOL_INCREMENT;
  env->list
      = (ISection **)IMAlloc (IObjectGetMemoryPool (root),
                              sizeof (ISection *) * MEMORY_POOL_INCREMENT);

  return env;
}

void
INIEnvironmentAppendSection (INIEnvironment *env, ISection *sec)
{
  if (!env)
    {
      printf ("%s: can't append section to null environment\n", __func__);
      return;
    }

  if (env->size >= env->cap)
    {
      env->cap += MEMORY_POOL_INCREMENT;
      env->list
          = (ISection **)realloc (env->list, sizeof (ISection *) * env->cap);
    }

  env->list[env->size] = sec;
  env->size++;
}

char *
ISectionGetHeader (ISection *sec)
{
  if (!sec)
    {
      return NULL;
    }

  return ISectionHeader (sec);
}

ISection *
INIEnvironmentGetSection (INIEnvironment *env, char *name)
{
  if (!env)
    {
      return NULL;
    }

  for (int i = 0; i < env->size; i++)
    {
      if (strcmp (ISectionGetHeader (env->list[i]), name) == 0)
        {
          return env->list[i];
        }
    }
}

IObject *
INIEnvObject (INIEnvironment *env)
{
  return (env->root);
}

void
INIEnvironmentPrint (INIEnvironment *env)
{
  if (!env)
    {
      return;
    }
  printf ("ini environment with %d section(s)\n<==>\n", env->size);

  for (int i = 0; i < env->size; i++)
    {
      ISectionPrint (env->list[i], '=');
    }
}

ISection *
INIEnvironmentAddSection (INIEnvironment *env, char *name)
{
  ISection *sec = ISectionNew (env->root);

  ISectionSetHeader (sec, name);

  INIEnvironmentAppendSection (env, sec);

  return sec;
}

void
INIEnvironmentDestroy (INIEnvironment *env)
{
  if (!env)
    {
      return;
    }

  IObjectFree (env->root);
}
