#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "read_write.h"
#include "matrix_operations.h"


void readers_func(int* arr, char* filename1, char* filename2, char mode, long int size){
    *arr = 0;
    int *arr_start = (int*)((char*)arr + 2);
    pid_t reader2 = fork();
    switch (reader2){
        case 0:
            // reader2
            if(read_matrix(filename2, mode, arr_start + size)){
                printf("[Reader 2] Cannot read the file %s\n", filename2);
                *arr = 0;
            }else{
                *((char*)arr + 1) = 0xFF;
            }
            break;
        case -1:
            // reader1 on fail
            puts("[Reader 1] Cannot create child");
            break;
        default:
            // reader1
            if(read_matrix(filename1, mode, arr_start)){
                printf("[Reader 1] Cannot read the file %s\n", filename1);
                *arr = 0;
            }else{
                *((char*)arr) = 0xFF;
            }
            wait(NULL);
    }
}


int main(int argc, char** argv){
    if (argc < 4){
        puts("Usage: process_sum <input_filename1> <input_filename2> <output_filename1> <mode>\nMode: bin (default) or text");
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
    // Read sizes of matrices
    long int height1, width1, height2, width2;
    if (read_size_from_file(argv[1], mode, &height1, &width1)){
        printf("Cannot read size from file %s\n", argv[1]);
        return 1;
    }
    if (read_size_from_file(argv[2], mode, &height2, &width2)){
        printf("Cannot read size from file %s\n", argv[2]);
        return 1;
    }
    if (height1 != height2 || width1 != width2){
        puts("Matrices have different dimensions!");
        return 1;
    }
    int ret;
    long int msize = height1 * width1;
    // alloc shared memory
    int shmid = shmget(IPC_PRIVATE, 2 * msize + 2, IPC_CREAT | S_IREAD | S_IWRITE);
    if (shmid == -1){
        puts("Cannot allocate shared memory");
        return 1;
    }
    int *arr = shmat(shmid, NULL, 0);
    if ((long int)arr == -1){
        puts("Cannot attach shared memory");
        ret = 1;
        goto destroy;
    }

    // read matrices
    pid_t reader1 = fork();
    switch (reader1){
        case 0:
            // reader1
            readers_func(arr, argv[1], argv[2], mode, msize);
            return 0; // readers exit from main
            break;
        case -1:
            // main on fail
            puts("[Main] Cannot create reader 1");
            ret = 1;
            goto detach;
            break;
        default:
            // main
            wait(NULL);
    }

    // Check read status
    if (*((unsigned short int*)arr) != 0xFFFF){
        puts("[Main] Matrices are not readed");
        goto detach;
    }

    // sum matrices
    int* start = (int*)((char*) arr + 2);
    pid_t summator = fork();
    switch (summator){
        case 0:
            // summator
            sum_elements(start, start + msize, 0, msize);
            return 0; // summator exits from main
            break;
        case -1:
            // main on fail
            puts("[Main] Cannot create summator");
            ret = 1;
            goto detach;
            break;
        default:
            // main
            wait(NULL);
    }

    // write result
    pid_t writer = fork();
    switch (writer){
        case 0:
            // writer
            if (write_matrix(argv[3], mode, start, height1, width1)){
                printf("[Writer] Cannot write matrix to the file %s", argv[3]);
            }
            return 0; // writer exits from main
            break;
        case -1:
            // main on fail
            puts("[Main] Cannot create writer");
            ret = 1;
            goto detach;
            break;
        default:
            // main
            wait(NULL);
    }

detach:
    if (shmdt(arr) == -1){
        puts("Cannot detach shared memory");
        ret = 1;
        goto end_main;
    }
destroy:
    if (shmctl(shmid, IPC_RMID, NULL) == -1){
        puts("Cannot destroy shared memory");
        ret = 1;
    }
end_main:
    return ret;
}
