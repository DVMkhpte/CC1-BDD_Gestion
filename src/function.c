#include <stdio.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include "../include/function.h"
#include "../include/database_structs.h"
#include "../include/file.h"
#include "../include/node.h"

void replaceSpacesToDashes(char *str) { // Fonction qui a pour but de remplacer les espaces par des tirets
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '-';
        }
    }
}

long createKey(char *str) { // Fonction qui a pour but de créer une clé qui sera utilisée pour les recherches dans l'arbre 
    long  sum = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        sum += (int)str[i]; // On additionne toutes les valeurs ascii dans sum
    }

    return sum;
}

ValueType detectValueType(char *value) { // Fonction qui a pour but de détecter le type de valeur char* qui lui est donnée
    
    char *endptr;
    strtol(value, &endptr, 10);
    if (*endptr == '\0') {
        return INT_VALUE;
    }

    strtof(value, &endptr);
    if (*endptr == '\0' && strchr(value, '.')) {
        return FLOAT_VALUE;
    }
        return STRING_VALUE;
}


int verifyInsert(char *sqlRest) { // Fonction qui a pour but de verifier que la requete SQL de l'insert a bien le meme nombre de valeurs que de colonnes
    int columnCount = 0, valueCount = 0;

    char *table = strstr(sqlRest, "INTO");
    char *values = strstr(sqlRest, "VALUES");

    if (table != NULL && values != NULL) {
        table += strlen("INTO");
        while (*table == ' ') {
            table++;
        }

        char *startColumns = strstr(table, "(");
        char *endColumns = strstr(startColumns, ")");
        
        if (startColumns && endColumns) {
            startColumns++; 
            
            char columns[256];
            strncpy(columns, startColumns, endColumns - startColumns);
            columns[endColumns - startColumns] = '\0';

            char *colToken = strtok(columns, ",");
            while (colToken != NULL) {
                columnCount++;
                colToken = strtok(NULL, ",");
            }
        }

        char *startValues = strstr(values, "(");
        char *endValues = strstr(startValues, ")");

        if (startValues && endValues) {
            startValues++;
            
            char values[256];
            strncpy(values, startValues, endValues - startValues);
            values[endValues - startValues] = '\0';

            char *valToken = strtok(values, ",");
            while (valToken != NULL) {
                valueCount++;
                valToken = strtok(NULL, ",");
            }
        }

        if (columnCount != valueCount) {
            printf("Erreur : Le nombre de colonnes (%d) ne correspond pas au nombre de valeurs (%d).\n", columnCount, valueCount);
            return 0;  
        }

        return 1;  
    }

    printf("Erreur : Format de la commande SQL invalide.\n");
    return 0;  
}

int detectColumnType(BinaryTree *tree, char *databaseValue) {  // Fonction qui va recuperer le type d'une colonne present dans le nœu qui lui est associé en fonction de son nom
    

    char table[256], columnName[256], value[256];

    sscanf(databaseValue, "values.%255[^.].%255[^.].%255s", table, columnName, value); // Recuperation du nom de la colonne
    
    char nameOfColumnKey[256]; 
    snprintf(nameOfColumnKey, sizeof(nameOfColumnKey), "column.%s.%s", table, columnName);
                    
    long columnKey = createKey(nameOfColumnKey); // Creation de la clé pour recuperer le nœu qui lui est associé
                    
    Node *current = tree->root;
    char columnType[6];

    while (current != NULL) {
        if (current->columnData.key == columnKey) { 
            snprintf(columnType, sizeof(columnType), "%s", current->columnData.type); // Recuperation du type de la colonne dans le nœu
            break;
        }
        if (columnKey < current->columnData.key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (strcmp(columnType, "INT") == 0) {
        return 1;
    }
    if (strcmp(columnType, "FLOAT") == 0) {
        return 2;
    }
    if (strcmp(columnType, "CHAR") == 0) {
        return 3;
    }

    
}