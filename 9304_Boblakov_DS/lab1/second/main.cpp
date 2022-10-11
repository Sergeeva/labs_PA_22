#include "../common.h"

Matrix sum(Matrix matrix1, Matrix matrix2){
    Matrix matrix_res(matrix1.size(), std::vector<int>(matrix1[0].size()));
    for (int i = 0; i < matrix1.size(); i++) {
        for (int j = 0; j < matrix1[i].size(); j++) {
            matrix_res[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return matrix_res;
}
Matrix generateMatrix(int height, int width) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-1000, 1000);

    Matrix matrix(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix[i][j] = distr(gen);
        }
    }

    return matrix;
}

void printMatrix(Matrix matrix, std::ostream &ostream) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            ostream << matrix[i][j] << " ";
        }

        ostream << "\n";
    }
}
void read_func_thread(Matrix matrix1, Matrix matrix2){
    matrix1 = generateMatrix(m_height, m_width);
    matrix2 = generateMatrix(m_height, m_width);
}

int main(){
    Matrix matrix1, matrix2, res;
    std::thread read_thread(read_func_thread, std::ref(matrix1), std::ref(matrix2));
    read_thread.join();
    std::ofstream firstMatrixFile("firstMatrix.txt");
    printMatrix(matrix1, firstMatrixFile);

    read_thread.join();
}