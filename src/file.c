#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file.h"
#include "../include/menu.h"
#include "../include/sql.h"
#include "../include/database_structs.h"


FILE *file = NULL;

int verifFileExist(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);

    file = fopen(filepath, "r");

    if (file) {
        printf("Une base de donnée existe deja sous ce nom.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

int verifFileExistW(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);

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
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);

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


int createDatabase(char *filename) {

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);

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

int writeInDatabase(char *values) {
    fprintf(file, "\n%s", values);
    fflush(file); // Permet de vider le tampon
    return EXIT_SUCCESS;
}