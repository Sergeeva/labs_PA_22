#include <iostream>
#include "Matrix.hpp"

void Matrix::generate() {
  for (int cur_symbol_num = 0; cur_symbol_num < ROWS * COLUMNS; cur_symbol_num++) {
    first_matrix[cur_symbol_num] = rand() % 1000;
    second_matrix[cur_symbol_num] = rand() % 1000;
  }
}

Matrix::Matrix() {
    generate();
}

void Matrix::print(std::mutex& print_mtx) {
  std::unique_lock<std::mutex> unique_lock(print_mtx);
  std::cout << "First ";
  for (int cur_symbol_num = 0; cur_symbol_num < ROWS * COLUMNS; cur_symbol_num++) {
    std::cout << first_matrix[cur_symbol_num] << " ";
  }
  std::cout  << "\n";
  std::cout << "Second ";
  for (int cur_symbol_num = 0; cur_symbol_num < ROWS * COLUMNS; cur_symbol_num++) {
    std::cout << second_matrix[cur_symbol_num] << " ";
  }
  std::cout  << "\n";
  unique_lock.unlock();
}

void Matrix::printToFile(std::ofstream& file, std::mutex& file_mtx) {
  std::unique_lock<std::mutex> unique_lock(file_mtx);
  int cur_symbol_num = 0;
  while (cur_symbol_num < ROWS * COLUMNS) {
    file << result[cur_symbol_num] << " ";
    cur_symbol_num++;
    if (cur_symbol_num % COLUMNS == 0) {
      file << std::endl;
    }
  }
  unique_lock.unlock();
}

void Matrix::sum_matrix_part(int start, int end) {
  for (int i = start; i < end; i++) {
    result[i] = first_matrix[i] + second_matrix[i];
  }
}

void Matrix::sum(){
  std::thread myThreads[SUM_THREADS];
  int thread_part = (ROWS * COLUMNS) / SUM_THREADS + ((ROWS * COLUMNS) % SUM_THREADS ? 1 : 0);
  for (int thread_num = 0; thread_num < SUM_THREADS; thread_num++) {
    int start_pos = thread_num * thread_part;
    int end_pos = (ROWS * COLUMNS > (thread_num + 1) * thread_part ) ? (thread_num + 1) * thread_part : ROWS * COLUMNS;
    myThreads[thread_num] = std::thread{&Matrix::sum_matrix_part, this,  start_pos, end_pos};
  }
  for (int thread_num = 0; thread_num < SUM_THREADS; thread_num++) {
    myThreads[thread_num].join();
  }
}

