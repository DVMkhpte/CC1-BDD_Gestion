#include <stdio.h>
#include "../include/menu.h"


int main(int argc, char *argv[]) {
    if (argc < 3) {
        createFirstMenu();  // Si il manque un argument on affiche le menu principal
    } else {
        verifFirstMenu(argv); // Sinon on utilise c'est argument pour lancer la bonne partie du programme
    }
    return 0; 
}
