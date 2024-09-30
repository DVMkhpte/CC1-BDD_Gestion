#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file.h"

int verifFileExist(char *filename) {
    
    char filepath[256]; 
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);
    
    FILE *file = fopen(filepath, "r");

    if (file) {
        printf("Une base de donnée existe deja sous ce nom.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
}

int verifFileExistW(char *filename) {
    
    char filepath[256]; 
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);
    
    FILE *file = fopen(filepath, "r");

    if (!file) {
        printf("Aucune base de donnée n'existe sous ce nom\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }else{
        fclose(file);
        file = fopen(filepath, "r+");
    }
}

int verifFileExistD(char *filename) {
    
    char filepath[256]; 
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);
    
    FILE *file = fopen(filepath, "r");

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
    
    FILE *file = fopen(filepath, "w");

    if (!file) {
        printf("Erreur lors de la création de la base de donnée\n");
        exit(EXIT_FAILURE);;
    }

    fclose(file);



}