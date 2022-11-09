#ifndef LABS_PA_22_LAB4_MAIN_H
#define LABS_PA_22_LAB4_MAIN_H

#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <chrono>

#include "constant/filenames.h"
#include "matrix/matrix.h"
#include "logger/logger.h"
#include "time/Time.h"

void enter_matrices(Matrix& first_matrix, Matrix& second_matrix);

void print_matrix(const Matrix& matrix);

#endif //LABS_PA_22_LAB4_MAIN_H
