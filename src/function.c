#include <stdio.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include "../include/function.h"
#include "../include/database_structs.h"
#include "../include/file.h"
#include "../include/node.h"

void replaceSpacesToDashes(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '-';
        }
    }
}

long  createKey(char *str) {
    long  sum = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        sum += (int)str[i]; // On additionne toutes les valeurs ascii dans sum
    }

    printf("Key creer pour %s: %ld\n", str, sum);

    
    return sum;
}

ValueType detectValueType(char *value) {
    
    char *endptr;
    strtol(value, &endptr, 10);
    if (*endptr == '\0') {
        return INT_VALUE;
    }

    strtof(value, &endptr);
    if (*endptr == '\0' && strchr(value, '.')) {
        printf("float detecter\n");
        return FLOAT_VALUE;
    }
        printf("str detecter\n");
        return STRING_VALUE;
    

    
}


void analyseCondition(BinaryTree *tree,char *tableName, char *column, char *operator, char *value) {
    ValueType valueType = detectValueType(value);

        deleteValuesFromFileWithConditionInt(tree,tableName,column,operator,value);
    
    if (valueType == STRING_VALUE) printf("Value str");
    if (valueType == FLOAT_VALUE) printf("Value float");

}

int verifyInsert(char *sqlRest) {
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