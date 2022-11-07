#pragma once

#include <iostream>
#include <thread>
#include <fstream>
#include <future>
#include <chrono>

#include "../util/util.h"
#include "../matrix/Matrix.h"

long long start(int threadsCount);

void sum(const Matrix &firstMatrix, const Matrix &secondMatrix, std::promise<Matrix> &resultPromise, int threadsCount);
