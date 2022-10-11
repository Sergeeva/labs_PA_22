#include "Matrix.h"

void Matrix::printMatrix() {
    for (int i = 0; i < this->width; ++i) {
        for (int j = 0; j < this->height; ++j) {
            std::cout<<this->get_matrix()[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

std::vector<std::vector<int>> Matrix::generateMatrix(int m_width, int m_height) {
    std::vector<int> init_vec(m_height, 0);
    std::vector<std::vector<int>> gen_matrix(m_width, init_vec);
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(-500, 500);
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            gen_matrix[i][j] = distr(gen);
        }
    }
    return gen_matrix;
}

Matrix::Matrix(int width, int height, bool generate) {
    this->width = width;
    this->height = height;
    this->genenrate = generate;

    if (this->genenrate) {
        this->matrix = generateMatrix(this->width, this->height);
    }

}

Matrix::~Matrix() {
    this->matrix.clear();
}

int Matrix::get_width() {
    return width;
}

int Matrix::get_height() {
    return height;
}

std::vector<std::vector<int>> Matrix::get_matrix() {
    return matrix;
}

void Matrix::set_matrix(std::vector<std::vector<int>> matrix){
    for (int i = 0; i < this->get_width(); ++i) {
        for (int j = 0; j < this->get_height(); ++j) {
            this->matrix[i][j] = matrix[i][j];
        }
    }

}

