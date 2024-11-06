#ifndef SQL_H
#define SQL_H
#include "database_structs.h"
#include "node.h"

void sqlEntry(BinaryTree *tree, Database *db);
void insert(BinaryTree *tree,Database *db,char *sqlRest);
void createTable(BinaryTree *tree, Database *db, char *sqlRest);
void delete(BinaryTree *tree, Database *db, char *sqlRest);
void dropTable(BinaryTree *tree, Database *db, char *sqlRest);
#endif