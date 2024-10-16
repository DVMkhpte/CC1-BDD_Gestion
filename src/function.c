#include <stdio.h>
#include <string.h>
#include "../include/function.h"

void replaceSpacesToDashes(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '-';
        }
    }
}