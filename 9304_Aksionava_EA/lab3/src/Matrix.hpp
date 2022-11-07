#ifndef Matrix_hpp
#define Matrix_hpp

#include <thread>
#include <fstream>
#include <mutex>

#define ROWS 3
#define COLUMNS 3
#define SUM_THREADS 3

class Matrix {
    int first_matrix[ROWS * COLUMNS];
    int second_matrix[ROWS * COLUMNS];
    int result[ROWS * COLUMNS];


    public:
        void generate();

        void print(std::mutex& print_mtx);

        void printToFile(std::ofstream& file, std::mutex& file_mtx);

        void sum();

        Matrix();

    private:
        void sum_matrix_part(int start, int end);
};

#endif