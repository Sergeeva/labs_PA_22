#pragma once

#include <iostream>
#include <thread>
#include <fstream>
#include <future>

#include "Matrix.h"

using namespace std;

void sum(const Matrix &Matrix1, const Matrix &Matrix2, promise<Matrix> &resultPromise);