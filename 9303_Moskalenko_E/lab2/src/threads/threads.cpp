#include "threads.h"
#include <thread>
#include "../matrixFunc/matrixFunc.h"
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;


void oneThreadAddition(vector<vector<int>> &firstMatrix, vector<vector<int>> &secondMatrix,
                       vector<vector<int>> &additionMatrix) {
    cout << "Third thread make addition" << endl;
    thread additionThread(makeAddition, ref(firstMatrix), ref(secondMatrix), ref(additionMatrix));
    additionThread.join();
}


void multiThreadsAddition(vector<vector<int>> &firstMatrix, vector<vector<int>> &secondMatrix,
                          vector<vector<int>> &additionMatrix) {
        int countOfThreads = 3;
        int part = N / countOfThreads; // сколько каждому потоку заполнять строк
        auto start = high_resolution_clock::now();
        std::vector<std::thread> vecOfThreads;
        vecOfThreads.reserve(countOfThreads);
        int rowStart = 0;

        for (int threadNumber = 0; threadNumber < countOfThreads; threadNumber++) {
            int rowStop = (threadNumber == countOfThreads - 1) ? N : rowStart + part;
            //cout << rowStart << " " << rowStop << endl;
            vecOfThreads.emplace_back(thread(makeRangeAddition, ref(firstMatrix),
                                 ref(secondMatrix), ref(additionMatrix), rowStart, rowStop));
            rowStart = rowStop;
        }

        for (int i = 0; i < vecOfThreads.size(); i++) {
            vecOfThreads[i].join();
        }

}


