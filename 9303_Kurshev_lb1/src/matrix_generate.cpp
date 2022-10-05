#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


void generate_matrix(const std::string& filename, int rows, int columns){
    std::ofstream file;
    file.open(filename);
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            file << rand() % 10000 << ' ';
        }
        file << std::endl;
    }
    file.close();
}

int main(int argc, char *argv[]){
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    srand(time(0));
    generate_matrix("Matrix1.txt", rows, columns);
    generate_matrix("Matrix2.txt", rows, columns);
    return 0;
}
