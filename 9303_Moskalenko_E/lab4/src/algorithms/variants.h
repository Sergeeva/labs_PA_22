//
// Created by Елизавета Москаленко on 06.11.2022.
//

#ifndef LAB1_PA_VARIANTS_H
#define LAB1_PA_VARIANTS_H

#include <iostream>
#include <thread>
#include <map>
#include "../matrixFunc/matrixFunc.h"
#include "../fileWriter/fileWriter.h"
#include "../queue/LockFreeQueue.h"
#include "../threads/threads.h"
#include "../strassen/strassen.h"
#include "../consumeAndProduce/consumeAndProduce.h"

void secondVariant(int countOfThreads, int iterations) {
    auto generateBuffer = new LockFreeQueue<MatrixPair>(); // очередь для пар матриц в виде тайлов
    auto writerBuffer = new LockFreeQueue<Matrix>(); // очередь для получвшихся при умножении матриц - для записи в файл
    std::vector<std::thread> vecOfThreads;
    vecOfThreads.reserve(countOfThreads);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < countOfThreads; i++) {

        vecOfThreads.emplace_back(std::thread(produceMatrixPair, std::ref(*generateBuffer), iterations));

        // consumeMatrixPairStrassenMultiply
        // consumeMatrixPairTiledMultiply
        vecOfThreads.emplace_back(std::thread(consumeMatrixPairStrassenMultiply, std::ref(*generateBuffer),
                                              std::ref(*writerBuffer), iterations));

        vecOfThreads.emplace_back(std::thread(consumeResultMatrix, std::ref(*writerBuffer),
                                              iterations));
    }

    for (auto &vecOfThread: vecOfThreads) {
        vecOfThread.join();
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Time taken by function: " + to_string(duration.count()) + " milliseconds";
}

void firstVariant(int countOfThreads, int iterations) {
    auto generateBuffer = new LockFreeQueue<TiledMatrixPair>(); // очередь для пар матриц в виде тайлов
    auto tilesMapBuffer = new LockFreeQueue<MatrixMap>(); // очередь для мап с умножением по тайлам двух матриц
    auto tiledResultBuffer = new LockFreeQueue<TiledMatrix>();
    auto writerBuffer = new LockFreeQueue<Matrix>(); // очередь для получвшихся при умножении матриц - для записи в файл
    std::vector<std::thread> vecOfThreads;
    vecOfThreads.reserve(countOfThreads);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < countOfThreads; i++) {

        // создаем разбитые на тайлы матрицы
        vecOfThreads.emplace_back(std::thread(produceTiledMatrixPair, std::ref(*generateBuffer), iterations));

        vecOfThreads.emplace_back(std::thread(consumeTiledMatrixPairAndProduceTiledMap, std::ref(*generateBuffer),
                                              std::ref(*tilesMapBuffer), iterations));

        vecOfThreads.emplace_back(std::thread(consumeTiledMapAndProduceResultTiledMatrix, std::ref(*tilesMapBuffer),
                                              std::ref(*tiledResultBuffer), iterations));

        vecOfThreads.emplace_back(std::thread(consumeTiledMatrixAndProduceResultMatrix, std::ref(*tiledResultBuffer),
                                              std::ref(*writerBuffer), iterations));

        vecOfThreads.emplace_back(std::thread(consumeResultMatrix, std::ref(*writerBuffer),
                                              iterations));
    }

    for (auto &vecOfThread: vecOfThreads) {
        vecOfThread.join();
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Time taken by function: " + to_string(duration.count()) + " milliseconds";
}

#endif //LAB1_PA_VARIANTS_H
