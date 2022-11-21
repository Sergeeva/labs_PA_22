#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if(argc < 4){
        puts("Usage: generate_matrix <row> <column> <matrix_filename>");
        return 1;
    }
    long int row = strtol(argv[1], NULL, 10), col = strtol(argv[2], NULL, 10);
    if(row <= 0){
        printf("Wrong argument row: %s\n", argv[1]);
    }
    if(col <= 0){
        printf("Wrong argument column: %s\n", argv[2]);
    }
    FILE* f = fopen(argv[3], "rb");
    if(f == NULL){
        printf("Cannot open file %s\n", argv[3]);
        return 1;
    }
    long int height, width;
    fread(&height, sizeof(long int), 1, f);
    fread(&width, sizeof(long int), 1, f);
    if(height < row || width < col){
        printf("Index is outside. Matrix is %ldx%ld\n", height, width);
        fclose(f);
        return 1;
    }
    fseek(f, 2 * sizeof(long int) + ((row - 1) * width + col - 1) * sizeof(int), SEEK_SET);
    int readed_value;
    fread(&readed_value, sizeof(long int), 1, f);
    fclose(f);
    printf("Readed value is %d\n", readed_value);
    return 0;
}
