#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include "../Headers/Buffer.h"


void generate_matrices(Buffer* first_matrix_buffer, Buffer* second_matrix_buffer, int iterations_count, int first_rows, int first_cols, int second_cols){
    for (int i = 0; i < iterations_count; i++){
        Matrix first_matrix(first_rows, first_cols), second_matrix(first_cols, second_cols);
        first_matrix.generate();
        second_matrix.generate();
        first_matrix_buffer->produce(first_matrix);
        second_matrix_buffer->produce(second_matrix);
    }
}

void mult_matrices(Buffer* first_matrix_buffer, Buffer* second_matrix_buffer, Buffer* result_matrix_buffer, int iterations_count, int threads_count){
    for (int i = 0; i < iterations_count; i++){
        Matrix first_matrix = *first_matrix_buffer->consume().get();
        Matrix second_matrix = *second_matrix_buffer->consume().get();
        Matrix result_matrix(first_matrix.get_rows(), second_matrix.get_cols());
        auto start = std::chrono::high_resolution_clock::now();
        result_matrix.mult(first_matrix, second_matrix, threads_count);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Duration - " << duration.count() << "ms" << std::endl;
        result_matrix_buffer->produce(result_matrix);
    }
}

void strassen_mult_matrices(Buffer* first_matrix_buffer, Buffer* second_matrix_buffer, Buffer* result_matrix_buffer, int depth, int threads_count, int iterations_count){
    for (int i = 0; i < iterations_count; i++){
        Matrix first_matrix = *first_matrix_buffer->consume().get();
        Matrix second_matrix = *second_matrix_buffer->consume().get();
        auto start = std::chrono::high_resolution_clock::now();
        Matrix result_matrix = strassen(first_matrix, second_matrix, depth, threads_count);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        std::cout << "Duration - " << duration.count() << "ms" << std::endl;
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
    int first_rows, first_cols, second_cols, threads_count, mul_threads_count, iterations_count, depth;
    if (*argv[1] == 'd'){
        if (argc != 8){
            std::cout << "Not enough arguments" << std::endl;
            std::cout << "Correct run should be: ./a.out d <first rows> <first columns> <second columns> <gen/sum/write threads count> <threads count> <iterations count>" << std::endl;
            return 0;
        }
        first_rows = atoi(argv[2]);
        first_cols = atoi(argv[3]);
        second_cols = atoi(argv[4]);
        threads_count = atoi(argv[5]);
        mul_threads_count = atoi(argv[6]);
        iterations_count = atoi(argv[7]);
    } else if (*argv[1] == 's'){
        if (argc != 9){
            std::cout << "Not enough arguments" << std::endl;
            std::cout << "Correct run should be: ./a.out s <first rows> <first columns> <second columns> <depth> <gen/sum/write threads count> <threads count> <iterations count>" << std::endl;
            return 0;
        }
        first_rows = atoi(argv[2]);
        first_cols = atoi(argv[3]);
        second_cols = atoi(argv[4]);
        depth = atoi(argv[5]);
        threads_count = atoi(argv[6]);
        mul_threads_count = atoi(argv[7]);
        iterations_count = atoi(argv[8]);
    } else {
        std::cout << "Wrong arguments" << std::endl;
        return 0;
    }

    std::ofstream matrix_file;
    matrix_file.open("result_matrix.txt");
    Buffer* first_matrix_buffer = new Buffer();
    Buffer* second_matrix_buffer = new Buffer();
    Buffer* result_matrix_buffer = new Buffer();

    std::vector<std::thread> gen_threads(threads_count);
    std::vector<std::thread> mult_threads(threads_count);

    for (int i = 0; i < threads_count; i++){
        if (*argv[1] == 'd'){
            gen_threads[i] = std::thread(generate_matrices, first_matrix_buffer, second_matrix_buffer, iterations_count, first_rows, first_cols, second_cols);
            mult_threads[i] = std::thread(mult_matrices, first_matrix_buffer, second_matrix_buffer, result_matrix_buffer, iterations_count, mul_threads_count);
        } else {
            gen_threads[i] = std::thread(generate_matrices, first_matrix_buffer, second_matrix_buffer, iterations_count, first_rows, first_cols, second_cols);
            mult_threads[i] = std::thread(strassen_mult_matrices, first_matrix_buffer, second_matrix_buffer, result_matrix_buffer, depth, threads_count, iterations_count);
        }
    }
    std::thread write_thread = std::thread(write_result, result_matrix_buffer, iterations_count * threads_count, std::ref(matrix_file));
    for (int i = 0; i < threads_count; i++){
        gen_threads[i].join();
        mult_threads[i].join();
    }
    write_thread.join();
    matrix_file.close();
    delete first_matrix_buffer;
    delete second_matrix_buffer;
    delete result_matrix_buffer;
    return 0;
}