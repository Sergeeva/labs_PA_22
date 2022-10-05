//
// Created by Елизавета Москаленко on 01.10.2022.
//

#include "processes.h"
#include "./matrixFunc.h"
#include "./fileWriter.h"

void processesFunc() {
    pid_t pid;
    int newProcess;
    cout << "First process creates matrix" << endl;
    vector<vector<int>> firstMatrix(N, vector<int>(M, 0));
    vector<vector<int>> secondMatrix(N, vector<int>(M, 0));
    vector<vector<int>> additionMatrix(N, vector<int>(M, 0));
    generateMatrix(firstMatrix);
    generateMatrix(secondMatrix);

    pid = fork();

    switch (pid) {
        case -1:
            exit(-1);

        case 0: //потомок
            cout << "Second process pid = " << getpid() << " makes addition " << endl;
            makeAddition(firstMatrix, secondMatrix, additionMatrix);
            newProcess = fork();
            switch (newProcess) {
                case -1:
                    exit(-1);
                case 0:
                    cout << "Third process pid = " << getpid() << " writes result to file" << endl;
                    writeToFile("processes", additionMatrix);
                    exit(0);
                default: //родитель pid>0
                    wait(&newProcess);
            }
            exit(0);

        default: //родитель pid>0
            wait(&pid);
    }
}