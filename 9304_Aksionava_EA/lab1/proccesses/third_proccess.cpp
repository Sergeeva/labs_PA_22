#include <iostream>
#include <fstream>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>

using namespace std;

#define THIRD_MATRIX_KEY 0x1434

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

int main(){
  int rows = 10;
  int columns = 6;
  int* result = new int[rows * columns];

  read_matrix_from_shm(result, rows, columns, THIRD_MATRIX_KEY);
  write_result_to_file(result, rows, columns);

  delete[] result;
  return 0;
}