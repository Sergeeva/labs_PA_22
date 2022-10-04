#include "read_write.h"

int read_size_from_file(char* filename, char mode, long int* height, long int* width){
    if (mode != BIN_MODE && mode != TEXT_MODE){
        return 1;
    }
    FILE *f = fopen(filename, "r");
    if (f == NULL){
        return 2;
    }
    int ret = 0;
    switch (mode){
        case BIN_MODE:
            ret = fread(height, sizeof(long int), 1, f);
            if (ret != -1) ret = fread(width, sizeof(long int), 1, f);
            if (ret != -1) ret = 0;
            break;
        case TEXT_MODE:
            ret = fscanf(f, "%ld %ld", height, width);
            ret = ret == 2 ? 0 : -1;
            break;
    }
    fclose(f);
    return ret;
}

int read_matrix(char* filename, char mode, int* arr){
    if (mode != BIN_MODE && mode != TEXT_MODE){
        return 1;
    }
    FILE *f = fopen(filename, "r");
    if (f == NULL){
        return 2;
    }
    int ret = 0;
    long int height, width;
    switch (mode){
        case BIN_MODE:
            ret = fread(&height, sizeof(long int), 1, f);
            if (ret != -1) ret = fread(&width, sizeof(long int), 1, f);
            if (ret != -1) ret = 0;
            break;
        case TEXT_MODE:
            ret = fscanf(f, "%ld %ld\n", &height, &width);
            ret = ret == 2 ? 0 : -1;
            break;
    }
    if (ret != 0) goto exit;
    for(long int i = 0; i < height; i++){
        for(long int j = 0; j < width; j++){
            switch (mode){
                case BIN_MODE:
                    ret = fread(&arr[i * width + j], sizeof(int), 1, f);
                    if (ret != -1) ret = 0;
                    break;
                case TEXT_MODE:
                    ret = fscanf(f, "%d\n", &arr[i * width + j]);
                    ret = ret == 1 ? 0 : -1;
                    break;
            }
            if (ret == -1) break;
        }
        if (ret == -1) break;
    }
exit:
    fclose(f);
    return ret;
}

int write_matrix(char* filename, char mode, int* arr, long height, long width){
    if (mode != BIN_MODE && mode != TEXT_MODE){
        return 1;
    }
    FILE *f = fopen(filename, "w");
    if (f == NULL){
        return 2;
    }
    switch (mode){
        case BIN_MODE:
            fwrite(&height, sizeof(long int), 1, f);
            fwrite(&width, sizeof(long int), 1, f);
            break;
        case TEXT_MODE:
            fprintf(f, "%ld %ld\n", height, width);
            break;
    }
    for(long int i = 0; i < height; i++){
        for(long int j = 0; j < width; j++){
            switch (mode){
                case BIN_MODE:
                    fwrite(&arr[i * width + j], sizeof(int), 1, f);
                    break;
                case TEXT_MODE:
                    fprintf(f, "%d\n", arr[i * width + j]);
                    break;
            }
        }
    }
    fclose(f);
    return 0;
}
