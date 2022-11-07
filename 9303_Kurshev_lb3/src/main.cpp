#include <iostream>
#include <ctime>
#include "Headers/Buffer.h"
#include "Headers/Matrix.h"

void generate_matrices(Buffer& stack_buffer, int iterations, int rows, int columns){
    for (int i = 0; i < iterations; i++){
        Matrix first_matrix(rows, columns), second_matrix(rows, columns);
        first_matrix.matrix_generate();
        second_matrix.matrix_generate();
        stack_buffer.push(first_matrix);
        stack_buffer.push(second_matrix);
    }
}

void sum_matrices(Buffer& stack_buffer, Buffer& result_matrix_buffer, int iterations, int rows, int columns, int num_threads){
    for (int i = 0; i < iterations; i++){
        Matrix first_matrix = stack_buffer.pop();
        Matrix second_matrix = stack_buffer.pop();
        Matrix result_matrix(rows, columns);
        result_matrix.sum(first_matrix, second_matrix, result_matrix, num_threads);
        result_matrix_buffer.push(result_matrix);
    }
}

void write_result(Buffer& result_matrix_buffer, int iterations){
    for (int i = 0; i < iterations; i++){
        Matrix result_matrix = result_matrix_buffer.pop();
        result_matrix.write_result();
    }
};

int main(int argc, char *argv[]){
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);
    int num_threads = atoi(argv[3]);
    int iterations = atoi(argv[4]);

    std::srand(std::time(0));
    Buffer stack_buffer, result_buffer;
    std::thread matrix_generate = std::thread(generate_matrices, std::ref(stack_buffer), iterations, rows, columns);
    std::thread matrix_sum = std::thread(sum_matrices, std::ref(stack_buffer), std::ref(result_buffer), iterations, rows, columns, num_threads);
    std::thread matrix_write = std::thread(write_result, std::ref(result_buffer), iterations);
    matrix_generate.join();
    matrix_sum.join();
    matrix_write.join();

    return 0;
}