#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sql.h"
#include "../include/file.h"
#include "../include/function.h"
#include "../include/database_structs.h"
#include "../include/node.h"


void sqlEntry(BinaryTree *tree, Database *db) { // Fonction qui recupere la commande sql et qui l'etudie pour lancer la bonne fonction
   
    int8_t x = 1;
    
    while(x == 1){
    
        printf("Commande SQL : ");

        char sql[256];
        fgets(sql, 255, stdin);
        sql[strcspn(sql, "\n")] = 0;

        char *token = strtok(sql, " "); // On recupere le premier mot pour la comparaison
        char *sqlRest = strtok(NULL, "");


        if (token != NULL) {
            if(strcmp(token, "INSERT") == 0) {
                if (verifyInsert(sqlRest)) insert(tree, db, sqlRest);
            } else if(strcmp(token, "SELECT") == 0) {
                selectt(tree, sqlRest);
            } else if(strcmp(token, "DELETE") == 0) {
                delete(tree,db,sqlRest);
            } else if(strcmp(token, "SHOW") == 0) {
                show(tree);
            } else if(strcmp(token, "CREATE") == 0) {
                createTable(tree, db, sqlRest);
            } else if(strcmp(token, "DROP") == 0) {
               dropTable(tree, db, sqlRest);
            } else if(strcmp(token, "EXIT") == 0) {
                x = 0;
            } else {
                printf("Commande SQL non conforme ou non reconnue\n");
            }

        }
    }
    freeTree(tree->root); // On supprime l'arbre a la sortie du programme
}


void insert(BinaryTree *tree, Database *db, char *sqlRest) { // Fonction d'insert de valeur dans la base de données
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
            tableName[startTable - table - 1] = '\0';  // Récupèration du nom de la table dans la commande SQL

            long tableKey = createKey(tableName); // Et création de la clé avec ce nom
            
            Node *current = tree->root;
            int8_t tableExists = 0;
            
            while (current != NULL) { // On vérifie que la table existe bien
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
            char *endColumns = strstr(startColumns, ")");  // On récupère toutes les colonnes de la commande SQL

            char **columns = malloc(1 * sizeof(char*));  // On crée aussi un tableau qui sera utilisé pour creer les valeurs
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

                char *column = strtok(columnNames, ","); // On sépare chaque colonne
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
                
                    char nameOfColumnKey[256]; 
                    snprintf(nameOfColumnKey, sizeof(nameOfColumnKey), "column.%s.%s", tableName, column);
                    
                    long columnKey = createKey(nameOfColumnKey); 
                    
                    current = tree->root;
                    int8_t columnExists = 0;
                    

                    while (current != NULL) {  // On vérifie que les colonnes existent dans la table
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

                char *value = strtok(values, ","); // On récupère les valeurs a ajouter dans la commande SQL

                int8_t valueCount = 0;
                while (value != NULL) {
                    while (*value == ' ') value++;

                    ValueType valueType = detectValueType(value);  // On detecte le type de la valeur
                    Node *newValueNode;
                    
                    char databaseValue[270];
                    
                    if (valueType == INT_VALUE) { 
                        
                        snprintf(databaseValue, sizeof(databaseValue), "values.%s.%s.%s", tableName,columns[valueCount],value); // On creer la valeur a ajouter dans le fichier texte
                        
                        if (detectColumnType(tree, databaseValue) != 1){  // On vérifie que la colonne est de type entier
                            printf("Erreur : La valeur %s ne correspond pas au type de la colonne %s.\n", value, columns[valueCount]);
                            return;
                         }
                           
                        if (writeInDatabase(databaseValue) != EXIT_SUCCESS) { // Et on ecrit dans le fichier texte la valeur a ajouter
                            printf("Erreur lors de l'écriture des valeurs dans la base de données.\n");
                        }
                        
                        int intValue = atoi(value);
                        newValueNode = createNode(VALUE_NODE, databaseValue, INT_VALUE, &intValue); // Puis pour finir on cree un nouveau node avec la valeur a ajouter
                    
                    } else if (valueType == FLOAT_VALUE) {  // Pareil mais pour un FLOAT
                        
                        float floatValue = atof(value);
                        snprintf(databaseValue, sizeof(databaseValue), "values.%s.%s.%f", tableName,columns[valueCount],floatValue);
                        
                        if (detectColumnType(tree, databaseValue) != 2){
                            printf("Erreur : La valeur %s ne correspond pas au type de la colonne %s.\n", value, columns[valueCount]);
                            return;
                         }
                        
                        if (writeInDatabase(databaseValue) != EXIT_SUCCESS) {
                            printf("Erreur lors de l'écriture des valeurs dans la base de données.\n");
                        }   
                        newValueNode = createNode(VALUE_NODE, databaseValue, FLOAT_VALUE, &floatValue);
                    
                    } else if (valueType == STRING_VALUE) {  // Pareil mais pour un STRING
                        
                        snprintf(databaseValue, sizeof(databaseValue), "values.%s.%s.%s", tableName,columns[valueCount],value);
                        
                        if (detectColumnType(tree, databaseValue) != 3){
                            printf("Erreur : La valeur %s ne correspond pas au type de la colonne %s.\n", value, columns[valueCount]);
                            return;
                         }
                        
                        
                        if (writeInDatabase(databaseValue) != EXIT_SUCCESS) {
                            printf("Erreur lors de l'écriture des valeurs dans la base de données.\n");
                        }
                        newValueNode = createNode(VALUE_NODE, databaseValue, STRING_VALUE, &value);
                    }

                    insertNode(tree,newValueNode);  // Et pour finir on insere le nouveau node dans l'arbre
                    

                   value = strtok(NULL, ",");
                   valueCount ++;
                }
                
            } else {
                printf("Valeur non trouvée ou mal formatée.\n");
            }
            
            for (int i = 0; i < columnCount; i++) {
                free(columns[i]);  // On libère le tableau utiliser pour stocker le nom des colonnes
            }
            free(columns); 
        }
    
    } else {
        printf("VALUES non trouvé dans la requête.\n");
    }
    
}


void createTable(BinaryTree *tree, Database *db, char *sqlRest) {  // Fonction pour creer une table ainsi que ces colonnes

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
            tableName[startColumns - table - 1] = '\0';  // On recupère le nom de la table

            long tableKey = createKey(tableName); 

            Node *current = tree->root;
            int8_t tableExists = 0;
            
            while (current != NULL) {  // Et on verifie si la table n'existe pas deja
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

            if (tableExists) {
                printf("Erreur : Une table avec ce nom existe deja.\n");
                return;
            }

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

            if (endColumns != NULL) {
                startColumns++;
                *endColumns = '\0';

                char *column = strtok(startColumns, ","); // On récupère les colonnes
                while (column != NULL) {
                    while (*column == ' ') column++; // On saute les espaces
                    replaceSpacesToDashes(column);  // Si il y a des espaces on les remplace par des tirets

                    char databaseColumn[256];
                    if (strlen(tableName) + strlen(column) + 8 < sizeof(databaseColumn)) {
                        snprintf(databaseColumn, sizeof(databaseColumn), "column.%s.%s", tableName, column); 
                    } else {
                        fprintf(stderr, "Erreur : le nom combiné de la table et de la colonne est trop long.\n");
                    }
                    
                    if (writeInDatabase(databaseColumn) != EXIT_SUCCESS) {  // ON les ecrient dans la base de donnees
                        printf("Erreur lors de l'écriture des colonnes de la table dans la base de données.\n");
                    }

                    char *dashPosition = strstr(databaseColumn, "-");
                    if (dashPosition != NULL) {
                        char columnName[100], columnType[100];
                        strncpy(columnName, databaseColumn, dashPosition - databaseColumn);
                        columnName[dashPosition - databaseColumn] = '\0';

                        strcpy(columnType, dashPosition + 1); // On récupère le type de la colonne dans la commande SQL

                        if (strcmp(columnType, "INT ") == 0 || strcmp(columnType, "FLOAT ") == 0 || strcmp(columnType, "CHAR ") == 0) {
                            printf("Type de colonne invalide pour la colonne %s.\n", columnName);
                            deleteTableFromFile(tree,tableName);  // Si le type n'est pas bon on supprime la table puisque les colonne qui lui sont associer ne sont pas correcte
                            return;
                        }


                        Node *newColumnNode = createNode(COLUMN_NODE, columnName, INT_VALUE, NULL);  // Creation du nœu de la colonne
                        strcpy(newColumnNode->columnData.columnName, columnName);
                        strcpy(newColumnNode->columnData.type, columnType);
                        newColumnNode->columnData.values = NULL;


                        newTableNode->tableData.columns = realloc(newTableNode->tableData.columns, sizeof(Column) * (newTableNode->tableData.columnCount + 1));
                        newTableNode->tableData.columns[newTableNode->tableData.columnCount] = newColumnNode->columnData;
                        newTableNode->tableData.columnCount++;

                        insertNode(tree, newColumnNode);  // Et on l'insert dans l'arbre

                        column = strtok(NULL, ","); // On récupère la prochaine colonne
                    }

                }

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


void delete(BinaryTree *tree, Database *db, char *sqlRest) {  // Fonction pour supprimer une ou des valeurs de la base de donnée

    char *from = strstr(sqlRest, "FROM");

    if(from != NULL) {
        from += strlen("FROM");
        while (*from == ' ') {
            from++;
        }

        char tableName[256];
        char *endTableName = strstr(from, "WHERE");
        int8_t allData = (endTableName == NULL);  // Vu que il n'y a pas de WHERE toute les donnée de la table seront suprimmer


        if(allData) {
            endTableName = from + strlen(from); // Et on va a la fin de la chaine
        }

        strncpy(tableName, from, endTableName - from);
        tableName[endTableName - from - 1] = '\0';

            long tableKey = createKey(tableName); 

            Node *current = tree->root;
            int8_t tableExists = 0;
            
            while (current != NULL) { // On regarde bien si la table existe dans l'arbre
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

        if (allData){  // Si il n'y a pas de WHERE on supprime toute les données de la table
            deleteValuesFromFile(tree,tableName);
        } else {  // Sino on recupere la condition, on la split et on supprime les données qui la remplissent
            endTableName += strlen("WHERE");
            while (*endTableName == ' ') endTableName++; 
            
            char condition[256];
            strncpy(condition, endTableName, sizeof(condition) - 1);
            condition[sizeof(condition) - 1] = '\0'; 

            size_t len = strlen(condition);
            if (len > 0 && condition[len - 1] == ';') {
                condition[len - 1] = '\0'; 
            }

            char *column = strtok(condition, " ");
            char *operator = strtok(NULL, " ");
            char *value = strtok(NULL, " ");

            if (column && operator && value) {

                char nameOfColumnKey[256]; 
                snprintf(nameOfColumnKey, sizeof(nameOfColumnKey), "column.%s.%s", tableName, column);
                
                long columnKey = createKey(nameOfColumnKey);  // ON verifie que la colonne de la condition existe bien
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
                    printf("Erreur : La colonne %s presente dans la condition n'existe pas dans la table %s.\n", column, tableName);
                    return;
                }

                ValueType valueType = detectValueType(value);
                deleteValuesFromFileWithCondition(tree,tableName,column,operator,value,valueType);
                
                } else {
                printf("Condition mal formée.\n");
            }

           
        }


    } else {
        printf("Mauvais formatage de la requete : FROM manquant !!\n");
    }
}

void dropTable(BinaryTree *tree, Database *db, char *sqlRest) {  // Fonction pour supprimer une table ainsi que toutes les données qui y sont stockées

    char *table = strstr(sqlRest, "TABLE");

    if(table != NULL) {
        table += strlen("TABLE");
        while (*table == ' ') {
            table++;
        }

       size_t len = strlen(table);
            if (len > 0 && table[len - 1] == ';') {
                table[len - 1] = '\0'; 
            }

       long tableKey = createKey(table); 

            Node *current = tree->root;
            int8_t tableExists = 0;
            
            while (current != NULL) {  // ON vérifie que la table existe dans l'arbre
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
                printf("Erreur : La table %s n'existe pas.\n", table);
                return;
            }
            deleteTableFromFile(tree, table);  // Et on appelle la fonction de suppression

    } else {
        printf("Erreur : Mauvais formatage de la requete : TABLE manquant !!\n");
    }
}

void show(BinaryTree *tree) { // Fonction pour afficher les tables ainsi que leurs colonne associées dans l'arbre
    if (tree->root == NULL) {
        printf("L'arbre est vide.\n");
        return;
    }

    char currentTable[256] = "";  
    printf("+----------------------+\n");
    printf("|   Liste des tables   |\n");
    printf("+----------------------+\n");

    showNode(tree->root, currentTable); // Appelle de la fonction qui va afficher chaque nœu nous interessant
    printf("\nFin de l'affichage des tables.\n");
}

void showNode(Node *node, char *currentTable) { // Fonction récursive qui va afficher chaque nœu nous interessant
    if (node == NULL) return;

    showNode(node->left, currentTable);

    if (node->type == TABLE_NODE) {
        strcpy(currentTable, node->tableData.tableName);  
        printf("\nTable: %s\n", currentTable);
        printf("Colonnes:\n");
    } else if (node->type == COLUMN_NODE) {
        char fullColumnName[512];
        snprintf(fullColumnName, sizeof(fullColumnName), "column.%s.%s", currentTable, node->columnData.columnName);
        
        long columnKey = createKey(fullColumnName);
        
        if (node->columnData.key == columnKey) {
            printf(" - %s (%s)\n", node->columnData.columnName, node->columnData.type);
        }
    }

    showNode(node->right, currentTable);
}

void selectt(BinaryTree *tree, char *sqlRest) {  // Fonction qui va afficher les valeurs d'une colonne
    char *from = strstr(sqlRest, "FROM");
    
    if (from != NULL) {
        from += strlen("FROM");
        while (*from == ' ') {
            from++;
        }

        char tableName[256];
        char *endTableName = strstr(from, "FOR");

        if (endTableName == NULL) {
            printf("Erreur : Syntaxe incorrecte. Utilisez 'SELECT * FROM <table_name> FOR <column_name>'.\n");
            return;
        }

        strncpy(tableName, from, endTableName - from - 1);
        tableName[endTableName - from] = '\0';

        endTableName += strlen("FOR");
        while (*endTableName == ' ') endTableName++;
        
        char columnName[256];
        strncpy(columnName, endTableName, sizeof(columnName) - 1);
        columnName[sizeof(columnName) - 1] = '\0';

        size_t len = strlen(columnName);
            if (len > 0 && columnName[len - 1] == ';') {
                columnName[len - 1] = '\0'; 
            }

        long tableKey = createKey(tableName);
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

        char fullColumnName[512];
        snprintf(fullColumnName, sizeof(fullColumnName), "column.%s.%s", tableName, columnName);

        long columnKey = createKey(fullColumnName);
        current = tree->root;
        int8_t columnExists = 0;

        while (current != NULL) {
            if (current->type == COLUMN_NODE && current->columnData.key == columnKey) {
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
            printf("Erreur : La colonne %s n'existe pas dans la table %s.\n", columnName, tableName);
            return;
        }

        if (tree->root == NULL) {
        printf("L'arbre est vide.\n");
        return;
        }

        printf("Liste des valeur pour la table %s et la colonne %s \n", tableName, columnName);
        
        showValue(tree,tree->root, tableName, columnName);
        printf("\nFin de l'affichage des tables.\n");
    } else {
        printf("Erreur : Mauvais formatage de la requete : FROM manquant !!\n");
    }
}

void showValue(BinaryTree *tree, Node *node, char *tableName, char *columnName) { // Fonction qui va afficher chaque nœeu VALUE nous interressant

    if (node == NULL) return;

    showValue(tree, node->left, tableName, columnName);

    if (node->type == VALUE_NODE) {
        char fullValueName[512];
        if (node->typeV == INT_VALUE) {
            snprintf(fullValueName, sizeof(fullValueName), "values.%s.%s.%d", tableName, columnName, node->valueData.data.intValue);
        } else if (node->typeV == FLOAT_VALUE) {
            snprintf(fullValueName, sizeof(fullValueName), "values.%s.%s.%f", tableName, columnName, node->valueData.data.floatValue);
        } else if (node->typeV == STRING_VALUE) {
            snprintf(fullValueName, sizeof(fullValueName), "values.%s.%s.%s", tableName, columnName, node->valueData.data.stringValue);
        }

        long valueKey = createKey(fullValueName);

        Node *current = tree->root;

        while (current != NULL) {
            if (current->valueData.key == valueKey) {

                printf("Valeur: ");
                if (node->typeV == INT_VALUE) {
                    printf("%d\n", node->valueData.data.intValue);  
                } else if (node->typeV == STRING_VALUE) {
                    printf("%s\n", node->valueData.data.stringValue);  
                } else if (node->typeV == FLOAT_VALUE) {
                    printf("%f\n", node->valueData.data.floatValue);  
                }
            }
            if (valueKey < current->tableData.key || valueKey < current->columnData.key || valueKey < current->valueData.key) {
                current = current->left;
            } else {
                current = current->right;
            }

        }
    }   

    showValue(tree, node->right, tableName, columnName);
}