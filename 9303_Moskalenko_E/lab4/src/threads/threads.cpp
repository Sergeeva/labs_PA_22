#include "threads.h"
#include <thread>
#include "../matrixFunc/matrixFunc.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;


void oneThreadAddition(vector<vector<int>> &firstMatrix, vector<vector<int>> &secondMatrix,
                       vector<vector<int>> &additionMatrix) {
    thread additionThread(makeAddition, ref(firstMatrix), ref(secondMatrix), ref(additionMatrix));
    additionThread.join();
}


