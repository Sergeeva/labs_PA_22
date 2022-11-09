#include <vector>
#include "./matrixFunc.h"
#include "../strassen/strassen.h"
#include "../threads/threads.h"
#include <iostream>

using namespace std;

void generateMatrix(vector<vector<int>> &matrix) {
    // srand(time(NULL));
    for (auto &i: matrix) {
        for (int &j: i) {
            j = rand() % 10;
        }
    }
}

void makeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>> &result) {
    for (int i = 0; i < first.size(); i++) {
        for (int j = 0; j < first[0].size(); j++) {
            result[i][j] = first[i][j] + second[i][j];
        }
    }
}

void subtract(Matrix &first, Matrix &second, Matrix &result) {
    for (int i = 0; i < first.size(); i++) {
        for (int j = 0; j < first[0].size(); j++) {
            result[i][j] = first[i][j] - second[i][j];
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

void makeRangeAddition(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>> &result,
                       int start, int stop) {
    for (int i = start; i < stop; i++) {
        for (int j = 0; j < first[0].size(); j++) {
            result[i][j] = first[i][j] + second[i][j];
        }
    }
}


void makeRangeSubtract(vector<vector<int>> &first, vector<vector<int>> &second, vector<vector<int>> &result,
                       int start, int stop) {
    for (int i = start; i < stop; i++) {
        for (int j = 0; j < first[0].size(); j++) {
            result[i][j] = first[i][j] - second[i][j];
        }
    }
}

void multiThreadsAdditionTemplate(Matrix &A, Matrix &B, Matrix &temp) {
    multiThreadsAddition(A, B, temp, makeRangeAddition);
}

void multiThreadsSubtractTemplate(Matrix &A, Matrix &B, Matrix &temp) {
    multiThreadsAddition(A, B, temp, makeRangeSubtract);
}

void tileMatrixMultiplication(Matrix &first, Matrix &second, Matrix &result) {
    for (int i = 0; i < first.size(); i++) {
        vector<int> resultRow(first.size(), 0);
        for (int j = 0; j < first.size(); j++) {
            for (int k = 0; k < first.size(); k++) {
                resultRow[j] += first[i][k] * second[k][j];
            }
        }
        result[i] = resultRow;
    }
}

vector<vector<Matrix>> divideMatrixOnTiles(Matrix &matrix) {
    vector<vector<Matrix>> tiles;
    for (int i = 0; i < matrix.size(); i += TILE_SIZE) {
        // тайл 2 на 2
        vector<Matrix> tilesRow;
        for (int j = 0; j < matrix[0].size(); j += TILE_SIZE) {
            Matrix tile;
            tile.push_back(vector<int>{matrix[i][j], matrix[i][j + 1]});  // в тайл поместили первую строку
            tile.push_back(vector<int>{matrix[i + 1][j], matrix[i + 1][j + 1]}); // вторую
            tilesRow.push_back(tile);
        }
        tiles.push_back(tilesRow);
    }
    return tiles;
}

Matrix toMatrix(vector<vector<Matrix>> array) {
    Matrix result(N, vector<int>(M, 0));

    for (int row = 0; row < array.size(); row++) {  // строка
        for (int col = 0; col < array[row].size(); col++) { // получили матрицу
            for (int rowMatrix = 0; rowMatrix < array[row][col].size(); rowMatrix++) { // две строки всего
                if (std::count(result[TILE_SIZE * row + rowMatrix].begin(),
                               result[TILE_SIZE * row + rowMatrix].end(), 0) == M) {
                    result[TILE_SIZE * row + rowMatrix].clear();
                }
                std::move(array[row][col][rowMatrix].begin(), array[row][col][rowMatrix].end(),
                          std::back_inserter(result[TILE_SIZE * row + rowMatrix]));
            }
        }
    }
    return result;
}

void makeMultiplyOfTiles(vector<Matrix> &temps, vector<Matrix> &AMatrices,
                         vector<Matrix> &BMatrices, int size) {
    if (size * 4 >= N) {
        std::vector<std::thread> vecOfThreads;
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[0]), std::ref(BMatrices[0]),
                                              std::ref(temps[0])));
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[1]), std::ref(BMatrices[2]),
                                              std::ref(temps[1])));
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[0]), std::ref(BMatrices[1]),
                                              std::ref(temps[2])));
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[1]), std::ref(BMatrices[3]),
                                              std::ref(temps[3])));
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[2]), std::ref(BMatrices[0]),
                                              std::ref(temps[4])));
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[3]), std::ref(BMatrices[2]),
                                              std::ref(temps[5])));
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[2]), std::ref(BMatrices[1]),
                                              std::ref(temps[6])));
        vecOfThreads.emplace_back(std::thread(tiledMultiply, std::ref(AMatrices[3]), std::ref(BMatrices[3]),
                                              std::ref(temps[7])));
        for (auto &vecOfThread: vecOfThreads) {
            vecOfThread.join();
        }
    } else {
        tiledMultiply(AMatrices[0], BMatrices[0], temps[0]);
        tiledMultiply(AMatrices[1], BMatrices[2], temps[1]);
        tiledMultiply(AMatrices[0], BMatrices[1], temps[2]);
        tiledMultiply(AMatrices[1], BMatrices[3], temps[3]);
        tiledMultiply(AMatrices[2], BMatrices[0], temps[4]);
        tiledMultiply(AMatrices[3], BMatrices[2], temps[5]);
        tiledMultiply(AMatrices[2], BMatrices[1], temps[6]);
        tiledMultiply(AMatrices[3], BMatrices[3], temps[7]);
    }
}

void makePartsOfC(vector<Matrix> &multiplyOfTiles, vector<Matrix> &partsOfC) {
    makeAddition(multiplyOfTiles[0], multiplyOfTiles[1], partsOfC[0]);
    makeAddition(multiplyOfTiles[2], multiplyOfTiles[3], partsOfC[1]);
    makeAddition(multiplyOfTiles[4], multiplyOfTiles[5], partsOfC[2]);
    makeAddition(multiplyOfTiles[6], multiplyOfTiles[7], partsOfC[3]);
}

void tiledMultiply(Matrix &first, Matrix &second, Matrix &result) {
    int n = result.size();
    if (n == 1) {
        result[0][0] = first[0][0] * second[0][0];
        return;
    }
    int halfSize = n / 2;
    if (halfSize < 64) {
        tileMatrixMultiplication(first, second, result);
        return;
    }
    vector<Matrix> partsOfC(4, Matrix(halfSize, vector<int>(halfSize, 0)));
    vector<Matrix> AMatrices(4, Matrix(halfSize, vector<int>(halfSize, 0)));
    vector<Matrix> BMatrices(4, Matrix(halfSize, vector<int>(halfSize, 0)));

    makeAB(AMatrices, BMatrices, first, second, halfSize);

    vector<Matrix> multiplyOfTiles(8, Matrix(halfSize, vector<int>(halfSize, 0)));
    makeMultiplyOfTiles(multiplyOfTiles, AMatrices, BMatrices, halfSize);
    makePartsOfC(multiplyOfTiles, partsOfC);
    fillResult(halfSize, result, partsOfC);
}

