#pragma once

#include <iostream>
#include <thread>
#include <fstream>
#include <future>

#include "../util/util.h"
#include "../matrix/Matrix.h"

void sum(const Matrix &firstMatrix, const Matrix &secondMatrix, std::promise<Matrix> &resultPromise);
