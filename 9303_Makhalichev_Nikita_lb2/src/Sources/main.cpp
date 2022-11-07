#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include "../Headers/Buffer.h"
#include "../Headers/Matrix.h"

void generate_matrices(Buffer& first_matrix_buffer, Buffer& second_matrix_buffer, int iterations_count, int rows, int cols){
    for (int i = 0; i < iterations_count; i++){
        Matrix first_matrix(rows, cols), second_matrix(rows, cols);
        first_matrix.generate();
        second_matrix.generate();
        first_matrix_buffer.produce(first_matrix);
        second_matrix_buffer.produce(second_matrix);
    }
}

void sum_matrices(Buffer& first_matrix_buffer, Buffer& second_matrix_buffer, Buffer& result_matrix_buffer, int iterations_count, int rows, int cols, int threads_count){
    for (int i = 0; i < iterations_count; i++){
        Matrix first_matrix = first_matrix_buffer.consume();
        Matrix second_matrix = second_matrix_buffer.consume();
        Matrix result_matrix(rows, cols);
        result_matrix.sum(first_matrix, second_matrix, threads_count);
        result_matrix_buffer.produce(result_matrix);
    }
}

void write_result(Buffer& result_matrix_buffer, int iterations_count, std::ostream& stream){
    for (int i = 0; i < iterations_count; i++){
        Matrix result_matrix = result_matrix_buffer.consume();
        stream << result_matrix << '\n';
    }
};

int main(int argc, char *argv[]){
    if (argc < 5){
    	std::cout << "Not enough arguments" << std::endl;
    	std::cout << "Correct run should be: ./a.out <rows> <columns> <threads count> <iterations count>" << std::endl;
    	return 0;
    }
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int threads_count = atoi(argv[3]);
    int iterations_count = atoi(argv[4]);
    
    std::srand(std::time(0));
    std::ofstream matrix_file;
    matrix_file.open("result_matrix.txt");
    Buffer first_matrix_buffer(8), second_matrix_buffer(8), result_matrix_buffer(8);
    
    std::thread generate = std::thread(generate_matrices, std::ref(first_matrix_buffer), std::ref(second_matrix_buffer), iterations_count, rows, cols);
    std::thread sum = std::thread(sum_matrices, std::ref(first_matrix_buffer), std::ref(second_matrix_buffer), std::ref(result_matrix_buffer), iterations_count, rows, cols, threads_count);
    std::thread write = std::thread(write_result, std::ref(result_matrix_buffer), iterations_count, std::ref(matrix_file));
    
    generate.join();
    sum.join();
    write.join();
    return 0;
}
