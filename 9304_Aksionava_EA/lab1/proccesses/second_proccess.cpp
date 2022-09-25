#include <iostream>
#include <fstream>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>

using namespace std;

#define FIRST_MATRIX_KEY 0x1234
#define SECOND_MATRIX_KEY 0x1334
#define THIRD_MATRIX_KEY 0x1434

void read_matrix_from_shm(int* matrix, int rows, int columns, key_t key) {
    int shmid = shmget(key,1024,0666|IPC_CREAT);
    int* shm_matrix = (int*) shmat(shmid,(void*)0,0);
    printf("Data read from memory: ");
    for (int i = 0; i < rows * columns; i++) {
      matrix[i] = shm_matrix[i];
      cout << to_string(matrix[i]) + " ";
    }
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

int main(){
  int rows = 10;
  int columns = 6;
  int* first_matrix = new int[rows * columns];
  int* second_matrix = new int[rows * columns];
  int* result = new int[rows * columns];

  read_from_shm(first_matrix, second_matrix, rows, columns);
  sum_matrix(result, first_matrix, second_matrix, rows * columns);
  write_to_shm(result, rows * columns, THIRD_MATRIX_KEY);

  delete[] first_matrix;
  delete[] second_matrix;
  delete[] result;
  return 0;
}