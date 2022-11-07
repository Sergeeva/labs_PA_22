//
// Created by Елизавета Москаленко on 11.10.2022.
//

#ifndef LAB1_PA_BUFFER_H
#define LAB1_PA_BUFFER_H
#define BUFFER_CAPACITY 10
#include <iostream>
#include <fstream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;
using MatrixPair = std::pair<Matrix, Matrix>;

class Buffer {
    std::vector<MatrixPair> buffer;
    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;
public:
    void produce(MatrixPair matrixs);
    MatrixPair consume();
};


#endif //LAB1_PA_BUFFER_H
