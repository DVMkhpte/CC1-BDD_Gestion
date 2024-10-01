#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sql.h"
#include "../include/file.h"

void sqlEntry() {
    
    int8_t x = 1;

    while(x == 1){
    
    printf("Commande SQL :");
    
    char sql[256];
    fgets(sql, 255, stdin);

    char *token = strtok(sql, " ");
    char *sqlRest = strtok(NULL, "");
    
    
    if (token != NULL) {
        if(strcmp(token, "INSERT") == 0) {
            x = 0;
            insert(sqlRest);

        } else if(strcmp(token, "SELECT") == 0) {
            x = 0;

        } else if(strcmp(token, "DELETE") == 0) {
            x = 0;
            
        } else if(strcmp(token, "UPDATE") == 0) {
            x = 0;
            
        } else if(strcmp(token, "SHOW") == 0) {
            x = 0;
            
        } else if(strcmp(token, "CREATE TABLE") == 0) {
            x = 0;
            
        } else {
            printf("Commande SQL non conforme ou non reconnue\n");
        }

    }
    }
    
}


void insert(char *sqlRest) {
    char *values = strstr(sqlRest, "VALUES");
    
    if(values != NULL) {
        char *startValue = strstr(values, "(");
        char *endValue = strstr(values, ")");

        if (startValue != NULL && endValue != NULL && endValue > startValue) {
            char value[256];
            strncpy(value, startValue + 1, endValue - startValue - 1);
            value[endValue - startValue - 1] = '\0';
            writeInDatabase(value);
            
        } else{
            printf("Valeur non trouver ou mal formattées. \n");
        }
    }else{
     printf("VALUES non trouvé dans la requête.\n");
    }
}
