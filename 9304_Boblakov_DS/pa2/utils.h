#ifndef PA2_UTILS_H
#define PA2_UTILS_H
#include <thread>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

typedef std::vector<std::vector<int>> Matrix;

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n";
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

void * read(Matrix & m1, Matrix & m2, int width, int height){

    m1 = generateMatrix(width, height);
    m2 = generateMatrix(width, height);

    std::cout<<"\nMatrix filled\n";
    return nullptr;
}

void * write(Matrix & m, std::string & file_name){

    std::ofstream out;
    out.open(file_name, std::fstream::out );

    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[i].size(); ++j) {
            out << m[i][j]<<" ";
        }
        out <<"\n";
    }
    out.close();
    return nullptr;
}
#endif //PA2_UTILS_H
