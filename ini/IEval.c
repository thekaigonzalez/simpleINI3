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
                          ps = 1;
                        }
                      else if (str[i] == '(' && ps != 2)
                        {
                          ps = 2;
                        }
                      else if (str[i] == ')' && ps == 2)
                        {
                          ps = 0;
                        }
                      else
                        {
                          if (ps == 2)
                            {
                              IBufferAppend (__tmp, str[i]);
                            }
                        }
                    }

                  char *value_nam = IBufferCopy (__tmp);
                  IValue *va
                      = IKeyValueGetValue (ISectionGet (current, value_nam));

                  printf ("value_nam: %d\n", IValueGetKind (va));

                  printf ("vanam: %s\n", value_nam);

                  IKeyValueSetValue (tmpkey_value, va);
                  ISectionAppend (current, tmpkey_value);

                  tmpkey_value = NULL;
                }
              else
                {

                  IKeyValueSetValue (tmpkey_value, vconv);

                  ISectionAppend (current, tmpkey_value);

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