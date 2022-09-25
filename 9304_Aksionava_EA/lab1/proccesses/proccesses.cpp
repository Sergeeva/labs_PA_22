#include <iostream>
#include <fstream>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

#define FIRST_MATRIX_KEY 0x1234
#define SECOND_MATRIX_KEY 0x1334
#define THIRD_MATRIX_KEY 0x1434

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
//  printf("Data written in memory: ");
//  for (int i = 0; i < size; i++) {
//    cout << to_string(matrix[i]) + " ";
//  }
  shmdt(shm_matrix);
}

void read_matrix_from_file(int* matrix, int rows, int columns, string file_name, key_t key) {
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

void read_first_matrix_from_file(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_1.txt";
  read_matrix_from_file(matrix, rows, columns, matrix_name, FIRST_MATRIX_KEY);
}

void read_second_matrix_from_file(int* matrix, int rows, int columns) {
  string matrix_name = "Matrix" + to_string(rows) + "_" + to_string(columns) + "_2.txt";
  read_matrix_from_file(matrix, rows, columns, matrix_name, SECOND_MATRIX_KEY);
}

void read(int* first_matrix, int* second_matrix, int rows, int columns) {
  read_first_matrix_from_file(first_matrix, rows, columns);
  read_second_matrix_from_file(second_matrix, rows, columns);
}

void read_matrix_from_shm(int* matrix, int rows, int columns, key_t key) {
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    int* shm_matrix = (int*) shmat(shmid,(void*)0,0);
//    printf("Data read from memory: ");
//    for (int i = 0; i < rows * columns; i++) {
//      matrix[i] = shm_matrix[i];
//      cout << to_string(matrix[i]) + " ";
//    }
    shmdt(shm_matrix);
    shmctl(shmid,IPC_RMID,NULL);
}

void read_first_matrix_from_shm(int* matrix, int rows, int columns) {
  read_matrix_from_shm(matrix, rows, columns, FIRST_MATRIX_KEY);
}

void read_second_matrix_from_shm(int* matrix, int rows, int columns) {
  read_matrix_from_shm(matrix, rows, columns, SECOND_MATRIX_KEY);
}

void read_from_shm(int* first_matrix, int* second_matrix, int rows, int columns) {
  read_first_matrix_from_shm(first_matrix, rows, columns);
  read_second_matrix_from_shm(second_matrix, rows, columns);
}


void sum_matrix(int* result, int* first_matrix, int* second_matrix, int size){
  for(int i = 0; i < size; i++) {
    result[i] = first_matrix[i] + second_matrix[i];
  }
}

ofstream open_file_write(string file_name) {
  ofstream file;
  file.open(file_name, ios::out);
  return file;
}

void write_result_to_file(int* result, int rows, int columns) {
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
  pid_t pid_first;
  pid_t pid_second;

  int ret_first;
  int ret_second;

  int rows = 10;
  int columns = 6;
  int* first_matrix = new int[rows * columns];
  int* second_matrix = new int[rows * columns];
  int* result = new int[rows * columns];

  read(first_matrix, second_matrix, rows, columns);
  pid_first = fork();
    switch (pid_first){
    case -1:
      exit(-1);

    case 0:
      cout << "\nChild start sum\n";
      read_from_shm(first_matrix, second_matrix, rows, columns);
      sum_matrix(result, first_matrix, second_matrix, rows * columns);
      write_to_shm(result, rows * columns, THIRD_MATRIX_KEY);
      pid_second = fork();
      switch (pid_second){
          case -1:
            exit(-1);
          case 0:
            cout << "\nChild start write\n";
            read_matrix_from_shm(result, rows, columns, THIRD_MATRIX_KEY);
            write_result_to_file(result, rows, columns);
            cout << "\nChild end write\n";
            exit(ret_first);
          default:
            wait(&pid_second);
            cout << "\nParent end wait write\n";
         }
         exit(ret_second);
    default:
      wait(&pid_first);
      cout << "\nParent end wait\n";
    }

  delete[] first_matrix;
  delete[] second_matrix;
  delete[] result;
  return 0;
}