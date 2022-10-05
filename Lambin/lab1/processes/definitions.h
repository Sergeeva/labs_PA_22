#ifndef PROCESSES_DEFINITIONS_H
#define PROCESSES_DEFINITIONS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEBUG 0

#if DEBUG
#include <cerrno>
#include <cstring>
#endif

#define SHMID 2001
#define PERMS 0666

#define MAXSIZE 16

template <typename T>
struct Matrix {
    T data[MAXSIZE][MAXSIZE];
    int height = 0;
    int width = 0;
};

enum process {
    READ,
    CALCULATE,
    WRITE
};
enum matrix {A, B, C};
enum status {
    START,
    READY,
    RUN,
    TERMINATE
};

#endif //PROCESSES_DEFINITIONS_H
