#include <stdio.h>
#include <string.h>
#include "../include/database_structs.h"


void initDatabase(Database *db,char *name) {    // Fonction pour initialiser la base de données
    strcpy(db->databaseName, name);
    db->tables = NULL;
    db->tableCount = 0;

}



