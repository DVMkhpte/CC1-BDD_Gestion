#ifndef FILE_H
#define FILE_H

#include "../include/node.h"

int verifFileExist(char *filename);
int verifFileExistW(char *filename);
int verifFileExistD(char *filename);
void createDatabase(char *filename);
void loadDatabase(char *filename);
int writeInDatabase(char *values);
void deleteValuesFromFile(BinaryTree *tree, char *tableName);
void deleteValuesFromFileWithCondition(BinaryTree *tree,char *tableName,char *column, char *operator, char *value, ValueType typeV);
void deleteTableFromFile(BinaryTree *tree,char *tableName);


#endif