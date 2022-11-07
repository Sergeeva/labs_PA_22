#include <vector>
#include "./matrixFunc.h"
#include <iostream>

using namespace std;

void generateMatrix(vector<vector<int>> &matrix) {
    srand(time(NULL));
    for (auto &i: matrix) {
        for (int &j: i) {
            j = rand() % 100;
        }
    }
}
void makeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            result[i][j] = first[i][j] + second[i][j];
        }
    }
}

MatrixPair generateMatrixPair() {
    vector<vector<int>> firstMatrix(N, vector<int>(M, 0));
    vector<vector<int>> secondMatrix(N, vector<int>(M, 0));
    generateMatrix(firstMatrix);
    generateMatrix(secondMatrix);
    return std::make_pair(firstMatrix, secondMatrix);
}

void printMatrix(vector<vector<int>> &result) {
    for (auto &i: result) {
        std::cout << endl;
        for (int &j: i) {
            cout << j << " ";
        }
    }
}

void makeRangeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result,
                       int start, int stop) {
    for (int i = start; i < stop; i++) {
        for (int j = 0; j < M; j++) {
            result[i][j] = first[i][j] + second[i][j];
        }
    }
}