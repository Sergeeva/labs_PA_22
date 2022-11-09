//
// Created by Елизавета Москаленко on 01.10.2022.
//

#ifndef LAB1_PA_THREADS_H
#define LAB1_PA_THREADS_H

#include <vector>
#include <iostream>
#include <thread>

using namespace std;

void oneThreadAddition(vector<vector<int>> &firstMatrix, vector<vector<int>> &secondMatrix,
                       vector<vector<int>> &additionMatrix);

template<typename T>
void multiThreadsAddition(vector<vector<int>> &firstMatrix, vector<vector<int>> &secondMatrix,
                          vector<vector<int>> &additionMatrix, T function) {
    int countOfThreads = (firstMatrix.size() / 2) > 1 ? firstMatrix.size() / 2 : 1;
    //int countOfThreads = 1;
    int part = firstMatrix.size() / countOfThreads; // сколько каждому потоку заполнять строк
    std::vector<std::thread> vecOfThreads;
    vecOfThreads.reserve(countOfThreads);
    int rowStart = 0;

    for (int threadNumber = 0; threadNumber < countOfThreads; threadNumber++) {
        int rowStop = (threadNumber == countOfThreads - 1) ? firstMatrix.size() : rowStart + part;
        //  std::cout << rowStart << " " << rowStop  << " " << countOfThreads << endl;
        vecOfThreads.emplace_back(std::thread(function, ref(firstMatrix),
                                              ref(secondMatrix), ref(additionMatrix), rowStart, rowStop));
        rowStart = rowStop;
    }

    for (int i = 0; i < vecOfThreads.size(); i++) {
        vecOfThreads[i].join();
    }
}


#endif //LAB1_PA_THREADS_H
