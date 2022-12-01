

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "Matrix.h"

void MatrixRead();

void MatrixSum();

void StartProcess(const std::function<void()> &go);

void MatrixWrite();

void *getSharedPointer();