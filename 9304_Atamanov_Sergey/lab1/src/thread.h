#ifndef PA_THREAD_H
#define PA_THREAD_H

#include <mutex>
#include <chrono>

#include "matrix.h"

void startThreadMode(int M, int N, int T);
void fillMatrix(Matrix& A, Matrix& B, std::mutex& mutex);
void sumMatrix(Matrix& A, Matrix& B, Matrix& C, std::mutex& mutex, int tempThread, int T);
void sumOneMatrix(Matrix& A, Matrix& B, Matrix& C);
void printMatrix(Matrix& A, Matrix& B, Matrix& C, std::mutex& mutex);


#endif //PA_THREAD_H
