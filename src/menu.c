#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/menu.h"
#include "../include/file.h"
#include "../include/sql.h"


void createFirstMenu() {
    printf("+-------------------------------------------------------------+-------------------------------------------------+\n");
    printf("|                     Premier argument                        |                 Second argument                 |\n");
    printf("+-------------------------------------------------------------+-------------------------------------------------+\n");
    printf("| 1. Mettez un 1 pour CREATE une base de donnée               |                                                 |\n");
    printf("| 2. Mettez un 2 pour USE une base de donnée                  | 1.Entrer le nom de la base de donnée            |\n");
    printf("| 3. Mettez un 3 pour Supprimer une base de donnée            |                                                 |\n");
    printf("+-------------------------------------------------------------+-------------------------------------------------+\n");
}

void createSecondMenu() {
    printf("+-----------------------------------------+\n");
    printf("|            Commande possible            |\n");
    printf("+-----------------------------------------+\n");
    printf("| 1. INSERT                               |\n");
    printf("| 2. SELECT                               |\n");
    printf("| 3. DELETE                               |\n");
    printf("| 4. UPDATE                               |\n");
    printf("| 5. SHOW                                 |\n");
    printf("| 6. CREATE TABLE                         |\n");
    printf("| 7. DROP TABLE                           |\n");
    printf("| 8. EXIT (Quitter le programme)          |\n");
    printf("+-----------------------------------------+\n");
}

void verifFirstMenu(char *argv[]) {
    switch (argv[1][0]) {
        case '1':
            verifFileExist(argv[2]);
            createDatabase(argv[2]);
            break;
        case '2':
            verifFileExistW(argv[2]);
            loadDatabase(argv[2]);
            break;
        case '3':
            verifFileExistD(argv[2]);

            break;
        default:
            createFirstMenu();
            break;
    }
}