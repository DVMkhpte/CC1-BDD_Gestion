#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/file.h"

void createFirstMenu() {
    printf("+-------------------------------------------------------------+-------------------------------------------------+\n");
    printf("|                     Premier argument                        |                 Second argument                 |\n");
    printf("+-------------------------------------------------------------+-------------------------------------------------+\n");
    printf("| 1. Mettez un 1 pour CREATE une base de donnée               |                                                 |\n");
    printf("| 2. Mettez un 2 pour USE une base de donnée                  | 1.Entrer le nom de la base de donnée            |\n");
    printf("| 3. Mettez un 3 pour Supprimer une base de donnée            |                                                 |\n");
    printf("+-------------------------------------------------------------+-------------------------------------------------+\n");
}

void verifFirstMenu(char *argv[]) {
    switch (argv[1][0]) {  
        case '1':
            verifFileExist(argv[2]);
            createDatabase(argv[2]);
            break;
        case '2':
            
            break;
        case '3':
            
            break;
        default:
            createFirstMenu();
            break;
    }
}