#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <chrono>

#include "matrix.h"
#include "queue.h"
#include <queue>

void startLab2(int M, int N, int count, int pc, int cc, int T);
void createMatrix(int M, int N, Queue<Matrix*>& queue, int count);
void sumMatrix(int M, int N, Queue<Matrix*>& queue, Queue<Matrix*>& queuePrint, int count, std::atomic<int>& sumCount);
void printMatrix(Queue<Matrix*>& queue, int count, std::fstream* output);