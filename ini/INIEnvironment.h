// $Id: INIEnvironment.h

#ifndef INIENVIRONMENT_H
#define INIENVIRONMENT_H

#include "IObject.h"
#include "ISection.h"

// environments contain a memory pool as well as a list of sections
typedef struct INIEnvironment INIEnvironment;

// allocates a new environment
INIEnvironment *INIEnvironmentNew ();

// adds a section to the environment
void INIEnvironmentAppendSection (INIEnvironment *env, ISection *sec);

// gets the header of a section
char *ISectionGetHeader (ISection *sec);

// returns a section by name
ISection *INIEnvironmentGetSection (INIEnvironment *env, char *name);

// returns the environment's root pool
IObject *INIEnvObject (INIEnvironment *env);

// prints out an environment
void INIEnvironmentPrint (INIEnvironment *env);

// allocates and adds an empty section with the header name then returns it
ISection *INIEnvironmentAddSection (INIEnvironment *env, char *name);

// frees an environment
void INIEnvironmentDestroy (INIEnvironment *env);

#endif
