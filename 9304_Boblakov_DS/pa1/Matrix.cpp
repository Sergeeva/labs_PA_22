#include "Matrix.h"

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

std::vector<std::vector<int>> generateMatrix(int m_width, int m_height) {
    std::vector<int> init_vec(m_height, 0);
    Matrix gen_matrix(m_width, init_vec);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(-500, 500);
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            gen_matrix[i][j] = distr(gen);
        }
    }
    return gen_matrix;
}