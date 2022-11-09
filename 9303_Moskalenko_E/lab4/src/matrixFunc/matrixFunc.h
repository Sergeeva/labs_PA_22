#ifndef LAB1_PA_MATRIXFUNC_H
#define LAB1_PA_MATRIXFUNC_H

#include <vector>
#include <map>

using namespace std;
const int N = 8;
const int M = 8;
const int TILE_SIZE = 2;
using Matrix = std::vector<std::vector<int>>;
using MatrixPair = std::pair<Matrix, Matrix>;
using MatrixMap = std::map<vector<int>, Matrix>;

MatrixPair generateMatrixPair();

void printMatrix(vector<vector<int>> &result);

void generateMatrix(vector<vector<int>> &matrix);

void subtract(Matrix &first, Matrix &second, Matrix &result);

void makeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>> &result);

void makeRangeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>> &result,
                       int start, int stop);

void tileMatrixMultiplication(Matrix &first, Matrix &second, Matrix &result);

vector<vector<Matrix>> divideMatrixOnTiles(Matrix &matrix);

Matrix toMatrix(vector<vector<Matrix>> array);

void tiledMultiply(Matrix &first, Matrix &second, Matrix &result);

void multiThreadsAdditionTemplate(Matrix &A, Matrix &B, Matrix &temp);

void multiThreadsSubtractTemplate(Matrix &A, Matrix &B, Matrix &temp);

#endif //LAB1_PA_MATRIXFUNC_H
