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
        return EXIT_FAILURE;
    }
}

int createDatabase(char *filename) {
     
    char filepath[256]; 
    snprintf(filepath, sizeof(filepath), "../database/%s", filename);
    
    FILE *file = fopen(filepath, "w");

    if (!file) {
        printf("Erreur lors de la création de la base de donnée");
        return 1;
    }

}