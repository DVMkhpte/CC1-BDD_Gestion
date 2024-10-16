#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/node.h"

Node* createNode(NodeType type) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erreur d'allocation de mémoire pour le nœud.\n");
        exit(EXIT_FAILURE);
    }
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void initBinaryTree(BinaryTree *tree) {
    tree->root = NULL; // Initialise l'arbre à vide
}

void insertNode(BinaryTree *tree, Node *newNode) {
    if (tree->root == NULL) {
        tree->root = newNode;
    } else {
        Node *current = tree->root;
        Node *parent = NULL;

        while (current != NULL) {
            parent = current;


            switch (newNode->type) {
                case TABLE_NODE:

                    if (strcmp(newNode->tableData.tableName, current->tableData.tableName) < 0) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                    break;

                case COLUMN_NODE:

                    if (strcmp(newNode->columnData.columnName, current->columnData.columnName) < 0) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                    break;

                case VALUE_NODE:

                    if (strcmp(newNode->valueData.data.stringValue, current->valueData.data.stringValue) < 0) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                    break;
            }
        }


        switch (newNode->type) {
            case TABLE_NODE:
                if (strcmp(newNode->tableData.tableName, parent->tableData.tableName) < 0) {
                    parent->left = newNode;
                } else {
                    parent->right = newNode;
                }
                break;

            case COLUMN_NODE:
                if (strcmp(newNode->columnData.columnName, parent->columnData.columnName) < 0) {
                    parent->left = newNode;
                } else {
                    parent->right = newNode;
                }
                break;

            case VALUE_NODE:

                break;
        }
    }
}



/*--------------------------------------------------------------               DEBUG FONCTION               -------------------------------------------------------------EXIT
Q*/




void displayNode(Node *node) {
    if (node == NULL) {
        return; // Si le nœud est NULL, rien à afficher
    }

    // Parcours à gauche
    displayNode(node->left);

    // Affichage des données du nœud selon son type
    switch (node->type) {
        case TABLE_NODE:
            printf("Table: %s\n", node->tableData.tableName);
            break;
        case COLUMN_NODE:
            printf("Column: %s (Type: %s)\n", node->columnData.columnName, node->columnData.type);
            break;
        case VALUE_NODE:
            // Ici, tu dois décider comment afficher la valeur,
            // mais pour l'instant on va afficher une valeur générique
            // Par exemple, on peut vérifier un des types de la valeur
            printf("Value: ");
            // On utilise un exemple, tu devras ajuster en fonction de ta logique
            if (node->valueData.data.intValue != 0) {
                printf("%d\n", node->valueData.data.intValue);
            } else if (node->valueData.data.floatValue != 0.0f) {
                printf("%f\n", node->valueData.data.floatValue);
            } else {
                printf("%s\n", node->valueData.data.stringValue);
            }
            break;
        default:
            break;
    }

    // Parcours à droite
    displayNode(node->right);
}

void displayTree(BinaryTree *tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("L'arbre est vide.\n");
        return;
    }

    printf("Affichage de l'arbre binaire:\n");
    displayNode(tree->root);
}