#include <iostream>
#include <fstream>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>

using namespace std;

#define FIRST_MATRIX_KEY 0x1234
#define SECOND_MATRIX_KEY 0x1334

ifstream open_file_read(string file_name) {
  ifstream file;
  file.open(file_name, ios::in);
  return file;
}

void write_to_shm(int* matrix, int size, key_t key) {
  int shmid = shmget(key, 1024, 0666|IPC_CREAT);
  int* shm_matrix = (int*) shmat(shmid,(void*)0,0);
  int cur_symbol_num = 0;
  while (cur_symbol_num < size) {
    shm_matrix[cur_symbol_num] = matrix[cur_symbol_num];
    cur_symbol_num++;
  }
  printf("Data written in memory: ");
  for (int i = 0; i < size; i++) {
    cout << to_string(matrix[i]) + " ";
  }
  shmdt(shm_matrix);
}

void read_matrix(int* matrix, int rows, int columns, string file_name, key_t key) {
  ifstream file = open_file_read(file_name);
  string delimiter = " ";
  for(int i = 0; i < rows; i++){
    string line;
    getline(file, line);
    for(int j = 0; j < columns; j++){
      matrix[i * columns + j] = stoi(line.substr(0, line.find(delimiter)));
      line = line.substr(line.find(delimiter) + 1);
    }
  }
  file.close();

  write_to_shm(matrix, rows * columns, key);
}

void read_first_matrix(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_1.txt";
  read_matrix(matrix, rows, columns, matrix_name, FIRST_MATRIX_KEY);
}

void read_second_matrix(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_2.txt";
  read_matrix(matrix, rows, columns, matrix_name, SECOND_MATRIX_KEY);
}

void init_block(int* first_matrix, int* second_matrix, int rows, int columns) {
  read_first_matrix(first_matrix, rows, columns);
  read_second_matrix(second_matrix, rows, columns);
}

int main(){
  int rows = 10;
  int columns = 6;
  int* first_matrix = new int[rows * columns];
  int* second_matrix = new int[rows * columns];

  init_block(first_matrix, second_matrix, rows, columns);

  delete[] first_matrix;
  delete[] second_matrix;
  return 0;
}