#pragma once

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../matrix/Matrix.h"
#include "../util/util.h"

void startProcessFor(const std::function<void()> &toDo);

void readMatrices();

void sumMatrices();

void writeMatrices();

void *getSharedPointer();
