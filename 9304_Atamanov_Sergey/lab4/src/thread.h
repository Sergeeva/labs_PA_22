#pragma once

#include <thread>
#include <mutex>
#include <fstream>
#include <atomic>
#include <chrono>

#include "matrix.h"

using matrix = std::vector<Matrix*>;

void startLab4();
void multiplyMatrix(Matrix& A, Matrix& B, Matrix& C, int n);
void referenceMatrix(Matrix A, Matrix B, Matrix& C);
void multiplyShtrassenMatrix(std::vector<Matrix*>& As, std::vector<Matrix*>& Bs, std::vector<Matrix*>& Ms, int thread);
void distributeMatrices(std::vector<Matrix*>& matrixs, Matrix& matrix);
void collectMatrix(std::vector<Matrix*>& matrixs, Matrix& matrix);