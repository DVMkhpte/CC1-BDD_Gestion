#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/node.h"
#include "../include/function.h"

Node *createNode(NodeType type, char *name, ValueType valueType, void *data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erreur d'allocation de mémoire pour le nœud.\n");
        exit(EXIT_FAILURE);
    }
    newNode->type = type;
    newNode->left = NULL;
    newNode->right = NULL;

    long key = createKey(name);
    
    switch (type) {
        case TABLE_NODE:
            strncpy(newNode->tableData.tableName, name, sizeof(newNode->tableData.tableName) - 1);
            newNode->tableData.key = key;
            break;
        case COLUMN_NODE:
            strncpy(newNode->columnData.columnName, name, sizeof(newNode->columnData.columnName) - 1);
            newNode->columnData.key = key;
            break;
        case VALUE_NODE:
    
        switch (valueType) {
            case INT_VALUE:
                newNode->valueData.data.intValue = *(int *)data;
                break;
            case FLOAT_VALUE:
                newNode->valueData.data.floatValue = *(float *)data;
                break;
            case STRING_VALUE:
                strncpy(newNode->valueData.data.stringValue, (char *)data, sizeof(newNode->valueData.data.stringValue) - 1);
                newNode->valueData.data.stringValue[sizeof(newNode->valueData.data.stringValue) - 1] = '\0';
            break;
        }
    
            newNode->valueData.key = key;
            break;
    }

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
                    if (newNode->tableData.key < current->tableData.key) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                    break;

                case COLUMN_NODE:
                    if (newNode->columnData.key < current->columnData.key) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                    break;

                case VALUE_NODE:
                    if (newNode->valueData.key < current->valueData.key) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                    break;
            }
        }

        
        switch (newNode->type) {
            case TABLE_NODE:
                if (newNode->tableData.key < parent->tableData.key) {
                    parent->left = newNode;
                } else {
                    parent->right = newNode;
                }
                break;

            case COLUMN_NODE:
                if (newNode->columnData.key < parent->columnData.key) {
                    parent->left = newNode;
                } else {
                    parent->right = newNode;
                }
                break;

            case VALUE_NODE:
                if (newNode->valueData.key < parent->valueData.key) {
                    parent->left = newNode;
                } else {
                    parent->right = newNode;
                }
                break;
        }
    }
}

void deleteNode(Node **node) {
    if (*node == NULL) return;

    if ((*node)->left == NULL) {
        Node *temp = (*node)->right;
        free(*node);
        *node = temp;
    } else if ((*node)->right == NULL) {
        Node *temp = (*node)->left;
        free(*node);
        *node = temp; 
    } else {
        
        Node *temp = (*node)->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }

        (*node)->valueData = temp->valueData; 

        deleteNode(&((*node)->right)); 
    }
}

void freeTree(Node *node) {
    if (node == NULL) {
        return;
    }

    freeTree(node->left);
    freeTree(node->right);

    free(node);
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
            printf("Table: %s (KEY : %ld)\n", node->tableData.tableName,node->tableData.key);
            break;
        case COLUMN_NODE:
            printf("Column: %s (Type: %s) (KEY : %ld)\n", node->columnData.columnName, node->columnData.type, node->columnData.key);
            break;
        case VALUE_NODE:
            // Ici, tu dois décider comment afficher la valeur,
            // mais pour l'instant on va afficher une valeur générique
            // Par exemple, on peut vérifier un des types de la valeur
            printf("Value: ");
            // On utilise un exemple, tu devras ajuster en fonction de ta logique
            if (node->valueData.data.intValue != 0) {
                printf("%d (KEY : %ld)\n", node->valueData.data.intValue, node->valueData.key);

            } else if (node->valueData.data.floatValue != 0.0f) {
                printf("%f (KEY : %ld)\n", node->valueData.data.floatValue, node->valueData.key);

            } else {
                printf("%s  (KEY : %ld)\n", node->valueData.data.stringValue, node->valueData.key);


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

void generateDotFile(Node *node, FILE *file) {
    if (node == NULL) {
        return;
    }

    // Écrire le nœud actuel
    switch (node->type) {
        case TABLE_NODE:
            fprintf(file, "    \"%s\" [label=\"Table: %s\"];\n", node->tableData.tableName, node->tableData.tableName);
            break;
        case COLUMN_NODE:
            fprintf(file, "    \"%s\" [label=\"Column: %s (Type: %s)\"];\n", node->columnData.columnName, node->columnData.columnName, node->columnData.type);
            break;
        case VALUE_NODE:
            fprintf(file, "    \"Value_%p\" [label=\"Value: %s\"];\n", (void*)node, node->valueData.data.stringValue);
            break;
    }

    // Écrire les relations avec les nœuds enfants
    if (node->left != NULL) {
        fprintf(file, "    \"%s\" -> \"%s\";\n", node->tableData.tableName, node->left->tableData.tableName);
        generateDotFile(node->left, file);
    }
    if (node->right != NULL) {
        fprintf(file, "    \"%s\" -> \"%s\";\n", node->tableData.tableName, node->right->tableData.tableName);
        generateDotFile(node->right, file);
    }
}

void generateGraph(BinaryTree *tree) {
    FILE *file = fopen("tree.dot", "w");
    if (file == NULL) {
        printf("Erreur d'ouverture du fichier DOT.\n");
        return;
    }

    fprintf(file, "digraph G {\n"); // Commence le graphe
    generateDotFile(tree->root, file); // Générez le contenu de l'arbre
    fprintf(file, "}\n"); // Termine le graphe

    fclose(file);

    // Utiliser Graphviz pour générer une image PNG
    system("dot -Tpng tree.dot -o tree.png");
    printf("L'arbre a été généré sous forme d'image tree.png.\n");
}