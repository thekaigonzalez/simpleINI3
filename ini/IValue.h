// $Id: IValue.h

#ifndef __IVALUE_H
#define __IVALUE_H

#include "IObject.h"

// defines transforms for values

typedef struct IValue IValue;
typedef struct IValueList IValueList;

typedef enum IValueKind
{
  Number,
  Float,
  Identifier,
  String,
  List,
  Nil,
  Malformed,
  Interpolation,
  Unknown,
} IValueKind;

// allocates a new regular value
IValue *IValueNew (IObject *blk);

// converts the input string to a value, returns a new value
IValue *IValueFromString (IObject *blk, char *str);

// creates a new value list
IValueList *IValueListNew (IObject *blk);

// appends a value to a value list
void IValueListAppend (IValueList *vl, IValue *v);

// returns the kind of an IValue
IValueKind IValueGetKind (IValue *v);

// type conversion
int IValueToInt (IValue *v);
float IValueToFloat (IValue *v);
char *IValueToString (IValue *v);
char *IValueToIdentifier (IValue *v);
IValueList *IValueToList (IValue *v);

void IValuePrintAsString (IValue *v);

#endif
