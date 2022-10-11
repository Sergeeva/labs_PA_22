#ifndef LAB1_COMMON_H
#define LAB1_COMMON_H

#include <iostream>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <vector>
#include <random>
#include <unistd.h>
#include <thread>
#include <fstream>
#include <random>
#include <ctime>

typedef std::vector<std::vector<int>> Matrix;

const int m_height = 15;
const int m_width = 15;

//Matrix generateMatrix(int height, int width);
//
//void printMatrix(Matrix matrix, std::ostream &ostream);



#endif //LAB1_COMMON_H
