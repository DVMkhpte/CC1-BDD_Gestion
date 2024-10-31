#ifndef NODE_H
#define NODE_H

#include "database_structs.h"

typedef enum { TABLE_NODE, COLUMN_NODE, VALUE_NODE } NodeType;

typedef struct Node {
    NodeType type;
    union {
        Table tableData;
        Column columnData;
        Value valueData;
    };
    struct Node *left;
    struct Node *right;
} Node;

typedef struct BinaryTree {
    Node *root;
} BinaryTree;



Node* createNode(NodeType type, char *name, ValueType valueType, void *data);
void insertNode(BinaryTree *tree, Node *newNode);
void initBinaryTree(BinaryTree *tree);

void displayNode(Node *node);
void displayTree(BinaryTree *tree);
void generateGraph(BinaryTree *tree);

#endif 