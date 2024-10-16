#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sql.h"
#include "../include/file.h"
#include "../include/function.h"
#include "../include/database_structs.h"
#include "../include/node.h"

void sqlEntry(BinaryTree *tree, Database *db) {

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
                insert(db, sqlRest);
            } else if(strcmp(token, "SELECT") == 0) {


            } else if(strcmp(token, "DELETE") == 0) {


            } else if(strcmp(token, "UPDATE") == 0) {


            } else if(strcmp(token, "SHOW") == 0) {


            } else if(strcmp(token, "CREATE") == 0) {
                createTable(tree, db, sqlRest);
            } else if(strcmp(token, "EXIT") == 0) {
                x = 0;
            } else {
                printf("Commande SQL non conforme ou non reconnue\n");
            }

        }
    }

}


void insert(Database *db, char *sqlRest) {

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


void createTable(BinaryTree *tree, Database *db, char *sqlRest) {

    char *table = strstr(sqlRest, "TABLE");

    if(table != NULL) {
        table += strlen("TABLE");
        while (*table == ' ') {
            table++;
        }

        char tableName[256];
        char *startColumns = strstr(table, "(");
        char *endColumns = strstr(table, ")");

        if (startColumns != NULL && endColumns != NULL && endColumns > startColumns) {

            strncpy(tableName, table, startColumns - table);
            tableName[startColumns - table - 1] = '\0';

            char databaseTableName[270];
            snprintf(databaseTableName, sizeof(databaseTableName), "table.%s", tableName);


            if (writeInDatabase(databaseTableName) != EXIT_SUCCESS) {
                printf("Erreur lors de l'écriture du nom de la table dans la base de données.\n");
            }

            Node *newTableNode = createNode(TABLE_NODE); // On creer un nouveau node de TABLE
            strcpy(newTableNode->tableData.tableName, tableName); // On y met la nom de la table
            newTableNode->tableData.columns = NULL;
            newTableNode->tableData.columnCount = 0;
            db->tableCount++;

            insertNode(tree, newTableNode); // On l'insert dans l'arbre

            //displayNode(newTableNode);

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

                    char *dashPosition = strstr(databaseColumn, "-");
                    if (dashPosition != NULL) {
                        char columnName[100], columnType[100];
                        strncpy(columnName, databaseColumn, dashPosition - databaseColumn);
                        columnName[dashPosition - databaseColumn] = '\0';

                        strcpy(columnType, dashPosition + 1);


                        Node *newColumnNode = createNode(COLUMN_NODE);
                        strcpy(newColumnNode->columnData.columnName, columnName);
                        strcpy(newColumnNode->columnData.type, columnType);
                        newColumnNode->columnData.values = NULL;


                        newTableNode->tableData.columns = realloc(newTableNode->tableData.columns, sizeof(Column) * (newTableNode->tableData.columnCount + 1));
                        newTableNode->tableData.columns[newTableNode->tableData.columnCount] = newColumnNode->columnData;
                        newTableNode->tableData.columnCount++;

                        insertNode(tree, newColumnNode);

                        // displayNode(newColumnNode);

                        column = strtok(NULL, ","); // On récupère la prochaine colonne
                    }

                }

                //displayTree(tree);


            }else {
                printf("Valeur non trouvée ou mal formatée.\n");
            }
        }else{
            printf("Requete non trouvée ou mal formatée.\n");
        }
    }else{
        printf("TABLE non trouvé dans la requête.\n");
    }
}


/* void delete(char *sqlRest) {

    char *from = strstr(sqlRest, "FROM");

    if(from != NULL) {
        from += strlen("FROM");
        while (*from == ' ') {
            from++;
        }

        char tableName[256];
        char *endTableName = strstr(from, "WHERE");

        int8_t allData = 0;

        if (endTableName == NULL) {
            allData = 1; // Vu que il n'y a pas de WHERE toute les donnée de la table seront suprimmer
            endTableName = from + strlen(from); // On va a la fin de la chaine
        }

        strncpy(tableName, from, endTableName - from);
        tableName[endTableName - from - 1] = '\0';

        if (allData){

        }else{

        }



        printf("%s\n",from);
        printf("%s",tableName);

    }
}

*/