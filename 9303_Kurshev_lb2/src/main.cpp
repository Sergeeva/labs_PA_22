#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include "Headers/Buffer.h"
#include "Headers/Matrix.h"

void generate_matrices(Buffer& matrix1_buffer, Buffer& matrix2_buffer, int iterations, int rows, int columns){
    for (int i = 0; i < iterations; i++){
        Matrix first_matrix(rows, columns), second_matrix(rows, columns);
        first_matrix.matrix_generate();
        second_matrix.matrix_generate();
        matrix1_buffer.produce(first_matrix);
        matrix2_buffer.produce(second_matrix);
    }
}

void sum_matrices(Buffer& matrix1_buffer, Buffer& matrix2_buffer, Buffer& result_buffer, int iterations, int rows, int columns, int num_threads){
    for (int i = 0; i < iterations; i++){
        Matrix first_matrix = matrix1_buffer.consume();
        Matrix second_matrix = matrix2_buffer.consume();
        Matrix result_matrix(rows, columns);
        result_matrix.sum(first_matrix, second_matrix, result_matrix, num_threads);
        result_buffer.produce(result_matrix);
    }
}

void write_result(Buffer& result_matrix_buffer, int iterations){
    for (int i = 0; i < iterations; i++){
        Matrix result_matrix = result_matrix_buffer.consume();
        result_matrix.write_result();
    }
};

int main(int argc, char *argv[]){
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    int num_threads = atoi(argv[3]);
    int iterations = atoi(argv[4]);

    std::srand(std::time(0));
    Buffer matrix1_buffer(10), matrix2_buffer(10), result_buffer(10);

    std::thread generate = std::thread(generate_matrices, std::ref(matrix1_buffer), std::ref(matrix2_buffer), iterations, rows, columns);
    std::thread sum = std::thread(sum_matrices, std::ref(matrix1_buffer), std::ref(matrix2_buffer), std::ref(result_buffer), iterations, rows, columns, num_threads);
    std::thread write = std::thread(write_result, std::ref(result_buffer), iterations);

    generate.join();
    sum.join();
    write.join();
    return 0;
}