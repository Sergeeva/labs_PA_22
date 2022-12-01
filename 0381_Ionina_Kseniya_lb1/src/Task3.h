#pragma once

#include <iostream>
#include <thread>
#include <fstream>
#include <future>
#include <chrono>
#include "Matrix.h"

long long start(int count_thread);

void sum(const Matrix &Matrix1, const Matrix &secondMatrix, std::promise<Matrix> &resultPromise, int count_thread);