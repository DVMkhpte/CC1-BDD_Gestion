#ifndef FUNCTION_H
#define FUNCTION_H
#include "database_structs.h"
#include "node.h"

void replaceSpacesToDashes(char *str);
long createKey(char *str);
ValueType detectValueType(char *value);
void analyseCondition(BinaryTree *tree, char *tableName, char *column, char *operator, char *value);

#endif