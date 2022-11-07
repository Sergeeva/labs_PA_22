#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BIN_MODE 1
#define TEXT_MODE 2
#define MAX_ABS_VALUE (1000)


int main(int argc, char** argv){
    if(argc < 4){
        puts("Usage: generate_matrix <height> <width> <output_filename> <mode>\nMode: bin (default) or text");
        return 1;
    }
    char mode = 0;
    if(argc < 5){
        printf("No mode is provided. Using bin mode by default\n");
        mode = BIN_MODE;
    }else if(strcmp(argv[4], "bin") == 0){
        printf("Using bin mode\n");
        mode = BIN_MODE;
    }else if(strcmp(argv[4], "text") == 0){
        printf("Using text mode\n");
        mode = TEXT_MODE;
    }else{
        printf("Unknown mode. Avaiable modes are 'bin' and 'text'\n");
        return 1;
    }
    long int height = strtol(argv[1], NULL, 10), width = strtol(argv[2], NULL, 10);
    if(height <= 0){
        printf("Wrong argument height: %s\n", argv[1]);
    }
    if(width <= 0){
        printf("Wrong argument width: %s\n", argv[2]);
    }
    FILE *f = NULL;
    if(mode == BIN_MODE){
        f = fopen(argv[3], "wb");
    }else{
        f = fopen(argv[3], "w");
    }
    if(f == NULL){
        printf("Cannot open file %s\n", argv[3]);
        return 1;
    }
    if(mode == BIN_MODE){
        fwrite(&height, sizeof(long int), 1, f);
        fwrite(&width, sizeof(long int), 1, f);
    }else{
        fprintf(f, "%ld %ld\n", height, width);
    }
    srand(time(NULL));
    int rand_var;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            rand_var = rand() % (2 * MAX_ABS_VALUE) - MAX_ABS_VALUE;
            if(mode == BIN_MODE) fwrite(&rand_var, sizeof(int), 1, f);
            else fprintf(f, "%d\n", rand_var);
        }
    }
    fclose(f);
    return 0;
}
