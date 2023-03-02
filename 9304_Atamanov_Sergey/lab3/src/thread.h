#include <iostream>
#include <random>

#include "matrix.h"
#include "queue.h"
#include <queue>
#include "node.h"
#include <thread>
#include <chrono>

void startLab3(int M, int N, int T, int count, int pc, int cc);
void createMatrix(int M, int N, Queue& queue, int count);
void sumMatrix(int M, int N, Queue& queue, Queue& queuePrint, int count, std::atomic<int>& sumCount);
void printMatrix(Queue& queue, int count, std::fstream* output);