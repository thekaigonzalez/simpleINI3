// $Id: ISection.h

#ifndef __ISECTION_H
#define __ISECTION_H

// defines sections which are lists of key-value pairs

#include "IKeyValue.h"
#include "IObject.h"

typedef struct ISection ISection;

// allocates a new section
ISection *ISectionNew (IObject *blk);

// adds a key-value pair to the section
void ISectionAppend (ISection *sec, IKeyValue *kv);

// changes a section's header
void ISectionSetHeader (ISection *sec, char *hdr);

// gets a sections header
char *ISectionHeader (ISection *sec);

// gets the number of key-value pairs in a section
int ISectionSize (ISection *sec);

// gets the section key value at index (for indexing)
IKeyValue *ISectionGetAt (ISection *sec, int index);

// gets a key-value pair
IKeyValue *ISectionGet (ISection *sec, char *key);

// prints out a section
void ISectionPrint (ISection *sec, char separator);

#endif
