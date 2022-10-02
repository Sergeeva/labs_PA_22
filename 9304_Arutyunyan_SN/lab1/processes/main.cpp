#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "../common/matrix.hpp"
#include "../common/matrix_generation.hpp"
#include "../common/matrix_write.hpp"

constexpr std::size_t N = 5;
constexpr std::size_t M = 5;

struct MatrixesSharedMemory {
    char* matrix1 = nullptr;
    char* matrix2 = nullptr;
    char* summed_matrix = nullptr;
};

void* create_shared_memory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANONYMOUS;
  return mmap(NULL, size, protection, visibility, -1, 0);
}


MatrixesSharedMemory CreateSharedMemory() {
    MatrixesSharedMemory memory;
    memory.matrix1 = (char*) create_shared_memory(sizeof(MatrixValueType) * N * M);
    memory.matrix2 = (char*) create_shared_memory(sizeof(MatrixValueType) * N * M);
    memory.summed_matrix = (char*) create_shared_memory(sizeof(MatrixValueType) * N * M);

    return memory;
}

Matrix MatrixAddition(const Matrix& matrix1, const Matrix& matrix2) {
    Matrix result(N, Row(M));

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    return result;
}

void WriteMatrixToSharedMemory(const Matrix& matrix, char* shared_memory) {
    for (std::size_t i = 0; i < N; ++i) {
        memcpy(shared_memory + i * M * sizeof(MatrixValueType), matrix[i].data(), M * sizeof(MatrixValueType));
    }
}

void ReadMatrixFromSharedMemory(Matrix& matrix, char* shared_memory) {

    for (std::size_t i = 0; i < N; ++i) {
        memcpy(matrix[i].data(), shared_memory + i * M * sizeof(MatrixValueType), M * sizeof(MatrixValueType));
    }
}

void GenerateMatrixesInProcess(const MatrixesSharedMemory& memory) {
    auto matrix1 = GenerateMatrix(N, M);
    auto matrix2 = GenerateMatrix(N, M);

    WriteMatrixToSharedMemory(matrix1, memory.matrix1);
    WriteMatrixToSharedMemory(matrix2, memory.matrix2);
}

void MatrixesAdditionInProcess(const MatrixesSharedMemory& memory) {
    Matrix matrix1(N, Row(M)), matrix2(N, Row(M));
    ReadMatrixFromSharedMemory(matrix1, memory.matrix1);
    ReadMatrixFromSharedMemory(matrix2, memory.matrix2);

    auto summed_matrix = MatrixAddition(matrix1, matrix2);
    WriteMatrixToSharedMemory(summed_matrix, memory.summed_matrix);
}

void WriteSummedMatrixToFileInProcess(const MatrixesSharedMemory& memory) {
    Matrix summed_matrix(N, Row(M));
    ReadMatrixFromSharedMemory(summed_matrix, memory.summed_matrix);

    std::ofstream file("result.txt");
    MatrixWrite(summed_matrix, file);
}

int main() {
    const auto memory = CreateSharedMemory();

    auto generating_pid = fork();
    if (generating_pid == 0) {
        wait(NULL);
    } else {
        GenerateMatrixesInProcess(memory);
        return 0;
    }

    auto addition_pid = fork();
    if (addition_pid == 0) {
        wait(NULL);
    } else {
        MatrixesAdditionInProcess(memory);
        return 0;
    }

    auto write_pid = fork();
    if (write_pid == 0) {
        wait(NULL);
    } else {
        WriteSummedMatrixToFileInProcess(memory);
        return 0;
    }

    return 0;
}