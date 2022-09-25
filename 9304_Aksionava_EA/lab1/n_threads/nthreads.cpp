#include <iostream>
#include <fstream>
#include <string>
#include <thread>

using namespace std;

void thread_func(const int i) {
    cout << "hello from thread: " << i << endl;
}

ifstream open_file_read(string file_name) {
  ifstream file;
  file.open(file_name, ios::in);
  return file;
}

void read_matrix(int* matrix, int rows, int columns, string file_name) {
  ifstream file = open_file_read(file_name);
  string delimiter = " ";
  for(int i = 0; i < rows; i++){
    string line;
    getline(file, line);
    for(int j = 0; j < columns; j++){
      matrix[i * columns + j] = stoi(line.substr(0, line.find(delimiter)));
//      cout << matrix[i * columns + j] << " ";
      line = line.substr(line.find(delimiter) + 1);
    }
  }
  file.close();
}

void read_first_matrix(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_1.txt";
  read_matrix(matrix, rows, columns, matrix_name);
}

void read_second_matrix(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_2.txt";
  read_matrix(matrix, rows, columns, matrix_name);
}

void init_block(int* first_matrix, int* second_matrix, int rows, int columns) {
  read_first_matrix(first_matrix, rows, columns);
  read_second_matrix(second_matrix, rows, columns);
}

void sum_matrix_elem(int* result, int first_matrix_elem, int second_matrix_elem, int index) {
  result[index] = first_matrix_elem + second_matrix_elem;
}

void sum_matrix(int* result, int* first_matrix, int* second_matrix, int size, int thread_count){
  std::thread myThreads[thread_count];
  int cur_thread_count = thread_count;
  for (int elem_num = 0; elem_num < size; elem_num += cur_thread_count){
    for (int thread_num = 0; thread_num < cur_thread_count; thread_num++) {
      int index = elem_num + thread_num;
      myThreads[thread_num] = std::thread{sum_matrix_elem, result, first_matrix[index], second_matrix[index], index};
      if (elem_num + cur_thread_count >= size) {
          cur_thread_count = size - elem_num;
      }
    }
    for (int thread_num = 0; thread_num < thread_count; thread_num++) {
      myThreads[thread_num].join();
    }
  }
//  for(int i = 0; i < size; i++) {
//    result[i] = first_matrix[i] + second_matrix[i];
//  }
}

ofstream open_file_write(string file_name) {
  ofstream file;
  file.open(file_name, ios::out);
  return file;
}

void write_result(int* result, int rows, int columns) {
  ofstream file = open_file_write("result.txt");
  int cur_symbol_num = 0;
  while (cur_symbol_num < rows * columns) {
    file << result[cur_symbol_num] << " ";
    cur_symbol_num++;
    if (cur_symbol_num % columns == 0) {
      file << endl;
    }
  }
  file.close();
}

int main() {

  int rows = 10;
  int columns = 6;
  int thread_count = 10;
  int* first_matrix = new int[rows * columns];
  int* second_matrix = new int[rows * columns];
  int* result = new int[rows * columns];
  thread read_t;
  thread sum_t;
  thread write_t;

  cout << "read_t exists" << endl;

  read_t = thread{init_block, first_matrix, second_matrix, rows, columns};
  read_t.join();

  cout << "read" << endl;

  sum_t = thread{ sum_matrix, result, first_matrix, second_matrix, rows * columns, thread_count};
  sum_t.join();

  cout << "sum" << endl;

  write_t = thread{ write_result, result, rows, columns };
  write_t.join();

  cout << "write" << endl;

  delete[] first_matrix;
  delete[] second_matrix;
  delete[] result;
  return 0;
}