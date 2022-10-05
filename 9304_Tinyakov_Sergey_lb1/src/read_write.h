#ifndef READ_WRITE_H
#define READ_WRITE_H

#include <stdio.h>

#define BIN_MODE 1
#define TEXT_MODE 2

int read_size_from_file(char* filename, char mode, long int* height, long int* width);

int read_matrix(char* filename, char mode, int* arr);

int write_matrix(char* filename, char mode, int* arr, long height, long width);

#endif // READ_WRITE_H
