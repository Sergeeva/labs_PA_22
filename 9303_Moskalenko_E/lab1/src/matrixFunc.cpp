#include <vector>
#include "./matrixFunc.h"

using namespace std;

void generateMatrix(vector<vector<int>> &matrix) {
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

void makeRangeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>>& result,
                       int start, int stop) {
    for (int i = start; i < stop; i++) {
        for (int j = 0; j < M; j++) {
            result[i][j] = first[i][j] + second[i][j];
        }
    }
}