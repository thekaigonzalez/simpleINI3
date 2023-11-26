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
              IKeyValueSetValue (tmpkey_value,
                                 IConvertToValue (blk, ITokenGetStr (tok)));

              ISectionAppend (current, tmpkey_value);

              tmpkey_value = NULL;
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