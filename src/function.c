#include <stdio.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include "../include/function.h"

void replaceSpacesToDashes(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '-';
        }
    }
}

long  createKey(const char *str) {
    long  sum = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        sum += (int)str[i];  // On additionne toutes les valeurs ascii dans sum
    }

    time_t currentTime = time(NULL);
    sum = sum * currentTime; // Et on multiplie cette somme au nb de secondes depuis Unix

    return sum;
}