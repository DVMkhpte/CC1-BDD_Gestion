#include <stdio.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include "../include/function.h"
#include "../include/database_structs.h"

void replaceSpacesToDashes(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            str[i] = '-';
        }
    }
}

long  createKey(char *str) {
    long  sum = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        sum += (int)str[i]; // On additionne toutes les valeurs ascii dans sum
    }
    
    return sum;
}

ValueType detectValueType(char *value) {
    
    char *endptr;
    strtol(value, &endptr, 10);
    if (*endptr == '\0') {
        return INT_VALUE;
    }

    strtof(value, &endptr);
    if (*endptr == '\0' && strchr(value, '.')) {
        return FLOAT_VALUE;
    }

    size_t len = strlen(value);
    if ((value[0] == '\'' && value[len - 1] == '\'') || 
        (value[0] == '"' && value[len - 1] == '"')) {
        return STRING_VALUE;
    }

    return;
}