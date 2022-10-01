#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "../common/matrix.hpp"
#include "../common/matrix_generation.hpp"
#include "../common/matrix_write.hpp"

constexpr std::size_t N = 5;
constexpr std::size_t M = 5;
constexpr auto MATRIXES_SHM_NAME = "matrixes_shm";

struct MatrixesSharedMemory {
    void* matrix1 = nullptr;
    void* matrix2 = nullptr;
    void* summed_matrix = nullptr;
};

void* create_shared_memory(size_t size) {
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANONYMOUS;
  return mmap(NULL, size, protection, visibility, -1, 0);
}


MatrixesSharedMemory CreateSharedMemory() {
    MatrixesSharedMemory memory;
    memory.matrix1 = create_shared_memory(sizeof(Matrix<N, M>::value_type) * N * M);
    memory.matrix2 = create_shared_memory(sizeof(Matrix<N, M>::value_type) * N * M);
    memory.summed_matrix = create_shared_memory(sizeof(Matrix<N, M>::value_type) * N * M);

    return memory;
}

template <std::size_t N, std::size_t M>
Matrix<N, M> MatrixAddition(const Matrix<N, M>& matrix1, const Matrix<N, M>& matrix2) {
    Matrix<N, M> result;

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    return result;
}

void GenerateMatrixesInProcess(const MatrixesSharedMemory& memory) {
    auto matrix1 = GenerateMatrix<N, M>();
    auto matrix2 = GenerateMatrix<N, M>();

    memcpy(memory.matrix1, &matrix1, sizeof(matrix1));
    memcpy(memory.matrix2, &matrix2, sizeof(matrix2));
}

void MatrixesAdditionInProcess(const MatrixesSharedMemory& memory) {
    Matrix<N, M> matrix1, matrix2;
    memcpy(&matrix1, memory.matrix1, sizeof(matrix1));
    memcpy(&matrix2, memory.matrix2, sizeof(matrix2));

    auto summed_matrix = MatrixAddition(matrix1, matrix2);
    memcpy(memory.summed_matrix, &summed_matrix, sizeof(summed_matrix));
}

void WriteSummedMatrixToFileInProcess(const MatrixesSharedMemory& memory) {
    Matrix<N, M> summed_matrix;
    memcpy(&summed_matrix, memory.summed_matrix, sizeof(summed_matrix));

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