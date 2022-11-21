#ifndef Matrix_hpp
#define Matrix_hpp

#include <thread>
#include <fstream>
#include <mutex>
#include <cmath>

#define FIRST_ROWS 1024
#define FIRST_COLUMNS 1
#define SECOND_ROWS 1
#define SECOND_COLUMNS 1024
#define SUM_THREADS 2

class Matrix {
    public:
        int* first_matrix;
        int* second_matrix;
        int* result;

        void generate();
        void print(std::mutex& print_mtx);
        void printToFile(std::ofstream& file, std::mutex& file_mtx);
        void multiply(int* first, int* second);
        Matrix();

    private:
        void multiply_matrix_part(int* first, int* second, int thread_rows_first_start, int thread_columns_second_start, int thread_rows_first_end, int thread_columns_second_end);
        void choose_part_and_multiply(int* first, int* second, int thread_num_rows, int thread_num_columns, int thread_rows_first, int thread_columns_second);
        int calculate_first_start(int thread_num_rows, int thread_rows_first);
        int calculate_first_end(int thread_num_rows, int thread_rows_first);
        int calculate_second_start(int thread_num_columns, int thread_columns_second);
        int calculate_second_end(int thread_num_columns, int thread_columns_second);
        int find_divisor(int n);
};

#endif