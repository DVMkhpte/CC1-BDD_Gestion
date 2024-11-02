#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file.h"
#include "../include/menu.h"
#include "../include/sql.h"
#include "../include/database_structs.h"
#include "../include/function.h"


FILE *file = NULL;

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
            printf("Le fichier a été supprimé avec succès.\n");
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
        scanf("%3s", res);

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
            printf("Nom de la table à charger : %s\n", tableName);

            

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
                valueNode = createNode(VALUE_NODE, columnName, INT_VALUE, &intValue);
            } else if (valueType == FLOAT_VALUE) {
                float floatValue = strtof(value, NULL);
                valueNode = createNode(VALUE_NODE, columnName, FLOAT_VALUE, &floatValue);
            } else if (valueType == STRING_VALUE) {
                valueNode = createNode(VALUE_NODE, columnName, STRING_VALUE, value);
            }

            insertNode(&tree, valueNode);
        }
         
    }

    displayTree(&tree);
    fclose(file);
    printf("Base de données chargée avec succès depuis le fichier %s\n", filepath);

    createSecondMenu();
    sqlEntry(&tree, db);
}



int writeInDatabase(char *values) {
    fprintf(file, "\n%s", values);
    fflush(file); // Permet de vider le tampon
    return EXIT_SUCCESS;
}