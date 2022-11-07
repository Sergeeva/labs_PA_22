#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

void generate_matrix(string filename, int rows, int cols){
    ofstream matrix_file;
    matrix_file.open(filename);
    for (int i = 0; i < rows * cols; i++){
    	matrix_file << rand() % 10000 - 5000 << ' ';
    }
    matrix_file.close();
    return;
}

int main(int argc, char *argv[]){
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    srand(time(0));
    generate_matrix("first_matrix.txt", rows, cols);
    generate_matrix("second_matrix.txt", rows, cols);
    return 0;
}
