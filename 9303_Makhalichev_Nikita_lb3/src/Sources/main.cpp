#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include "../Headers/Buffer.h"


void generate_matrices(Buffer* first_matrix_buffer, Buffer* second_matrix_buffer, int iterations_count, int rows, int cols){
    for (int i = 0; i < iterations_count; i++){
        Matrix first_matrix(rows, cols), second_matrix(rows, cols);
        first_matrix.generate();
        second_matrix.generate();
        first_matrix_buffer->produce(first_matrix);
        second_matrix_buffer->produce(second_matrix);
    }
}

void sum_matrices(Buffer* first_matrix_buffer, Buffer* second_matrix_buffer, Buffer* result_matrix_buffer, int iterations_count, int rows, int cols, int threads_count){
    for (int i = 0; i < iterations_count; i++){
        Matrix first_matrix = *first_matrix_buffer->consume().get();
        Matrix second_matrix = *second_matrix_buffer->consume().get();
        Matrix result_matrix(rows, cols);
        result_matrix.sum(first_matrix, second_matrix, threads_count);
        result_matrix_buffer->produce(result_matrix);
    }
}

void write_result(Buffer* result_matrix_buffer, int iterations_count, std::ostream& stream){
    for (int i = 0; i < iterations_count; i++){
        Matrix result_matrix = *result_matrix_buffer->consume().get();
        stream << result_matrix << '\n';
    }
};

int main(int argc, char *argv[]){
    if (argc < 6){
        std::cout << "Not enough arguments" << std::endl;
        std::cout << "Correct run should be: ./a.out <rows> <columns> <gen/sum/write threads count> <threads count> <iterations count>" << std::endl;
        return 0;
    }
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int threads_count = atoi(argv[3]);
    int sum_threads_count = atoi(argv[4]);
    int iterations_count = atoi(argv[5]);

    std::srand(std::time(0));
    std::ofstream matrix_file;
    matrix_file.open("result_matrix.txt");
    Buffer* first_matrix_buffer = new Buffer();
    Buffer* second_matrix_buffer = new Buffer();
    Buffer* result_matrix_buffer = new Buffer();

    std::vector<std::thread> gen_threads(threads_count);
    std::vector<std::thread> sum_threads(threads_count);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threads_count; i++){
        gen_threads[i] = std::thread(generate_matrices, first_matrix_buffer, second_matrix_buffer, iterations_count, rows, cols);
        sum_threads[i] = std::thread(sum_matrices, first_matrix_buffer, second_matrix_buffer, result_matrix_buffer, iterations_count, rows, cols, sum_threads_count);
    }
    std::thread write_thread = std::thread(write_result, result_matrix_buffer, iterations_count * threads_count, std::ref(matrix_file));
    for (int i = 0; i < threads_count; i++){
        gen_threads[i].join();
        sum_threads[i].join();
    }
    write_thread.join();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Duration - " << duration.count() << "ms" << std::endl;
    matrix_file.close();
    delete first_matrix_buffer;
    delete second_matrix_buffer;
    delete result_matrix_buffer;
    return 0;
}