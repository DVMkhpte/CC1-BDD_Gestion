#ifndef FILE_H
#define FILE_H

int verifFileExist(char *filename);
int verifFileExistW(char *filename);
int verifFileExistD(char *filename);
int createDatabase(char *filename);
int writeInDatabase(char *values);


#endif