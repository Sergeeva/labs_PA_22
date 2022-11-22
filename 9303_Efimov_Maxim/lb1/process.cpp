#include <iostream>
#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#include <filesystem>
#include <string>


enum {N = 5, M = 5};



void test(int* buf) {
    int index = 0;
    for(int i = 0,index = 0;i< N*M; i++) {
        std::cout << buf[i] <<" ";
        index++;
        if(index == N) {
            index = 0;
            std::cout << std::endl;
        }
    }
}
int main()
{    
    Matrix first(N, M), second(N,M);
    first.fileOutput("first.txt");
    second.fileOutput("second.txt");
    int fd[2];
    const int read_index  = 0;
    const int write_index = 1;
    if (pipe(fd) < 0) {
        perror("pipe:");
        return 1;
    }
    int id = fork();
    if (id == -1) {
        perror("fork:");
        return 2;
    }
    if (id == 0) {                         /* Children process */
        std::cout << "the number of the process that summarizes:" << getpid()  <<std::endl;
        close(fd[read_index]);
        first.add(second);
        first.fileOutput("test_sum.txt");
        int* buf = first.serialization();
        write(fd[write_index], (void*)buf,sizeof(int) * N*M);
        delete[] buf;
        exit(42);

    }
    close(fd[write_index]);
    int* parent_buf = new int[N * M];
    int status = 0;
    wait(&status);                         //we dont need use wait becouse read is a block operation
    std::cout << "the status of children process: " << WEXITSTATUS(status) <<std::endl;
    read(fd[read_index], parent_buf,sizeof(int)* N * M);
    test(parent_buf);
    if (pipe(fd) < 0) {
        perror("pipe:");
        return 1;
    }
    id = fork();
    if (id == -1) {
        perror("fork:");
        return 2;
    }
    if (id == 0) {                         /* Children process */
        int* buf = new int [N* M];
        close(fd[write_index]);
        read(fd[read_index], (void*)buf, sizeof(int) * N*M);
        Matrix out;
        out.deserialization(buf, N, M);
        out.fileOutput("result_sum.txt");
        delete[] buf;
        exit(153); //PPDI

    }
    close(fd[read_index]);
    write(fd[write_index], (void*)parent_buf,sizeof(int) * N*M);
    wait(&status);
    std::cout << "the status of children process: " << WEXITSTATUS(status) <<std::endl;
    delete[] parent_buf;
    return 0;
}
