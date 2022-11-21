//
// Created by Елизавета Москаленко on 01.10.2022.
//

#ifndef LAB1_PA_MATRIXFUNC_H
#define LAB1_PA_MATRIXFUNC_H

#include <vector>
#include "../matrixBuffer/Buffer.h"

using namespace std;
const int N = 10;
const int M = 10;

MatrixPair generateMatrixPair();
void printMatrix(vector<vector<int>> &result);
void generateMatrix(vector<vector<int>> &matrix);
void makeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result);
void makeRangeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result,
                       int start, int stop);

#endif //LAB1_PA_MATRIXFUNC_H
