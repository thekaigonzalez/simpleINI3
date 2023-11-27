// $Id: ILexer.c

#include "ILexer.h"
#include "IBuffer.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

enum ILexerState
{
  Start,
  RightSide,
  CollectingName,
  Ignorant,
  CollectingListItems
};

struct ILexer
{
  IObject *blk;
  ILexerState state;
  IBuffer *buf;

  int dp; // depth - you'll find out later.
};

struct IToken
{
  char *str;
  ITokenType type;
};

struct ITokens
{
  IToken **ptr;
  int size;
  int cap;
  IMemoryPool *blk;
};

IToken *
ITokenNew (IObject *blk, char *str, ITokenType type)
{
  IToken *t = (IToken *)IMAlloc (IObjectGetMemoryPool (blk), sizeof (IToken));

  if (!t)
    {
      printf ("%s: unable to allocate enough memory for token\n", __func__);
      return NULL;
    }

  t->str = str;
  t->type = type;

  return t;
}

ILexer *
ILexerNew (IObject *blk)
{
  ILexer *l = (ILexer *)IMAlloc (IObjectGetMemoryPool (blk), sizeof (ILexer));

  if (!l)
    {
      printf ("%s: unable to allocate enough memory for lexer\n", __func__);
      return NULL;
    }

  l->blk = blk;
  l->state = Start;
  l->buf = IBufferNew (blk);

  return l;
}

void
ILexerSetState (ILexer *lex, ILexerState state)
{
  lex->state = state;
}

ILexerState
ILexerGetState (ILexer *lex)
{
  return lex->state;
}

IBuffer *
ILexerGetBuffer (ILexer *lex)
{
  return lex->buf;
}

void
ILexerAppend (ILexer *lex, char c)
{
  IBufferAppend (lex->buf, c);
}

ITokens *
ITokensNew (IObject *blk)
{
  ITokens *t
      = (ITokens *)IMAlloc (IObjectGetMemoryPool (blk), sizeof (ITokens));

  t->ptr = IMAlloc (IObjectGetMemoryPool (blk),
                    sizeof (IToken *) * MEMORY_POOL_INCREMENT);

  if (!t || !t->ptr)
    {
      printf ("%s: unable to allocate enough memory for tokens\n", __func__);
      return NULL;
    }

  t->size = 0;
  t->cap = MEMORY_POOL_INCREMENT;

  return t;
}

void
ITokensAppend (ITokens *toks, IToken *tok)
{
  if (!toks || !tok)
    {
      return;
    }

  if (toks->size >= toks->cap)
    {
      toks->cap += MEMORY_POOL_INCREMENT;
      toks->ptr
          = (IToken **)realloc (toks->ptr, sizeof (IToken *) * toks->cap);
      if (!toks->ptr)
        {
          printf (
              "%s: unable to reallocate memory pool\nyou probably should stop "
              "using this if the memory isn't available dude.\n",
              __func__);
        }
    }

  toks->ptr[toks->size] = tok;
  toks->size++;
}

ITokens *
ILexerTokenize (ILexer *lex, char *str)
{
  /* note: follow the syntax of:
  [section]
  key = value

  Should translate to

  Token(TKSectionName, "section")
  Token(TKBreakout, "\n")
  Token(TKIdentifier, "key")
  Token(TKSymbol, "=")
  Token(TKIdentifier, "value")
  Token(TKBreakout, "\n")
  */

  ITokens *toks = ITokensNew (lex->blk);

  int __sec = 0;

  if (!toks)
    {
      return NULL;
    }

  for (int i = 0; i < strlen (str); ++i)
    {
      char c = str[i];
      int end = (i + 1 <= strlen (str) ? i + 1 : strlen (str));
      char next = str[end];

      if (c == '=' && lex->state == Start)
        {
          ILexerSetState (lex, CollectingName);

          ITokensAppend (toks, ITokenNew (lex->blk, IBufferCopy (lex->buf),
                                          TKIdentifier));

          IBufferClear (lex->buf);
          ILexerSetState (lex, RightSide);
        }
      else if (c == ';' && lex->state == Start)
        {
          ILexerSetState (lex, Ignorant);
        }
      else if (c == '[' && lex->state == Start)
        {
          ILexerSetState (lex, CollectingName);
        }

      else if (c == ']' && lex->state == CollectingName)
        {
          ILexerSetState (lex, Start);
          ITokensAppend (toks, ITokenNew (lex->blk, IBufferCopy (lex->buf),
                                          TKSectionName));
          IBufferClear (lex->buf);
          __sec = 1;
        }

      else if (c == '(' && lex->state == RightSide)
        {
          ILexerSetState (lex, CollectingListItems);
          lex->dp = 1;
          IBufferAppend (lex->buf, c);
        }
      else if (c == ')' && lex->state == CollectingListItems && lex->dp == 1)
        {
          ILexerSetState (lex, Start);
          IBufferAppend (lex->buf, c);

          ITokensAppend (
              toks, ITokenNew (lex->blk, IBufferCopy (lex->buf), TKValue));
          IBufferClear (lex->buf);
        }
      else if (c == '(' && lex->state == CollectingListItems)
        {
          lex->dp++;
          IBufferAppend (lex->buf, c);
        }
      else if (c == ')' && lex->state == CollectingListItems)
        {
          lex->dp--;
          IBufferAppend (lex->buf, c);
        }

      else if ((c == '\n' || c == ';') && lex->state == RightSide)
        {

          ITokensAppend (
              toks, ITokenNew (lex->blk, IBufferCopy (lex->buf), TKValue));
          IBufferClear (lex->buf);
          if (c == ';')
            {
              ILexerSetState (lex, Ignorant);
            }
          else
            ILexerSetState (lex, Start);
        }
      else if (c == '\n' && lex->state == Ignorant)
        {
          ILexerSetState (lex, Start);
          IBufferClear (lex->buf);
        }
      else
        {
          if (!isspace (c))
            {
              ILexerAppend (lex, c);
            }
          else if (lex->state == Ignorant)
            {
              ILexerAppend (lex, c);
            }
          else if (!isalpha (c) && !isspace (c) && !isdigit (c)
                   && lex->state == Start)
            {
              printf ("si3: bad character at %d: %c\n", i, c);
              exit (1);
            }
        }
    }

  if (IBufferGetSize (lex->buf) > 0)
    {
      switch (lex->state)
        {
        case RightSide:
          {
            ITokensAppend (
                toks, ITokenNew (lex->blk, IBufferCopy (lex->buf), TKValue));
          }
          break;
        default:
          break;
        }
    }

  return toks;
}

void
IPrintTokens (ITokens *tks)
{
  for (int i = 0; i < tks->size; ++i)
    {
      printf ("Token(%d, \"%s\")\n", tks->ptr[i]->type, tks->ptr[i]->str);
    }
}

int
ITokensSize (ITokens *tks)
{
  return tks->size;
}

IToken *
ITokensAt (ITokens *tks, int index)
{
  if (index >= 0 && index < tks->size)
    {
      return tks->ptr[index];
    }

  return NULL;
}

char *
ITokenGetStr (IToken *tok)
{
  if (!tok)
    {
      return NULL;
    }
  return tok->str;
}

ITokenType
ITokenGetType (IToken *tok)
{
  return tok->type;
}
