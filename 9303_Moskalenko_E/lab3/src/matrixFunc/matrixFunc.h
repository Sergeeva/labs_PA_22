//
// Created by Елизавета Москаленко on 01.10.2022.
//

#ifndef LAB1_PA_MATRIXFUNC_H
#define LAB1_PA_MATRIXFUNC_H

#include <vector>

using namespace std;
const int N = 3;
const int M = 3;
using Matrix = std::vector<std::vector<int>>;
using MatrixPair = std::pair<Matrix, Matrix>;

MatrixPair generateMatrixPair();
void printMatrix(vector<vector<int>> &result);
void generateMatrix(vector<vector<int>> &matrix);
void makeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result);
void makeRangeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result,
                       int start, int stop);

#endif //LAB1_PA_MATRIXFUNC_H
