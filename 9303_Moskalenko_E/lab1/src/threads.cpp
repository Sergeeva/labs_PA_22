#include "threads.h"
#include <thread>
#include "./matrixFunc.h"
#include "./fileWriter.h"
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
    for (int countOfThreads = 2; countOfThreads < N + 1; countOfThreads++) { // min 2, max - N
        cout << "Count of threads : " << countOfThreads << endl;

        auto start = high_resolution_clock::now();

        int part = N / countOfThreads; // сколько каждому потоку заполнять строк
        int rowStart = 0;
        for (int threadNumber = 0; threadNumber < countOfThreads; threadNumber++) {
            int rowStop = (threadNumber == countOfThreads - 1) ? N : rowStart + part;
            //cout << rowStart << " " << rowStop << endl;
            thread rangeAddition(makeRangeAddition, ref(firstMatrix),
                                 ref(secondMatrix), ref(additionMatrix), rowStart, rowStop);
            rangeAddition.join();
            rowStart = rowStop;
        }

        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);

//        thread writerThread(writeToFile, "threads" + to_string(countOfThreads), ref(additionMatrix),
//                            "Time taken by function: "+ to_string(duration.count()) + " microseconds" );
        //writerThread.join();

        cout << "Time taken by function: "+ to_string(duration.count()) + " microseconds" << endl;
    }
}

void threads(void (*func)(vector<vector<int>> &, vector<vector<int>> &,
                          vector<vector<int>> &)) {
    vector<vector<int>> firstMatrix(N, vector<int>(M, 0));
    vector<vector<int>> secondMatrix(N, vector<int>(M, 0));
    vector<vector<int>> additionMatrix(N, vector<int>(M, 0));

    cout << "First and second threads generate matrixes" << endl;
    thread firstMatrixCreator(generateMatrix, ref(firstMatrix));
    thread secondMatrixCreator(generateMatrix, ref(secondMatrix));
    firstMatrixCreator.join();
    secondMatrixCreator.join();

    thread firstMatrixWriter(writeToFile, "matrix1", ref(firstMatrix), "");
    firstMatrixWriter.join();

    thread secondMatrixWriter(writeToFile, "matrix2", ref(secondMatrix), "");
    secondMatrixWriter.join();

    func(firstMatrix, secondMatrix, additionMatrix);
}
