#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
#include "matrix.h"

void createMatrix(int M, int N){
    std::fstream ao, bo;
    Matrix A(M, N);
    Matrix B(M, N);
    ao.open("aoutput.json", std::ios_base::out);
    bo.open("boutput.json", std::ios_base::out);

    if(!ao.is_open() || !bo.is_open())
        std::cerr << "File is undefined\n";
    A.fillMatrix();
    B.fillMatrix();

    ao << A;
    bo << B;

    ao.close();
    bo.close();
}

void sumMatrix(int M, int N){
    Matrix A(M, N);
    Matrix B(M, N);
    Matrix C(M, N);

    std::fstream ain, bin, co;
    ain.open("aoutput.json", std::ios_base::in);
    bin.open("boutput.json", std::ios_base::in);

    if(!ain.is_open() || !bin.is_open())
        std::cerr << "File is undefined\n";

    ain >> A;
    bin >> B;

    ain.close();
    bin.close();

    C = A + B;

    co.open("coutput.json", std::ios_base::out);
    co << C;
    co.close();
}

void startProcessMode(int M, int N){
    pid_t pid;

    pid = fork();

    switch (pid) {
        case -1:
            std::cerr << "Error in process creation\n";
            std::exit(-1);
        case 0:
            createMatrix(M, N);
        default:
            wait(&pid);
            break;
    }

    pid = fork();

    switch (pid){
        case -1:
            std::cerr << "Error in process creation\n";
            std::exit(-1);
        case 0:
            sumMatrix(M, N);
        default:
            wait(&pid);
            break;
    }

    /*pid = fork();

     switch (pid) {
         case -1:
             std::cerr << "Error in process creation\n";
             std::exit(-1);
         case 0:
             std::fstream co;
             co.open("coutput.json", std::ios_base::out);
             if(!co.is_open())
                 std::cerr << "File is undefined\n";
             c
     }*/
}