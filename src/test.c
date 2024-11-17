#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/database_structs.h"
#include "../include/node.h"
#include "../include/sql.h"
#include "../include/file.h"
#include "../include/function.h"

Node *searchNode(Node* root, long key) {
    if (root == NULL) {
        return NULL; 
    }

    if ((root->tableData.key == key && root->tableData.key != 0) ||
        (root->columnData.key == key && root->columnData.key != 0) ||
        (root->valueData.key == key && root->valueData.key != 0)) {
        return root;
    }

    long rootKey = root->tableData.key ? root->tableData.key :
                   root->columnData.key ? root->columnData.key : 
                   root->valueData.key;

    if (key < rootKey) {
        return searchNode(root->left, key);
    } else {
        return searchNode(root->right, key);
    }
}

Node *svalue(Node* current, long valueKey) {
                        while (current != NULL) {
                            if (current->valueData.key == valueKey) {
                                return current;
                            }
                            if (valueKey < current->valueData.key) {
                                current = current->left;
                            } else {
                                current = current->right;
                            }
                        }
}

void test() {
    BinaryTree tree;
    Database db;

    // Test de l'initialisation de l'arbre
    initBinaryTree(&tree);
    assert(tree.root == NULL); 
    printf("Test de l'initialisation de l'arbre réussi.\n");

    // Test de l'initialisation de la base de données
    initDatabase(&db, "test");
    assert(strcmp(db.databaseName, "test") == 0);
    printf("Test de l'initialisation de la base de données réussi.\n");

    // Test de création d'une table et insertion dans l'arbre
    Node *tableNode = createNode(TABLE_NODE, "tableTest", INT_VALUE, NULL);
    strcpy(tableNode->tableData.tableName, "tableTest"); 
    tableNode->tableData.columns = NULL;
    tableNode->tableData.columnCount = 0;
    db.tableCount++;

    insertNode(&tree, tableNode);
    assert(searchNode(tree.root, createKey("tableTest")) != NULL);
    printf("Test de la création et insertion d'une table nommée tableTest réussi.\n");

    // Test de création de colonnes de différents types et insertion dans l'arbre
    Node *columnNode;
    
    columnNode = createNode(COLUMN_NODE, "column.tableTest.int", INT_VALUE, NULL); 
    strcpy(columnNode->columnData.columnName, "int");
    strcpy(columnNode->columnData.type, "INT");
    insertNode(&tree, columnNode);
    assert(searchNode(tree.root, createKey("column.tableTest.int")) != NULL);
    printf("Test de la création et insertion de la colonne INT réussi.\n");

    columnNode = createNode(COLUMN_NODE, "column.tableTest.float", FLOAT_VALUE, NULL); 
    strcpy(columnNode->columnData.columnName, "float");
    strcpy(columnNode->columnData.type, "FLOAT");
    insertNode(&tree, columnNode);
    assert(searchNode(tree.root, createKey("column.tableTest.float")) != NULL);
    printf("Test de la création et insertion de la colonne FLOAT réussi.\n");

    columnNode = createNode(COLUMN_NODE, "column.tableTest.str", STRING_VALUE, NULL); 
    strcpy(columnNode->columnData.columnName, "str");
    strcpy(columnNode->columnData.type, "CHAR");
    insertNode(&tree, columnNode);
    assert(searchNode(tree.root, createKey("column.tableTest.str")) != NULL);
    printf("Test de la création et insertion de la colonne CHAR réussi.\n");

    // Test d'insertion de valeurs dans la table
    Node *valueNode;
    int intValue = atoi("123");
    float floatValue = atof("3.14");

    valueNode = createNode(VALUE_NODE, "values.tableTest.int.123", INT_VALUE, &intValue);
    insertNode(&tree, valueNode);
    assert(svalue(tree.root, createKey("values.tableTest.int.123")) != NULL);
    printf("Test d'insertion de valeur 123 dans INT réussi.\n");

    valueNode = createNode(VALUE_NODE, "values.tableTest.float.3.140000", FLOAT_VALUE, &floatValue);
    insertNode(&tree, valueNode);
    assert(svalue(tree.root, createKey("values.tableTest.float.3.140000")) != NULL);
    printf("Test d'insertion de valeur 3.14 dans FLOAT réussi.\n");

    valueNode = createNode(VALUE_NODE, "values.tableTest.str.testValue", STRING_VALUE, "testValue");
    insertNode(&tree, valueNode);
    assert(svalue(tree.root, createKey("values.tableTest.str.testValue")) != NULL);
    printf("Test d'insertion de valeur testValue dansCHAR réussi.\n");

    // Test de Show sur la database
    printf("Test de la fonction SHOW sur la database.\n");
    show(&tree);

    // Test de SELECT sur la table tableTest pour une colonne
    printf("Test de la fonction SELECT sur la table tableTest pour la colonne int.\n");
    selectt(&tree, "SELECT * FROM tableTest FOR int;");

    printf("Test de la fonction SELECT sur la table tableTest pour la colonne float.\n");
    selectt(&tree, "SELECT * FROM tableTest FOR float;");

    printf("Test de la fonction SELECT sur la table tableTest pour la colonne str.\n");
    selectt(&tree, "SELECT * FROM tableTest FOR str;");


    // Test de DELETE sur la table tableTest pour les differentes colonnes
    printf("Test de la fonction DELETE sur la table tableTest pour les differentes colonnes.\n");
    valueNode = svalue(tree.root, createKey("values.tableTest.int.123"));
    deleteNode(&valueNode);
    valueNode = svalue(tree.root, createKey("values.tableTest.float.3.140000"));
    deleteNode(&valueNode);
    valueNode = svalue(tree.root, createKey("values.tableTest.str.testValue"));
    deleteNode(&valueNode);
    printf("Test de la suppression des 3 valeurs creer ulterieurement réussi.\n");
    printf("\n");

    printf("Tous les tests ont réussi.\n");
}
