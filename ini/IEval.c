#include "IEval.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

IValue *
IConvertToValue (IObject *blk, char *str)
{
  return IValueFromString (blk, str);
}

void
IEvaluateTokens (IObject *blk, ITokens *tokens, INIEnvironment *env)
{
  if (!blk || !tokens || !env)
    {
      return;
    }

  ISection *current = ISectionNew (blk);
  ISectionSetHeader (current, "_root");

  IKeyValue *tmpkey_value = NULL;
  IKeyValue *last = NULL;

  int pc = 0;

  for (int i = 0; i < ITokensSize (tokens); ++i)
    {
      IToken *tok = ITokensAt (tokens, i);

      if (ITokenGetType (tok) == TKIdentifier)
        {
          tmpkey_value = IKeyValueNew (blk);

          IKeyValueSetKey (tmpkey_value, ITokenGetStr (tok));
        }
      else if (ITokenGetType (tok) == TKValue)
        {
          if (tmpkey_value)
            {
              IValue *vconv = IConvertToValue (blk, ITokenGetStr (tok));

              if (IValueGetKind (vconv) == Interpolation)
                {
                  IBuffer *__tmp = IBufferNew (blk);
                  char *str = IValueToString (vconv);

                  int ps = 0;

                  for (int i = 0; i < strlen (str); ++i)
                    {
                      if (str[i] == '\\' && ps != 1)
                        {
                          ps = 1; // first part
                        }
                      else if (str[i] == '(' && ps != 2)
                        {
                          ps = 2; // second part
                        }
                      else if (str[i] == ')' && ps == 2)
                        {
                          ps = 0;
                        }
                      else
                        {
                          if (!isalpha (str[i]) && str[i] != '$' && ps != 1)
                            {
                              printf ("si3: error in interpolation: only "
                                      "[a-zA-Z] allowed here\n");
                              printf ("\t%s\n", IBufferCopy (__tmp));
                              exit (1);
                            }
                          if (ps == 2)
                            {
                              IBufferAppend (__tmp, str[i]);
                            }
                        }
                    }

                  char *value_nam = IBufferCopy (__tmp);

                  IValue *va
                      = IKeyValueGetValue (ISectionGet (current, value_nam));

                  if (!va)
                    {
                      printf ("si3: error in interpolation: reference `%s' "
                              "could not be located.\n",
                              value_nam);
                      printf ("note: variables you are allowed to reference "
                              "in this section:\n");

                      for (int i = 0; i < ISectionSize (current); ++i)
                        {
                          printf (
                              "  \\(%s)\n",
                              IKeyValueGetKey (ISectionGetAt (current, i)));
                        }
                      IObjectFree (blk);
                      exit (1);
                    }

                  IKeyValueSetValue (tmpkey_value, va);
                  ISectionAppend (current, tmpkey_value);
                  last = tmpkey_value;

                  tmpkey_value = NULL;
                }
              else
                {

                  IKeyValueSetValue (tmpkey_value, vconv);

                  ISectionAppend (current, tmpkey_value);
                  last = tmpkey_value;

                  tmpkey_value = NULL;
                }
            }
        }
      else if (ITokenGetType (tok) == TKSectionName)
        {
          INIEnvironmentAppendSection (env, current);

          current = ISectionNew (blk);

          ISectionSetHeader (current, ITokenGetStr (tok));
        }
    }

  printf ("current header: %s\n", ISectionGetHeader (current));

  INIEnvironmentAppendSection (env, current);
}