//
// Created by Елизавета Москаленко on 01.10.2022.
//

#ifndef LAB1_PA_MATRIXFUNC_H
#define LAB1_PA_MATRIXFUNC_H

#include <vector>
using namespace std;
const int N = 500;
const int M = 500;
void generateMatrix(vector<vector<int>> &matrix);
void makeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result);
void makeRangeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result,
                       int start, int stop);

#endif //LAB1_PA_MATRIXFUNC_H
