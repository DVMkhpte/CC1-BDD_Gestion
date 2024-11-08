#ifndef SQL_H
#define SQL_H
#include "database_structs.h"
#include "node.h"

void sqlEntry(BinaryTree *tree, Database *db);
void insert(BinaryTree *tree,Database *db,char *sqlRest);
void createTable(BinaryTree *tree, Database *db, char *sqlRest);
void delete(BinaryTree *tree, Database *db, char *sqlRest);
void dropTable(BinaryTree *tree, Database *db, char *sqlRest);
void show(BinaryTree *tree);
void showNode(Node *node, char *currentTable);
void selectt(BinaryTree *tree, char *sqlRest);
void showValue(BinaryTree *tree, Node *node, char *tableName, char *columnName);
#endif