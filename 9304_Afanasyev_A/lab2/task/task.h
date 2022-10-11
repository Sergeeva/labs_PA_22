#pragma once

#include <iostream>
#include <thread>
#include <fstream>

#include "../util/util.h"
#include "../matrix/Matrix.h"
#include "../blocking-queue/BlockingQueue.h"

void pushMatrix(BlockingQueue<Matrix> &queue, int iterations);

void popAndSumAndPushResult(BlockingQueue<Matrix> &queue, BlockingQueue<Matrix> &resultQueue, int iterations);

void popAndWrite(BlockingQueue<Matrix> &queue, std::ofstream &file, std::mutex &fileMutex, int iterations);