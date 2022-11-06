#include "consumeAndProduce.h"
#include "../fileWriter/fileWriter.h"
#include "../strassen/strassen.h"

void produceTiledMatrixPair(LockFreeQueue<TiledMatrixPair> &buffer, int iterations) {   // кладет в очередь пару тайлов
    for (int i = 0; i < iterations; i++) {
        MatrixPair pairOfMatrix = generateMatrixPair();
        TiledMatrix firstTiles = divideMatrixOnTiles(pairOfMatrix.first);
        TiledMatrix secondTiles = divideMatrixOnTiles(pairOfMatrix.second);
        buffer.produce(std::make_pair(firstTiles, secondTiles));
    }
}

void consumeTiledMatrixPairAndProduceTiledMap(LockFreeQueue<TiledMatrixPair> &buffer,
                                              LockFreeQueue<MatrixMap> &tiledMapBuffer,
                                              int iterations) {                        // из массивов тайлов делает MatrixMap и кладет в очередь
    for (int iteration = 0; iteration < iterations; iteration++) {
        TiledMatrixPair pairOfMatrix = *buffer.consume();
        TiledMatrix &firstTiles = pairOfMatrix.first;
        TiledMatrix &secondTiles = pairOfMatrix.second;
        Matrix result(N, vector<int>(M, 0));
        std::vector<std::thread> vecOfThreads;
        vecOfThreads.reserve((N / TILE_SIZE) * (N / TILE_SIZE) * TILE_SIZE);
        MatrixMap tileCombinations;
        for (int i = 0; i < firstTiles.size(); i++) {  //  по двумерному массиву тайлов
            for (int j = 0; j < firstTiles[0].size(); j++) {  // получаем один тайл
                for (int k = 0; k < secondTiles.size(); k++) {
                    tileCombinations.insert({vector<int>{i, j, k}, Matrix(TILE_SIZE, vector<int>(TILE_SIZE, 0))});
                }
            }
        }
        for (int i = 0; i < firstTiles.size(); i++) {  //  по двумерному массиву тайлов
            for (int j = 0; j < firstTiles[0].size(); j++) {  // получаем один тайл
                for (int k = 0; k < secondTiles.size(); k++) {
                    // cout << "OPERATION " << i << " " << j << " " << k << endl;
                    vecOfThreads.emplace_back(std::thread(tileMatrixMultiplication, std::ref(firstTiles[i][k]),
                                                          std::ref(secondTiles[k][j]),
                                                          std::ref(tileCombinations.at(vector<int>{i, j, k}))));
                }
            }
        }

        for (auto &thread: vecOfThreads) {
            thread.join();
        }

        tiledMapBuffer.produce(tileCombinations);
    }
}


void consumeTiledMapAndProduceResultTiledMatrix(LockFreeQueue<MatrixMap> &tilesMapBuffer,
                                                LockFreeQueue<TiledMatrix> &resultTiledMatrixBuffer,
                                                int iterations) {
    for (int iteration = 0; iteration < iterations; iteration++) {
        MatrixMap tileCombinations = *tilesMapBuffer.consume();
        vector<vector<Matrix>> results;
        for (int i = 0; i < N / TILE_SIZE; i++) {  //  по двумерному массиву тайлов
            vector<Matrix> resultsRow;
            for (int j = 0; j < M / TILE_SIZE; j++) {  // получаем один тайл
                Matrix temp(TILE_SIZE, vector<int>(TILE_SIZE, 0));
                for (int k = 0; k < N / TILE_SIZE; k++) {
                    Matrix need = tileCombinations.at(vector<int>{i, j, k});
                    multiThreadsAdditionTemplate(temp, need, temp);
                    // makeAddition(temp, need, temp);
                }
                resultsRow.emplace_back(temp);
            }
            results.emplace_back(resultsRow);
        }
        resultTiledMatrixBuffer.produce(results);
    }
}

void consumeTiledMatrixAndProduceResultMatrix(LockFreeQueue<TiledMatrix> &tiledMatrixBuffer,
                                              LockFreeQueue<Matrix> &fileWriterBuffer,
                                              int iterations) {
    for (int iteration = 0; iteration < iterations; iteration++) {
        TiledMatrix tiledResult = *tiledMatrixBuffer.consume();
        fileWriterBuffer.produce(toMatrix(tiledResult));
    }
}

void consumeResultMatrix(LockFreeQueue<Matrix> &fileWriterBuffer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        Matrix resultMatrix = *fileWriterBuffer.consume();
        // std::thread::id this_id = std::this_thread::get_id();
        writeToFile("result_lab4", resultMatrix, "");
    }
}

void produceMatrixPair(LockFreeQueue<MatrixPair> &buffer, int iterations) {   // кладет в очередь пару тайлов
    for (int i = 0; i < iterations; i++) {
        MatrixPair pairOfMatrix = generateMatrixPair();
        buffer.produce(pairOfMatrix);
    }
}

void consumeMatrixPairTiledMultiply(LockFreeQueue<MatrixPair> &buffer, LockFreeQueue<Matrix> &resultBuffer,
                                    int iterations) {   // кладет в очередь пару тайлов
    for (int i = 0; i < iterations; i++) {
        Matrix result(N, vector<int>(M, 0));
        MatrixPair pairOfMatrix = *buffer.consume();
        tiledMultiply(pairOfMatrix.first, pairOfMatrix.second, result);
        resultBuffer.produce(result);
    }
}

void consumeMatrixPairStrassenMultiply(LockFreeQueue<MatrixPair> &buffer, LockFreeQueue<Matrix> &resultBuffer,
                                       int iterations) {   // кладет в очередь пару тайлов
    for (int i = 0; i < iterations; i++) {
        Matrix result(N, vector<int>(M, 0));
        MatrixPair pairOfMatrix = *buffer.consume();
        strassenMultiply(pairOfMatrix.first, pairOfMatrix.second, result);
        resultBuffer.produce(result);
    }
}