#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sql.h"
#include "../include/file.h"
#include "../include/function.h"

void sqlEntry() {

    int8_t x = 1;

    while(x == 1){

        printf("Commande SQL : ");

        char sql[256];
        fgets(sql, 255, stdin);
        sql[strcspn(sql, "\n")] = 0;

        char *token = strtok(sql, " ");
        char *sqlRest = strtok(NULL, "");


        if (token != NULL) {
            if(strcmp(token, "INSERT") == 0) {
                insert(sqlRest);
            } else if(strcmp(token, "SELECT") == 0) {


            } else if(strcmp(token, "DELETE") == 0) {


            } else if(strcmp(token, "UPDATE") == 0) {


            } else if(strcmp(token, "SHOW") == 0) {


            } else if(strcmp(token, "CREATE") == 0) {
                createTable(sqlRest);
            } else if(strcmp(token, "EXIT") == 0) {
                x = 0;
            } else {
                printf("Commande SQL non conforme ou non reconnue\n");
            }

        }
    }

}


void insert(char *sqlRest) {
    char *values = strstr(sqlRest, "VALUES");

    if(values != NULL) {
        char *startValue = strstr(values, "(");
        char *endValue = strstr(values, ")");

        if (startValue != NULL && endValue != NULL && endValue > startValue) {
            char value[256];
            strncpy(value, startValue + 1, endValue - startValue - 1);


            value[endValue - startValue - 1] = '\0';
            char databaseValue[270];
            snprintf(databaseValue, sizeof(databaseValue), "values.%s", value);

            if (writeInDatabase(databaseValue) != EXIT_SUCCESS) {
                printf("Erreur lors de l'écriture des valeurs dans la base de données.\n");
            }
        } else {
            printf("Valeur non trouvée ou mal formatée.\n");
        }

    }else{
        printf("VALUES non trouvé dans la requête.\n");
    }
}


void createTable(char *sqlRest) {

    char *table = strstr(sqlRest, "TABLE");

    if(table != NULL) {
        table += strlen("TABLE");
        while (*table == ' ') {
            table++;
        }

        char tableName[256];
        char *startColumns = strstr(table, "(");
        char *endColumns = strstr(table, ")");

        strncpy(tableName, table, startColumns - table);
        tableName[startColumns - table - 1] = '\0';

        char databaseTableName[270];
        snprintf(databaseTableName, sizeof(databaseTableName), "table.%s", tableName);


        if (writeInDatabase(databaseTableName) != EXIT_SUCCESS) {
            printf("Erreur lors de l'écriture du nom de la table dans la base de données.\n");
        }


        if (endColumns != NULL) {
            startColumns++;
            *endColumns = '\0';

            char *column = strtok(startColumns, ",");
            while (column != NULL) {
                while (*column == ' ') column++; // On saute les espaces
                replaceSpacesToDashes(column);

                char databaseColumn[256];
                snprintf(databaseColumn, sizeof(databaseColumn), "column.%s.%s", tableName,column);

                if (writeInDatabase(databaseColumn) != EXIT_SUCCESS) {
                    printf("Erreur lors de l'écriture des colonnes de la table dans la base de données.\n");
                }

                column = strtok(NULL, ","); // On récupère la prochaine colonne
            }
        }else {
            printf("Valeur non trouvée ou mal formatée.\n");
        }
    }else{
        printf("TABLE non trouvé dans la requête.\n");
    }
}