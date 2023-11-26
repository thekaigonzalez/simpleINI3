// $Id: IEval.h

#ifndef __IEVAL_H
#define __IEVAL_H

// evaluates tokens and converts them to values

#include "IKeyValue.h"
#include "ILexer.h"
#include "IMemoryPool.h"
#include "INIEnvironment.h"
#include "IObject.h"
#include "IValue.h"

IValue *IConvertToValue (IObject *blk, char *str);
void IEvaluateTokens (IObject *blk, ITokens *tokens, INIEnvironment *env);

#endif
