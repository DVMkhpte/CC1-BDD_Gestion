#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file.h"
#include "../include/menu.h"
#include "../include/sql.h"
#include "../include/database_structs.h"
#include "../include/function.h"


FILE *file = NULL;
char *fileName = NULL;

int verifFileExist(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    file = fopen(filepath, "r");

    if (file) {
        printf("Une base de donnée existe deja sous ce nom.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

int verifFileExistW(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    file = fopen(filepath, "r");

    if (!file) {
        printf("Aucune base de donnée n'existe sous ce nom\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }else{
        fclose(file);
        file = fopen(filepath, "a");
    }
}

int verifFileExistD(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    file = fopen(filepath, "r");

    if (file) {
        if (remove(filepath) == 0) {
            printf("La base de donnée a été supprimé avec succès.\n");
            return EXIT_SUCCESS;
        }else{
            printf("Erreur lors de la suppression de la base de donnée.\n");
            exit(EXIT_FAILURE);
        }
    }else{
        printf("Aucune base de donnée n'existe sous ce nom\n");
        exit(EXIT_FAILURE);
    }
}


void createDatabase(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    file = fopen(filepath, "w");

    if (!file) {
        printf("Erreur lors de la création de la base de donnée\n");
        exit(EXIT_FAILURE);;
    }

    fclose(file);

    char res[4];
    int8_t x = 1;

    while (x == 1) {
        printf("Voulez-vous USE la base de données %s ? (Oui/Non) : ", filename);
        fgets(res, sizeof(res), stdin);
        res[strcspn(res, "\n")] = '\0';
        fflush(stdin);

        if (strcmp(res, "O") == 0 || strcmp(res, "o") == 0 || strcmp(res, "oui") == 0 || strcmp(res, "Oui") == 0 || strcmp(res, "OUI") == 0) {
            file = fopen(filepath, "a");
            if (!file) {
                printf("Erreur lors de l'ouverture de la base de données\n");
                exit(EXIT_FAILURE);
            }

            Database *db = malloc(sizeof(Database));
            if (db == NULL) {
                printf("Erreur lors de l'allocation de la mémoire pour la base de données.\n");
                exit(EXIT_FAILURE);
            }

            initDatabase(db,filename);

            printf("Vous utilisez maintenant la base de données ' %s '.\n", filename);
            x = 0;

            BinaryTree tree;
            initBinaryTree(&tree);

            fileName = filename;

            createSecondMenu();
            sqlEntry(&tree, db);

        } else if (strcmp(res, "N") == 0 || strcmp(res, "n") == 0 || strcmp(res, "non") == 0 || strcmp(res, "Non") == 0 || strcmp(res, "NON") == 0) {
            return EXIT_SUCCESS;
        } else {
            printf("Réponse invalide.\n");
        }
    }
    
}


void loadDatabase(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture de la base de donnée\n");
        exit(EXIT_FAILURE);
    }

    BinaryTree tree;
    initBinaryTree(&tree);

    Database *db = malloc(sizeof(Database));
    if (db == NULL) {
        printf("Erreur lors de l'allocation de la mémoire pour la base de données.\n");
        exit(EXIT_FAILURE);
    }

    initDatabase(db, filename);

    char line[512];
    Node *tableNode = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        if (strncmp(line, "table.", 6) == 0) {
            char tableName[256];
            sscanf(line, "table.%255s", tableName);
            
            Node *tableNode = createNode(TABLE_NODE, tableName, INT_VALUE, NULL);
            strcpy(tableNode->tableData.tableName, tableName); 
            tableNode->tableData.columns = NULL;
            tableNode->tableData.columnCount = 0;
            db->tableCount++;
            
            insertNode(&tree, tableNode);
         
        }
        else if (strncmp(line, "column.", 7) == 0) {
            char tableName[256], columnName[256], columnType[100];
            sscanf(line, "column.%255[^.].%255[^-]-%99s", tableName, columnName, columnType);

            char fullColumnName[512];
            snprintf(fullColumnName, sizeof(fullColumnName), "column.%s.%s", tableName, columnName);

            Node *columnNode = createNode(COLUMN_NODE, fullColumnName, INT_VALUE, NULL);
            strcpy(columnNode->columnData.columnName, columnName);
            strcpy(columnNode->columnData.type, columnType);

            //tableNode->tableData.columns = realloc(tableNode->tableData.columns, sizeof(Column) * (tableNode->tableData.columnCount + 1));
            //tableNode->tableData.columns[tableNode->tableData.columnCount] = columnNode->columnData;
            //tableNode->tableData.columnCount++;
            
            insertNode(&tree, columnNode);
         
        }
        else if (strncmp(line, "values.", 7) == 0) {
            char tableName[256], columnName[256], value[256];
            sscanf(line, "values.%255[^.].%255[^.].%255s", tableName, columnName, value);

            ValueType valueType = detectValueType(value);
            Node *valueNode;

            if (valueType == INT_VALUE) {
                int intValue = strtol(value, NULL, 10);
                valueNode = createNode(VALUE_NODE, line, INT_VALUE, &intValue);
            } else if (valueType == FLOAT_VALUE) {
                float floatValue = strtof(value, NULL);
                valueNode = createNode(VALUE_NODE, line, FLOAT_VALUE, &floatValue);
            } else if (valueType == STRING_VALUE) {
                valueNode = createNode(VALUE_NODE, line, STRING_VALUE, value);
            }

            insertNode(&tree, valueNode);
        }
         
    }

    fclose(file);
    printf("Base de données chargée avec succès depuis le fichier %s\n", filepath);

    fileName = filename;

    createSecondMenu();
    sqlEntry(&tree, db);
}



int writeInDatabase(char *values) {
    fprintf(file, "\n%s", values);
    fflush(file); // Permet de vider le tampon
    return EXIT_SUCCESS;
}

void deleteValuesFromFile(BinaryTree *tree, char *tableName) {
    
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", fileName);

    file = fopen(filepath, "r");  
    if (!file) {
        printf("Erreur lors de l'ouverture de la base de donnée\n");
        return;
    }
    
    FILE *tempFile = fopen("database/temp.txt", "w");
    if (!tempFile) {
        printf("Erreur lors de la création du fichier temporaire\n");
        fclose(file);
        return;
    }

    char line[512];
    char valuesPrefix[512];
    int8_t isValue = 0;
    snprintf(valuesPrefix, sizeof(valuesPrefix), "values.%s", tableName);

    while (fgets(line, sizeof(line), file) != NULL) {
        
        if (strncmp(line, valuesPrefix, strlen(valuesPrefix)) != 0) {
            fputs(line, tempFile);  
        }else{
            isValue++;
            char tableName[256], columnName[256], value[256];
            line[strcspn(line, "\n")] = '\0';
            
            long valueKey = createKey(line);
            Node *current = tree->root;

            while (current != NULL) {
                if (current->valueData.key == valueKey) {  
                    printf("go delete le node\n");
                    deleteNode(&current);
                    break;
                }
                if (valueKey < current->valueData.key) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }

        }


    }

    if (isValue  == 0) printf("Aucune valeur de la table %s n'a été supprimée puisque elle est vide.\n", tableName);
    else printf("Toutes les valeurs de la table '%s' ont été supprimées\n", tableName);

    fclose(file);
    fclose(tempFile);

    remove(filepath);
    rename("database/temp.txt", filepath);

}

void deleteValuesFromFileWithConditionInt(BinaryTree *tree, char *tableName, char *column, char *operator, char *value) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", fileName);

    file = fopen(filepath, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture de la base de données\n");
        return;
    }

    FILE *tempFile = fopen("database/temp.txt", "w");
    if (!tempFile) {
        printf("Erreur lors de la création du fichier temporaire\n");
        fclose(file);
        return;
    }

    char line[512];
    char valuesPrefix[512];
    char lineValue[256];
    int isValue = 0;
    int targetValue = atoi(value);

    printf("operator : %s\n", operator);

    if (strcmp(operator, "=") == 0) {
        snprintf(valuesPrefix, sizeof(valuesPrefix), "values.%s.%s.%s", tableName, column, value);

        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, valuesPrefix, strlen(valuesPrefix)) != 0) {
                fputs(line, tempFile);
            } else {
                isValue++;
                line[strcspn(line, "\n")] = '\0';
                long valueKey = createKey(line);
                Node *current = tree->root;

                while (current != NULL) {
                    if (current->valueData.key == valueKey) {
                        
                        deleteNode(&current);
                        break;
                    }
                    if (valueKey < current->valueData.key) {
                        current = current->left;
                    } else {
                        current = current->right;
                    }
                }
            }
        }
    } else if (strcmp(operator, "<") == 0) {

        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, "values", 6) == 0) {  
                sscanf(line, "values.%*[^.].%*[^.].%s", lineValue);
                int currentValue = atoi(lineValue);

                if (currentValue >= targetValue) {
                    fputs(line, tempFile);
                } else {
                    isValue++;
                    line[strcspn(line, "\n")] = '\0';
                    long valueKey = createKey(line);
                    Node *current = tree->root;

                    while (current != NULL) {
                        if (current->valueData.key == valueKey) {
                            deleteNode(&current);
                            break;
                        }
                        if (valueKey < current->valueData.key) {
                            current = current->left;
                        } else {
                            current = current->right;
                        }
                    }
                }
            } else {
                fputs(line, tempFile);
            }
        }
    } else if (strcmp(operator, ">") == 0) {

        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, "values", 6) == 0) {  
                sscanf(line, "values.%*[^.].%*[^.].%s", lineValue);
                int currentValue = atoi(lineValue);

                if (currentValue <= targetValue) {
                    fputs(line, tempFile);
                } else {
                    isValue++;
                    line[strcspn(line, "\n")] = '\0';
                    long valueKey = createKey(line);
                    Node *current = tree->root;

                    while (current != NULL) {
                        if (current->valueData.key == valueKey) {
                            deleteNode(&current);
                            break;
                        }
                        if (valueKey < current->valueData.key) {
                            current = current->left;
                        } else {
                            current = current->right;
                        }
                    }
                }
            } else {
                fputs(line, tempFile);
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filepath);
    rename("database/temp.txt", filepath);
}

void deleteTableFromFile(BinaryTree *tree, char *tableName) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", fileName);

    file = fopen(filepath, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture de la base de donnée\n");
        return;
    }

    FILE *tempFile = fopen("database/temp.txt", "w");
    if (!tempFile) {
        printf("Erreur lors de la création du fichier temporaire\n");
        fclose(file);
        return;
    }

    char line[512];
    char tablePrefix[512], columnPrefix[512], valuesPrefix[512];
    int8_t isValue = 0;

    snprintf(tablePrefix, sizeof(tablePrefix), "table.%s", tableName);
    snprintf(columnPrefix, sizeof(columnPrefix), "column.%s", tableName);
    snprintf(valuesPrefix, sizeof(valuesPrefix), "values.%s", tableName);

    long tableKey = createKey(tablePrefix);

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, tablePrefix, strlen(tablePrefix)) != 0 &&
            strncmp(line, columnPrefix, strlen(columnPrefix)) != 0 &&
            strncmp(line, valuesPrefix, strlen(valuesPrefix)) != 0) {
            fputs(line, tempFile);  
        } else {
            isValue++;
            line[strcspn(line, "\n")] = '\0';

            char *suffixes[] = {"-INT", "-CHAR", "-FLOAT"};
            for (int i = 0; i < 3; i++) {
                char *pos = strstr(line, suffixes[i]);
                if (pos != NULL) {
                    *pos = '\0';
                    break;
                }
            }

            long lineKey = createKey(line);
            Node *current = tree->root;

            while (current != NULL) {
                if (current->tableData.key == tableKey) {
                    printf("Suppression de la table dans l'arbre\n");
                    deleteNode(&current);
                    break;
                } else if (current->columnData.key == lineKey) {
                    printf("Suppression de la colonne dans l'arbre\n");
                    deleteNode(&current);
                    break;
                } else if (current->valueData.key == lineKey) {
                    printf("Suppression de la valeur dans l'arbre\n");
                    deleteNode(&current);
                    break;
                }

                if (lineKey < current->valueData.key || tableKey < current->tableData.key || lineKey < current->columnData.key) {
                    current = current->left;
                    printf("Go Gauche\n");
                } else {
                    current = current->right;
                    printf("Go Droite\n");
                }
            }
        }

        printf("Ligne suivante\n");
    }

    if (isValue != 0) {
        printf("La table %s et toutes ses valeurs ont été supprimées.\n", tableName);
    }

    fclose(file);
    fclose(tempFile);

    remove(filepath);
    rename("database/temp.txt", filepath);
}

