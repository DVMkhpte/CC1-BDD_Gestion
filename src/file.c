#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "../include/file.h"
#include "../include/menu.h"
#include "../include/sql.h"
#include "../include/database_structs.h"
#include "../include/function.h"


FILE *file = NULL;
char *fileName = NULL;

int verifFileExist(char *filename) {  // Ouverture du fichier de la base de donnée dans la variable file 

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    file = fopen(filepath, "r");

    if (file) {   // Tout en vérifiant quelle n'existe pas deja
        printf("Une base de donnée existe deja sous ce nom.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

int verifFileExistW(char *filename) {  // Ouverture du fichier de la base de donnée dans la variable file 

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    file = fopen(filepath, "r");

    if (!file) {   // Tout en vérifiant quelle existe pas deja
        printf("Aucune base de donnée n'existe sous ce nom\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }else{
        fclose(file);
        file = fopen(filepath, "a"); // Si elle existe bien on peut du coup la USE 
    }
}

int verifFileExistD(char *filename) {  // Ouverture du fichier de la base de donnée dans la variable file

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    file = fopen(filepath, "r");

    if (file) {  // Et on la supprime si elle existe bien
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


void createDatabase(char *filename) {  // Fonction pour créer une nouvelle base de donnée ainsi qu'un nouvelle arbre

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

            Database *db = malloc(sizeof(Database)); // Creation de la variable de la base de donnée puis on l'initialise avec initDatabase()
            if (db == NULL) {
                printf("Erreur lors de l'allocation de la mémoire pour la base de données.\n");
                exit(EXIT_FAILURE);
            }

            initDatabase(db,filename);

            printf("Vous utilisez maintenant la base de données ' %s '.\n", filename);
            x = 0;

            BinaryTree tree;   // Creation de la variable de l'arbre puis on l'initialise avec initTree()
            initBinaryTree(&tree);

            fileName = filename;  // On donne a fileName (une variagle globale au fichier) le nom de la database

            createSecondMenu(); // Appelle de la fonction pour afficher le menu des commandes SQL ainsi que la fonction d'écoute de ces commandes
            sqlEntry(&tree, db); 

        } else if (strcmp(res, "N") == 0 || strcmp(res, "n") == 0 || strcmp(res, "non") == 0 || strcmp(res, "Non") == 0 || strcmp(res, "NON") == 0) {
            return EXIT_SUCCESS;
        } else {
            printf("Réponse invalide.\n");
        }
    }
    
}


void loadDatabase(char *filename) {  // Fonction pour creer un arbre binaire en fonction du fichier texte de la base de donnée

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", filename);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture de la base de donnée\n");
        exit(EXIT_FAILURE);
    }

    BinaryTree tree;  // Creation de la variable de l'arbre puis on l'initialise avec initTree()
    initBinaryTree(&tree);

    Database *db = malloc(sizeof(Database));  // Creation de la variable de la base de donnée puis on l'initialise avec initDatabase()
    if (db == NULL) {
        printf("Erreur lors de l'allocation de la mémoire pour la base de données.\n");
        exit(EXIT_FAILURE);
    }

    initDatabase(db, filename);

    char line[512];
    Node *tableNode = NULL;

    while (fgets(line, sizeof(line), file) != NULL) { // Pour chaque ligne du fichier texte un nœu sera creer et ajouter a l'arbre
        line[strcspn(line, "\n")] = '\0';

        if (strncmp(line, "table.", 6) == 0) {
            char tableName[256];
            sscanf(line, "table.%255s", tableName);
            
            Node *tableNode = createNode(TABLE_NODE, tableName, INT_VALUE, NULL); // Creation d'un nœu de type TABLE
            strcpy(tableNode->tableData.tableName, tableName); 
            tableNode->tableData.columns = NULL;
            tableNode->tableData.columnCount = 0;
            db->tableCount++;
            
            insertNode(&tree, tableNode); // Et son insertion dans l'arbre
         
        }
        else if (strncmp(line, "column.", 7) == 0) {  // Creation d'un nœu de type COLUMN
            char tableName[256], columnName[256], columnType[100];
            sscanf(line, "column.%255[^.].%255[^-]-%99s", tableName, columnName, columnType);

            char fullColumnName[512];
            snprintf(fullColumnName, sizeof(fullColumnName), "column.%s.%s", tableName, columnName);

            Node *columnNode = createNode(COLUMN_NODE, fullColumnName, INT_VALUE, NULL);
            strcpy(columnNode->columnData.columnName, columnName);
            strcpy(columnNode->columnData.type, columnType);

            insertNode(&tree, columnNode);  // Et son insertion dans l'arbre
         
        }
        else if (strncmp(line, "values.", 7) == 0) {  // Creation d'un nœu de type VALUES
            char tableName[256], columnName[256], value[256];
            sscanf(line, "values.%255[^.].%255[^.].%255s", tableName, columnName, value);

            ValueType valueType = detectValueType(value); // On detecte le type de la valeur INT / FLOAT / STRING
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

            insertNode(&tree, valueNode); // Et son insertion dans l'arbre
        }
         
    }

    fclose(file);
    printf("Base de données chargée avec succès depuis le fichier %s\n", filepath);

    fileName = filename;  // On donne a fileName (une variagle globale au fichier) le nom de la database

    createSecondMenu();
    sqlEntry(&tree, db);
}



int writeInDatabase(char *values) {
    fprintf(file, "\n%s", values);
    fflush(file); // Permet de vider le tampon
    return EXIT_SUCCESS;
}

void deleteValuesFromFile(BinaryTree *tree, char *tableName) { // Fonction permettant de supprimer les valeurs d'un fichier et le nœu qui lui est associer
    
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "database/%s", fileName);

    file = fopen(filepath, "r");  
    if (!file) {
        printf("Erreur lors de l'ouverture de la base de donnée\n");
        return;
    }
    
    FILE *tempFile = fopen("database/temp.txt", "w"); // Creation d'un ficher temporaire qui sera par la suite notre nouveau ficher de base de donnée 
    if (!tempFile) {
        printf("Erreur lors de la création du fichier temporaire\n");
        fclose(file);
        return;
    }

    char line[512];
    char valuesPrefix[512];
    int8_t isValue = 0;
    snprintf(valuesPrefix, sizeof(valuesPrefix), "values.%s", tableName); // Prefix pour les valeurs de la table a trouver dans le fichier texte

    while (fgets(line, sizeof(line), file) != NULL) { // Pour chaque ligne du fichier texte
        
        if (strncmp(line, valuesPrefix, strlen(valuesPrefix)) != 0) { // Si la ligne n'est pas une valeur de la table a supprimer
            fputs(line, tempFile);  // On l'ecrit dans le fichier temporaire
        }else{                      // Sinon on cherche le nœu qui lui est associer pour le supprimer
            isValue++;
            char tableName[256], columnName[256], value[256];
            line[strcspn(line, "\n")] = '\0';
            
            long valueKey = createKey(line); // Creation d'une clé pour la valeur
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

    if (isValue  == 0) printf("Aucune valeur de la table %s n'a été supprimée puisque elle est vide.\n", tableName);
    else printf("Toutes les valeurs de la table '%s' ont été supprimées\n", tableName);

    fclose(file);
    fclose(tempFile);

    remove(filepath);
    rename("database/temp.txt", filepath); // Le ficher temporaire devient notre nouveau fichier de base de donnée

}

void deleteValuesFromFileWithCondition(BinaryTree *tree, char *tableName, char *column, char *operator, char *value, ValueType typeV) { // Meme fonction sauf que on a une condition pour supprimer une ou les valeurs
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

    char line[512];  // Creation des differentes variable pour les valeurs de la ligne et les valeurs de la condition
    char valuesPrefix[512];
    char lineValue[256];
    int isValue = 0; 
    int targetValueInt = 0;
    float targetValueFloat = 0.0f;

    if (typeV == INT_VALUE) { // Si la valeur dans la condition est un entier on la convertie en int     char* -> int
        targetValueInt = atoi(value);
    } else if (typeV == FLOAT_VALUE) { // Si la valeur dans la condition est un float on la convertie en float     char* -> float
        targetValueFloat = atof(value);
    }
    
    if (strcmp(operator, "=") == 0) {  // Suppression des valeurs avec une condition égale
        snprintf(valuesPrefix, sizeof(valuesPrefix), "values.%s.%s.%s", tableName, column, value); // Creation du prefix a chercher dans le fichier texte

        while (fgets(line, sizeof(line), file) != NULL) {  // Pour chaque ligne du fichier texte
            if (strncmp(line, valuesPrefix, strlen(valuesPrefix)) != 0) {  // Si la ligne n'est pas une valeur de la table a supprimer
                fputs(line, tempFile);  // On l'ecrit dans le fichier temporaire
            } else {                    // Sinon on cherche le nœu qui lui est assicier pour le supprimer
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
    } else if (strcmp(operator, "<") == 0) { // Suppression des valeurs avec une condition inférieur à la valeur cible
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, "values", 6) == 0) {  
                sscanf(line, "values.%*[^.].%*[^.].%s", lineValue);  // Cette fois on cherche la valeur de la ligne pour la comparer a notre valeur cible de notre condition
                int currentValue = atoi(lineValue);

                if (typeV == INT_VALUE) { // On le fait d'abord pour le type INT
                    if (currentValue >= targetValueInt) {
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
                } else if (typeV == FLOAT_VALUE) {  // Puis pour le type FLOAT
                    float currentValueFloat = atof(lineValue);
                    if (currentValueFloat >= targetValueFloat) {
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
            } else {
                fputs(line, tempFile);
            }
        }

    } else if (strcmp(operator, ">") == 0) { // Suppression des valeurs supérieures à la valeur cible fonctionnant de la meme façon que le inferieure 
        while (fgets(line, sizeof(line), file) != NULL) {
            if (strncmp(line, "values", 6) == 0) {  
                sscanf(line, "values.%*[^.].%*[^.].%s", lineValue);
                int currentValue = atoi(lineValue);

                if (typeV == INT_VALUE) {
                    if (currentValue <= targetValueInt) {
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
                } else if (typeV == FLOAT_VALUE) {
                    float currentValueFloat = atof(lineValue);
                    if (currentValueFloat <= targetValueFloat) {
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

void deleteTableFromFile(BinaryTree *tree, char *tableName) { // Fonction pour supprimer une table ainsi que toutes ces données d'un fichier ainsi que leurs nœux qui leurs sont associés
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
    char tablePrefix[256], columnPrefix[256], valuesPrefix[256];
    int8_t isValue = 0;

    snprintf(tablePrefix, sizeof(tablePrefix), "table.%s", tableName); // Création des 3 prefix a chercher dans le fichier texte
    snprintf(columnPrefix, sizeof(columnPrefix), "column.%s", tableName);
    snprintf(valuesPrefix, sizeof(valuesPrefix), "values.%s", tableName);

    long tableKey = createKey(tableName); // Création de la clé de la table qui se fait differament que les colonnes et les valueurs (uniquement le nom de la table)

    while (fgets(line, sizeof(line), file) != NULL) { // Pour toutes les lignes du fichier texte
        line[strcspn(line, "\n")] = '\0';

        if (strncmp(line, tablePrefix, strlen(tablePrefix)) == 0) { // Si la ligne commence par le prefix de la table que l'on veut supprimer
            isValue++;
            Node *current = tree->root;
            while (current != NULL) {
                if (current->tableData.key == tableKey) {  // On recherche et supprime le nœu qui lui est associé
                    deleteNode(&current);
                    break;
                }
                current = (tableKey < current->tableData.key) ? current->left : current->right;
            }
        } else if (strncmp(line, columnPrefix, strlen(columnPrefix)) == 0) { // Pareil pour si la ligne commence par le prefix d'une colonne
            char *suffixes[] = {"-INT", "-CHAR", "-FLOAT"};
            for (int i = 0; i < 3; i++) {
                char *pos = strstr(line, suffixes[i]);
                if (pos != NULL) {
                    *pos = '\0';
                    break;
                }
            }
            long columnKey = createKey(line);
            Node *current = tree->root;
            while (current != NULL) {
                if (current->columnData.key == columnKey) {
                    printf("Suppression du nœud de colonne avec key = %ld\n", columnKey);
                    deleteNode(&current);
                    break;
                }
                current = (columnKey < current->columnData.key) ? current->left : current->right;
            }
        } else if (strncmp(line, valuesPrefix, strlen(valuesPrefix)) == 0) { // Et si la ligne commence par le prefix d'une valueu
            isValue++;
            long valueKey = createKey(line);
            Node *current = tree->root;
            while (current != NULL) {
                if (current->valueData.key == valueKey) {
                    printf("Suppression du nœud de valeur avec key = %ld\n", valueKey);
                    deleteNode(&current);
                    break;
                }
                current = (valueKey < current->valueData.key) ? current->left : current->right;
            }
        } else {
            fputs(line, tempFile);
            fputc('\n', tempFile);
        }
    }

    if (isValue != 0) {
        printf("La table %s et toutes ses valeurs ont été supprimées.\n", tableName);
    }

    fclose(file);
    fclose(tempFile);

    remove(filepath);
    rename("database/temp.txt", filepath);
}

void displayDatabase() {  // Fonction pour afficher toutes les databases crées
    struct dirent *entry;
    char *dossier = "database/";
    DIR *dp = opendir(dossier); 

    if (dp == NULL) {
        printf("Erreur : impossible d'ouvrir le dossier %s.\n", dossier);
        return;
    }

    printf("Database existante : \n");
    
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) { // On ne veut pas afficher le repertoire courant et le parent
            printf("- %s\n", entry->d_name); 
        }
    }

    closedir(dp);  
}
