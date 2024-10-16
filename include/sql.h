#ifndef SQL_H
#define SQL_H
#include "database_structs.h"
#include "node.h"

void sqlEntry(BinaryTree *tree, Database *db);
void insert(Database *db,char *sqlRest);
void createTable(BinaryTree *tree, Database *db, char *sqlRest);
void delete(char *sqlRest);
#endif