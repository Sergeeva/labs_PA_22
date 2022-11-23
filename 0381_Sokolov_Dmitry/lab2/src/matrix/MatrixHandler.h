#ifndef PA_LAB2_MATRIXHANDLER_H
#define PA_LAB2_MATRIXHANDLER_H

#include <thread>
#include <functional>
#include <fstream>
#include <filesystem>

#include <mutex>
#include <condition_variable>

#include "./Matrix.h"
#include "../utilities/Generator.h"

class MatrixHandler {
    static const Generator gen;

public:

    static Matrix create(int rows, int columns, unsigned int seed);

    static Matrix parallel_sum(Matrix& A, Matrix& B, int thread_number);

    static void output(Matrix& matrix, const std::string path, const std::string& name);

};


#endif //PA_LAB2_MATRIXHANDLER_H
