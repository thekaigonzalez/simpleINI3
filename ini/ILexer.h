// $Id: ILexer.h

#ifndef __ILEXER_H
#define __ILEXER_H

// tokens, etc

#include "IBuffer.h"
#include "IObject.h"

typedef struct ILexer ILexer;
typedef struct IToken IToken;
typedef enum ILexerState ILexerState;
typedef enum ITokenType
{
  TKIdentifier,
  TKValue,
  TKSectionName,
  TKBreakout
} ITokenType;

typedef struct ITokens ITokens;

// creates a new token
IToken *ITokenNew (IObject *blk, char *str, ITokenType type);

// allocates a new lexer
ILexer *ILexerNew (IObject *blk);

// sets the state of the lexer
void ILexerSetState (ILexer *lex, ILexerState state);

// returns the state of the lexer
ILexerState ILexerGetState (ILexer *lex);

// returns the buffer of the lexer
IBuffer *ILexerGetBuffer (ILexer *lex);

// appends a character to the buffer
void ILexerAppend (ILexer *lex, char c);

// initiaize a list of tokens
ITokens *ITokensNew (IObject *blk);

// adds a token to the list
void ITokensAppend (ITokens *toks, IToken *tok);

// Tokenizes an input string
ITokens *ILexerTokenize (ILexer *lex, char *str);

// prints tokens
void IPrintTokens (ITokens *tks);

int ITokensSize (ITokens *tks);

IToken *ITokensAt (ITokens *tks, int index);

char *ITokenGetStr (IToken *tok);
ITokenType ITokenGetType (IToken *tok);

#endif
