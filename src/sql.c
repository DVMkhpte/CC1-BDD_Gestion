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
                insert(tree, db, sqlRest);
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


void insert(BinaryTree *tree, Database *db, char *sqlRest) {
    char *table = strstr(sqlRest, "INTO");
    char *values = strstr(sqlRest, "VALUES");

    if (table != NULL && values != NULL) {
        table += strlen("INTO");
        while (*table == ' ') {
            table++;
        }

        char tableName[256];
        char *startTable = strstr(table, "(");

        if (startTable != NULL) {
            strncpy(tableName, table, startTable - table - 1);
            tableName[startTable - table - 1] = '\0';

            char nameOfTableKey[256]; 
            snprintf(nameOfTableKey, sizeof(nameOfTableKey), "table.%s", tableName);

            long tableKey = createKey(nameOfTableKey); 

            Node *current = tree->root;
            int8_t tableExists = 0;
            
            while (current != NULL) {
                if (current->tableData.key == tableKey) {
                    tableExists = 1;
                    break;
                }
                if (tableKey < current->tableData.key) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }

            if (!tableExists) {
                printf("Erreur : La table %s n'existe pas.\n", tableName);
                return;
            }

            char *startColumns = startTable + 1;
            char *endColumns = strstr(startColumns, ")");

            char **columns = malloc(1 * sizeof(char*));
            if (columns == NULL) {
                printf("Échec de l'allocation de mémoire");
                return EXIT_FAILURE;
            }   

              int8_t columnCount = 0;
              int8_t size = 1;

            if (endColumns != NULL && endColumns > startColumns) {
                char columnNames[256];
                strncpy(columnNames, startColumns, endColumns - startColumns);
                columnNames[endColumns - startColumns] = '\0';

                char *column = strtok(columnNames, ",");
                while (column != NULL) {
                    while (*column == ' ') column++;

                    if (columnCount >= size) {
                        size *= 2; 
                        columns = realloc(columns, size * sizeof(char*));
                    if (columns == NULL) {
                        printf("Échec de la réallocation de mémoire");
                        return EXIT_FAILURE;
                    }
                }

                columns[columnCount] = strdup(column); 
                columnCount++; 
                for (int i = 0; i < columnCount; i++) {
                    printf("Nom de colonne dans le tableau : %s\n", columns[i]);
                }
                

                

                    char nameOfColumnKey[256]; 
                    snprintf(nameOfColumnKey, sizeof(nameOfColumnKey), "column.%s.%s", tableName, column);
                    
                    long columnKey = createKey(nameOfColumnKey); 
                    
                    current = tree->root;
                    int8_t columnExists = 0;
                    

                    while (current != NULL) {
                        if (current->columnData.key == columnKey) {  
                            columnExists = 1;
                            break;
                        }
                        if (columnKey < current->columnData.key) {
                            current = current->left;
                        } else {
                            current = current->right;
                        }
                    }

                    if (!columnExists) {
                        printf("Erreur : La colonne %s n'existe pas dans la table %s.\n", column, tableName);
                        return;
                    }

                    column = strtok(NULL, ",");
                }
            }

            char *startValue = strstr(values, "(");
            char *endValue = strstr(values, ")");

            if (startValue != NULL && endValue != NULL && endValue > startValue) {
                char values[256];
                strncpy(values, startValue + 1, endValue - startValue - 1);
                values[endValue - startValue - 1] = '\0';

                char *value = strtok(values, ",");

                int8_t valueCount = 0;
                while (value != NULL) {
                    while (*value == ' ') value++;

                    char databaseValue[270];
                    snprintf(databaseValue, sizeof(databaseValue), "values.%s.%s.%s", tableName,columns[valueCount],value);

                    if (writeInDatabase(databaseValue) != EXIT_SUCCESS) {
                        printf("Erreur lors de l'écriture des valeurs dans la base de données.\n");
                    }

                    ValueType valueType = detectValueType(value);
                    Node *newValueNode;

                    if (valueType == INT_VALUE) {
                        int intValue = strtol(value, NULL, 10);
                        newValueNode = createNode(VALUE_NODE, columns[valueCount], INT_VALUE, &intValue);
                    } else if (valueType == FLOAT_VALUE) {
                        float floatValue = strtof(value, NULL);
                        newValueNode = createNode(VALUE_NODE, columns[valueCount], FLOAT_VALUE, &floatValue);
                    } else if (valueType == STRING_VALUE) {
                        newValueNode = createNode(VALUE_NODE, columns[valueCount], STRING_VALUE, value);
                    }

                    insertNode(tree,newValueNode);
                    displayTree(tree);

                   value = strtok(NULL, ",");
                   valueCount ++;
                }
                
            } else {
                printf("Valeur non trouvée ou mal formatée.\n");
            }
            
            for (int i = 0; i < columnCount; i++) {
                free(columns[i]);  
            }
            free(columns); 
        }
    
    } else {
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

            char databaseTableName[256];
            snprintf(databaseTableName, sizeof(databaseTableName), "table.%s", tableName);


            if (writeInDatabase(databaseTableName) != EXIT_SUCCESS) {
                printf("Erreur lors de l'écriture du nom de la table dans la base de données.\n");
            }

            Node *newTableNode = createNode(TABLE_NODE, databaseTableName, INT_VALUE, NULL); // On creer un nouveau node de TABLE
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
                    if (strlen(tableName) + strlen(column) + 8 < sizeof(databaseColumn)) {
                        snprintf(databaseColumn, sizeof(databaseColumn), "column.%s.%s", tableName, column);
                    } else {
                        fprintf(stderr, "Erreur : le nom combiné de la table et de la colonne est trop long.\n");
                    }
                    
                    if (writeInDatabase(databaseColumn) != EXIT_SUCCESS) {
                        printf("Erreur lors de l'écriture des colonnes de la table dans la base de données.\n");
                    }

                    char *dashPosition = strstr(databaseColumn, "-");
                    if (dashPosition != NULL) {
                        char columnName[100], columnType[100];
                        strncpy(columnName, databaseColumn, dashPosition - databaseColumn);
                        columnName[dashPosition - databaseColumn] = '\0';

                        strcpy(columnType, dashPosition + 1);


                        Node *newColumnNode = createNode(COLUMN_NODE, columnName, INT_VALUE, NULL);
                        strcpy(newColumnNode->columnData.columnName, columnName);
                        strcpy(newColumnNode->columnData.type, columnType);
                        newColumnNode->columnData.values = NULL;


                        newTableNode->tableData.columns = realloc(newTableNode->tableData.columns, sizeof(Column) * (newTableNode->tableData.columnCount + 1));
                        newTableNode->tableData.columns[newTableNode->tableData.columnCount] = newColumnNode->columnData;
                        newTableNode->tableData.columnCount++;

                        insertNode(tree, newColumnNode);

                        //displayNode(newColumnNode);

                        column = strtok(NULL, ","); // On récupère la prochaine colonne
                    }

                }

                //displayTree(tree);
                //generateGraph(tree);

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