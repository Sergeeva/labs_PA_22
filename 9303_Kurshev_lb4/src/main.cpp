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

void mult_matrices(Buffer& stack_buffer, Buffer& result_matrix_buffer, int iterations, int num_threads){
    for (int i = 0; i < iterations; i++){
        Matrix first_matrix = stack_buffer.pop();
        Matrix second_matrix = stack_buffer.pop();
        Matrix result_matrix(first_matrix.getRows(), second_matrix.getColumns());
        result_matrix.multiply(first_matrix, second_matrix, num_threads);
        result_matrix_buffer.push(result_matrix);
    }
}

void strassen_mult(Buffer& stack_buffer, Buffer& result_matrix_buffer, int depth, int iterations, int num_threads){
    for (int i = 0; i < iterations; i++){
        Matrix first_matrix = stack_buffer.pop();
        Matrix second_matrix = stack_buffer.pop();
        Matrix result_matrix = strassenAlgorithm(first_matrix, second_matrix, depth, num_threads);
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
    int depth = atoi(argv[5]);
    int type = atoi(argv[6]);
    if (type == 1){
        std::srand(std::time(0));
        Buffer stack_buffer, result_buffer;
        clock_t start_time = clock();
        std::thread matrix_generate = std::thread(generate_matrices, std::ref(stack_buffer), iterations, rows, columns);
        std::thread matrix_sum = std::thread(mult_matrices, std::ref(stack_buffer), std::ref(result_buffer), iterations, num_threads);
        std::thread matrix_write = std::thread(write_result, std::ref(result_buffer), iterations);
        matrix_generate.join();
        matrix_sum.join();
        matrix_write.join();
        clock_t duration = clock() - start_time;
        std::cout << "Sum duration - " << duration / (double) CLOCKS_PER_SEC << "s" << std::endl;
    }
    else{
        Buffer stack_buffer2, result_buffer2;
        clock_t start_time2 = clock();
        std::thread matrix_generate2 = std::thread(generate_matrices, std::ref(stack_buffer2), iterations, rows, columns);
        std::thread matrix_sum2 = std::thread(strassen_mult, std::ref(stack_buffer2), std::ref(result_buffer2), depth, iterations, num_threads);
        std::thread matrix_write2 = std::thread(write_result, std::ref(result_buffer2), iterations);
        matrix_generate2.join();
        matrix_sum2.join();
        matrix_write2.join();
        clock_t duration2 = clock() - start_time2;
        std::cout << "Sum duration - " << duration2 / (double) CLOCKS_PER_SEC << "s" << std::endl;
    }

    return 0;
}