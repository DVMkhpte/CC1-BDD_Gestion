#include <stdio.h>
#include "../include/menu.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        createFirstMenu();
    } else {
        verifFirstMenu(argv);
    }
    return 0;
}
