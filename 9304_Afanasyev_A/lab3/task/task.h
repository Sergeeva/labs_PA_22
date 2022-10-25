#pragma once

#include <iostream>
#include <thread>
#include <fstream>

#include "../util/util.h"
#include "../matrix/Matrix.h"
#include "../blocking-stack/BlockingStack.h"

void pushMatrix(BlockingStack<Matrix> &stack, int iterations);

void popAndSumAndPushResult(BlockingStack<Matrix> &stack, BlockingStack<Matrix> &resultStack, int iterations);

void popAndWrite(BlockingStack<Matrix> &stack, int iterations);
