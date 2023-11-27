// $Id: ISample.c

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "IBuffer.h"
#include "IEval.h"
#include "IKeyValue.h"
#include "ILexer.h"
#include "IMemoryPool.h"
#include "INIEnvironment.h"
#include "IObject.h"
#include "ISection.h"
#include "IValue.h"

#define pass()                                                                \
  printf ("test: %s passed\n", __func__);                                     \
  return 0;

test_memory_pooling (void)
{
  IMemoryPool *p = IMNew ();

  char *str = (char *)IMAlloc (p, 10);

  str = IMAlloc (p, 10); // blatantly overriding the previous allocation,
                         // should still clean it

  IMClean (p);

  pass ();
}

test_object_pool (void)
{
  IObject *o = IObjectNew ();

  IObjectFree (o);

  pass ();
}

test_values (void)
{
  IObject *o = IObjectNew ();

  IValue *value = IValueFromString (o, "10");
  IValue *v2 = IValueFromString (o, "abc");

  assert (IValueToInt (value) + 2 == 12);
  assert (strcmp (IValueToIdentifier (v2), "abc") == 0);

  assert (IValueGetKind (value) == Number);
  assert (IValueGetKind (v2) == Identifier);

  IObjectFree (o);

  pass ();
}

test_them_key_values (void)
{
  IObject *o = IObjectNew ();

  IKeyValue *kv = IKeyValueNew (o);

  IKeyValueSetKey (kv, "key");
  IKeyValueSetValue (kv, IValueFromString (o, "10"));

  assert (strcmp (IKeyValueGetKey (kv), "key") == 0);
  assert (IValueToInt (IKeyValueGetValue (kv)) == 10);
  assert (IValueGetKind (IKeyValueGetValue (kv)) == Number);

  IObjectFree (o);

  pass ();
}

test_sections (void)
{
  IObject *o = IObjectNew ();
  ISection *root = ISectionNew (o);

  IKeyValue *k = IKeyValueNew (o);
  IKeyValueSetKey (k, "key");
  IKeyValueSetValue (k, IValueFromString (o, "10"));

  ISectionAppend (root, k);

  ISectionSetHeader (root, "header");

  IObjectFree (o);
  pass ();
}

test_environment (void)
{
  INIEnvironment *env = INIEnvironmentNew ();

  ISection *sect = INIEnvironmentAddSection (env, "test");

  IKeyValue *kv = IKeyValueNew (INIEnvObject (env));
  IKeyValueSetKey (kv, "key");
  IKeyValueSetValue (kv, IValueFromString (INIEnvObject (env), "10"));

  ISectionAppend (sect, kv);

  INIEnvironmentPrint (env);

  INIEnvironmentDestroy (env);

  pass ();
}

test_buffers (void)
{
  IObject *o = IObjectNew ();

  IBuffer *b = IBufferNew (o);

  IBufferAppend (b, 'a');
  IBufferAppend (b, 'b');
  IBufferAppend (b, 'c');

  assert (IBufferGetSize (b) == 3);
  assert (IBufferGet (b, 1) == 'b');

  IObjectFree (o);

  pass ();
}

test_lexer (void)
{
  IObject *blk = IObjectNew ();
  ILexer *l = ILexerNew (blk);

  ITokens *tks = ILexerTokenize (l, "[section]\na = (1,(2,3,42),3)\n");

  IPrintTokens (tks);

  IObjectFree (blk);

  pass ();
}

test_lexer_evaluation (void)
{
  INIEnvironment *env = INIEnvironmentNew ();

  ILexer *l = ILexerNew (INIEnvObject (env));

  ITokens *tks
      = ILexerTokenize (l, "[section]\n; this\na = (1, 2) ; inline comment\nb "
                           "= \\($)\n[section-2]\nb = 2\n");

  IPrintTokens (tks);

  IEvaluateTokens (INIEnvObject (env), tks, env);

  INIEnvironmentPrint (env);

  IObjectFree (INIEnvObject (env));

  pass ();
}

main (void)
{
  test_memory_pooling ();
  test_object_pool ();
  test_values ();
  test_them_key_values ();
  test_sections ();
  test_environment ();
  test_buffers ();
  test_lexer ();
  test_lexer_evaluation ();
}
