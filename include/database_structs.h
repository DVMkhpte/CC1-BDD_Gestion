#ifndef DATABASE_STRUCTS_H
#define DATABASE_STRUCTS_H

#include <stdint.h>

typedef struct Value {
    union {
        int intValue;
        float floatValue;
        char stringValue[256];
    } data;
    struct Value *left;
    struct Value *right;
} Value;

typedef struct Column {
    char columnName[256];
    char type[20];
    Value *values;
    struct Column *left;
    struct Column *right;
} Column;

typedef struct Table {
    char tableName[256];
    Column *columns;
    int columnCount;
    struct Table *left;
    struct Table *right;
} Table;

typedef struct Database {
    char databaseName[256];
    Table *tables;
    int tableCount;
} Database;


void initDatabase(Database *db, char *name);


void displayDatabase(Database *db);

#endif 