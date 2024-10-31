#ifndef FUNCTION_H
#define FUNCTION_H
#include "database_structs.h"

void replaceSpacesToDashes(char *str);
long createKey(char *str);
ValueType detectValueType(char *value);

#endif