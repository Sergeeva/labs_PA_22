#include <iostream>
#include "Matrix.hpp"


void Matrix::generate() {
  first_matrix = new int[FIRST_ROWS * FIRST_COLUMNS];
  for (int cur_symbol_num = 0; cur_symbol_num < FIRST_ROWS * FIRST_COLUMNS; cur_symbol_num++) {
    first_matrix[cur_symbol_num] = rand() % 10;
//    first_matrix[cur_symbol_num] = cur_symbol_num % 10;
  }
  second_matrix = new int[SECOND_ROWS * SECOND_COLUMNS];
  for (int cur_symbol_num = 0; cur_symbol_num < SECOND_ROWS * SECOND_COLUMNS; cur_symbol_num++) {
    second_matrix[cur_symbol_num] = rand() % 10;
//    second_matrix[cur_symbol_num] = cur_symbol_num % 10;
  }
}

Matrix::Matrix() {
    generate();
    result = new int[FIRST_ROWS * SECOND_COLUMNS];
}

void Matrix::print(std::mutex& print_mtx) {
  std::unique_lock<std::mutex> unique_lock(print_mtx);
  std::cout << "First ";
  for (int cur_symbol_num = 0; cur_symbol_num < FIRST_ROWS * FIRST_COLUMNS; cur_symbol_num++) {
    std::cout << first_matrix[cur_symbol_num] << " ";
  }
  std::cout  << "\n";
  std::cout << "Second ";
  for (int cur_symbol_num = 0; cur_symbol_num < SECOND_ROWS * SECOND_COLUMNS; cur_symbol_num++) {
    std::cout << second_matrix[cur_symbol_num] << " ";
  }
  std::cout  << "\n";
  unique_lock.unlock();
}

void Matrix::printToFile(std::ofstream& file, std::mutex& file_mtx) {
  std::unique_lock<std::mutex> unique_lock(file_mtx);
  int cur_symbol_num = 0;
  while (cur_symbol_num < FIRST_ROWS * SECOND_COLUMNS) {
    file << result[cur_symbol_num] << " ";
    cur_symbol_num++;
    if (cur_symbol_num % SECOND_COLUMNS == 0) {
      file << std::endl;
    }
  }
  unique_lock.unlock();
}

int Matrix::find_divisor(int n) {

  for (int i = n - 1; i > 1; i--) {
    if (n % i == 0)  {
      return i > 5 ? 1 : i;
    }
  }
  return 1;
}

void Matrix::choose_part_and_multiply(
  int* first,
  int* second,
  int thread_num_rows,
  int thread_num_columns,
  int thread_rows_first,
  int thread_columns_second
) {
  int thread_rows_first_start = calculate_first_start(thread_num_rows, thread_rows_first);
  int thread_columns_second_start = calculate_second_start(thread_num_columns, thread_columns_second);
  int thread_rows_first_end = calculate_first_end(thread_num_rows, thread_rows_first);
  int thread_columns_second_end = calculate_second_end(thread_num_columns, thread_columns_second);

  multiply_matrix_part(
    first,
    second,
    thread_rows_first_start,
    thread_columns_second_start,
    thread_rows_first_end, thread_columns_second_end
  );
}

int Matrix::calculate_first_start(int thread_num_rows, int thread_rows_first) {
  int start = thread_rows_first * thread_num_rows;
  return start < FIRST_ROWS ? start : FIRST_ROWS;
}

int Matrix::calculate_first_end(int thread_num_rows, int thread_rows_first) {
  int end = thread_rows_first * (thread_num_rows + 1);
  return end < FIRST_ROWS ? end : FIRST_ROWS;
}

int Matrix::calculate_second_start(int thread_num_columns, int thread_columns_second) {
  int start = thread_columns_second * thread_num_columns;
  return start < SECOND_COLUMNS ? start : SECOND_COLUMNS;
}

int Matrix::calculate_second_end(int thread_num_columns, int thread_columns_second) {
  int end = thread_columns_second * (thread_num_columns + 1);
  return end < SECOND_COLUMNS ? end : SECOND_COLUMNS;
}

void Matrix::multiply_matrix_part(
  int* first,
  int* second,
  int thread_rows_first_start,
  int thread_columns_second_start,
  int thread_rows_first_end,
  int thread_columns_second_end
) {
  for (int i = thread_rows_first_start; i < thread_rows_first_end; i++) {
    for (int j = thread_columns_second_start; j < thread_columns_second_end; j++) {
      int res = 0;
      for (int k = 0; k < FIRST_COLUMNS; k++) {
          res += first[i * FIRST_COLUMNS + k] * second[j + k * SECOND_COLUMNS];
      }
      result[i * SECOND_COLUMNS + j] = res;
    }
  }
}

void Matrix::multiply(int* first, int* second){
  int rows_threads = find_divisor(SUM_THREADS);
  int columns_threads = SUM_THREADS / rows_threads;

  std::thread myThreads[SUM_THREADS];

  int thread_rows_first = FIRST_ROWS / rows_threads + (FIRST_ROWS % rows_threads ? 1 : 0);
  int thread_columns_second = SECOND_COLUMNS / columns_threads + (SECOND_COLUMNS % columns_threads ? 1 : 0);

  for (int thread_num_rows = 0; thread_num_rows < rows_threads; thread_num_rows++) {
    for (int thread_num_columns = 0; thread_num_columns < columns_threads; thread_num_columns++) {
      myThreads[thread_num_rows * SUM_THREADS + thread_num_columns] = std::thread{
        &Matrix::choose_part_and_multiply,
        this,
        first,
        second,
        thread_num_rows,
        thread_num_columns,
        thread_rows_first,
        thread_columns_second
      };

      myThreads[thread_num_rows * SUM_THREADS + thread_num_columns].join();
    }
  }
}

