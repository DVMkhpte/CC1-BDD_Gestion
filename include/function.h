#ifndef FUNCTION_H
#define FUNCTION_H
#include "database_structs.h"
#include "node.h"

void replaceSpacesToDashes(char *str);
long createKey(char *str);
ValueType detectValueType(char *value);
int verifyInsert(char *sqlRest);

#endif