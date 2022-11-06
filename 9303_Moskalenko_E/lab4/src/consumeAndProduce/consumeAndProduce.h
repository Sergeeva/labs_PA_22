//
// Created by Елизавета Москаленко on 06.11.2022.
//

#ifndef LAB1_PA_CONSUMEANDPRODUCE_H
#define LAB1_PA_CONSUMEANDPRODUCE_H

#include "../matrixFunc/matrixFunc.h"
#include "../queue/LockFreeQueue.h"
#include <iostream>
#include <thread>
#include <vector>

using TiledMatrix = vector<vector<Matrix>>;
using TiledMatrixPair = std::pair<TiledMatrix, TiledMatrix>;
using namespace std;

void consumeTiledMatrixPairAndProduceTiledMap(LockFreeQueue<TiledMatrixPair> &buffer,
                                              LockFreeQueue<MatrixMap> &tiledMapBuffer,
                                              int iterations);

void produceTiledMatrixPair(LockFreeQueue<TiledMatrixPair> &buffer, int iterations);

void consumeTiledMapAndProduceResultTiledMatrix(LockFreeQueue<MatrixMap> &tilesMapBuffer,
                                                LockFreeQueue<TiledMatrix> &resultTiledMatrixBuffer,
                                                int iterations);

void consumeTiledMatrixAndProduceResultMatrix(LockFreeQueue<TiledMatrix> &tiledMatrixBuffer,
                                              LockFreeQueue<Matrix> &fileWriterBuffer,
                                              int iterations);

void consumeResultMatrix(LockFreeQueue<Matrix> &fileWriterBuffer, int iterations);

void produceMatrixPair(LockFreeQueue<MatrixPair> &buffer, int iterations);

void consumeMatrixPairTiledMultiply(LockFreeQueue<MatrixPair> &buffer, LockFreeQueue<Matrix> &resultBuffer,
                                    int iterations);

void consumeMatrixPairStrassenMultiply(LockFreeQueue<MatrixPair> &buffer, LockFreeQueue<Matrix> &resultBuffer,
                                       int iterations);

template<typename T>
void consumeMatrixPairTemplate(LockFreeQueue<MatrixPair> &buffer, LockFreeQueue<Matrix> &resultBuffer,
                               int iterations, T function) {
    consumeMatrixPair(buffer, resultBuffer, iterations, function);
}


#endif //LAB1_PA_CONSUMEANDPRODUCE_H
