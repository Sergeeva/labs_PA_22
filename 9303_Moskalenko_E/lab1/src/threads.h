//
// Created by Елизавета Москаленко on 01.10.2022.
//

#ifndef LAB1_PA_THREADS_H
#define LAB1_PA_THREADS_H
#include <vector>

using namespace std;
void oneThreadAddition(vector<vector<int>> &firstMatrix, vector<vector<int>> &secondMatrix,
                       vector<vector<int>> &additionMatrix);

void multiThreadsAddition(vector<vector<int>> &firstMatrix, vector<vector<int>> &secondMatrix,
                          vector<vector<int>> &additionMatrix);

void threads(void (*func)(vector<vector<int>> &, vector<vector<int>> &,
                          vector<vector<int>> &));


#endif //LAB1_PA_THREADS_H
