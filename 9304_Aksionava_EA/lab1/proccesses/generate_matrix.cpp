#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

ofstream open_file(int rows, int columns, int file_num) {
  ofstream file;
  file.open("Matrix" + to_string(rows) + "_" + to_string(columns) + "_" + to_string(file_num) + ".txt", ios::out);
  return file;
}

void write_matrix_size(ofstream& file, int rows, int columns) {
  file << to_string(rows) + " " + to_string(columns) << endl;
}

void write_rand_matrix(int rows, int columns, int file_num) {

  ofstream file = open_file(rows, columns, file_num);

//  write_matrix_size(file, rows, columns);

  int cur_symbol_num = 0;
  while (cur_symbol_num < rows * columns) {
    file << rand() % 1000 << " ";
    cur_symbol_num++;
    if (cur_symbol_num % columns == 0) {
      file << endl;
    }
  }

  file.close();
}

int main(){
  int rows = 10;
  int columns = 6;
  srand(time(0));

  write_rand_matrix(rows, columns, 1);
  write_rand_matrix(rows, columns, 2);

  return 0;
}