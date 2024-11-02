#ifndef FILE_H
#define FILE_H

int verifFileExist(char *filename);
int verifFileExistW(char *filename);
int verifFileExistD(char *filename);
void createDatabase(char *filename);
void loadDatabase(char *filename);
int writeInDatabase(char *values);


#endif