#include <stdio.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include "../include/function.h"
#include "../include/database_structs.h"
#include "../include/file.h"
#include "../include/node.h"

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

    //printf("Key creer pour %s: %ld\n", str, sum);

    
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

        return STRING_VALUE;
    

    
}


void analyseCondition(BinaryTree *tree,char *tableName, char *column, char *operator, char *value) {
    ValueType valueType = detectValueType(value);

        deleteValuesFromFileWithConditionInt(tree,tableName,column,operator,value);
    
    if (valueType == STRING_VALUE) printf("Value str");
    if (valueType == FLOAT_VALUE) printf("Value float");

}