#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <atomic>
#include "Buffer.h"
#include "Matrix.h"


using namespace std;

std::atomic<int> count{0};

int make_id() {
    return count++;
}


void matrices_generate(Buffer<pair <Matrix, Matrix>>* matrix_buffer, int count_iterations, int rows, int columns)
{

     ofstream res_matrix;
   
    for (int i = 0; i < count_iterations; i++)
    {         

        Matrix matrix1(rows, columns), matrix2(rows, columns);

        matrix1 = Matrix:: generate(rows, columns);
        matrix2 = Matrix:: generate(rows, columns);

        int id = make_id();

        matrix1.id = id;
        matrix2.id = id;

        res_matrix.open("./" + to_string(id) + ".txt", fstream::app);
   
        res_matrix <<"1:\n" << matrix1;
        
        res_matrix<<"2:\n" << matrix2;

        res_matrix.close();
    
        matrix_buffer->produce(make_pair(matrix1,matrix2));        
        
    }
}

void matrices_sum(Buffer<pair <Matrix, Matrix>>* matrix_buffer,  Buffer <Matrix>* result_buffer, int count_iterations, int rows, int columns, int count_threads)
{
    
    for (int i = 0; i < count_iterations; i++)
    {

        pair <Matrix, Matrix> matrix_pair = *matrix_buffer->consume().get();

        Matrix matrix1 = matrix_pair.first;
        Matrix matrix2 = matrix_pair.second;

        Matrix result =  matrix1.parallelSum(matrix2, count_threads);

        result.id = matrix1.id;

        result_buffer->produce(result);
        
    }
}

void write_result(Buffer<Matrix>* result_buffer, int count_iterations)
{

    ofstream res_matrix;
   

    for (int i = 0; i < count_iterations; i++)
    {
        Matrix result = *result_buffer->consume().get();
         
        res_matrix.open("./" + to_string(result.id) + ".txt", fstream::app);

        res_matrix<<"1+2:\n" << result;

        res_matrix.close();

    }
};


int main(int argc, char *argv[]){
   
    int rows;
    int columns;
    int count_threads;
    int count_iterations;
    int sum_threads_count;

    cin >> rows ;
    cin >> columns ;
    cin >> count_threads ;
    cin >> count_iterations ;
    cin >> sum_threads_count;
    
    
    srand(time(0));
   // ofstream res_matrix;
  //  res_matrix.open("result.txt");
    
    Buffer<pair <Matrix, Matrix>>* matrix_buffer = new Buffer<pair <Matrix, Matrix>>();
    Buffer<Matrix>* result_buffer = new Buffer<Matrix>();

    std::vector<std::thread> gen_threads(count_threads);
    std::vector<std::thread> sum_threads(count_threads);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < count_threads; i++){

        gen_threads[i] = thread(matrices_generate, matrix_buffer, count_iterations, rows, columns);
        sum_threads[i] = thread(matrices_sum, matrix_buffer, result_buffer, count_iterations, rows, columns, sum_threads_count);

    }
    
    std::thread write_thread = thread(write_result, result_buffer, count_iterations * count_threads);

    
    for (int i = 0; i < count_threads; i++){
        gen_threads[i].join();
        sum_threads[i].join();
    }
    write_thread.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Продолжительность - " << duration.count() << "ms" << std::endl;    
    
  //  res_matrix.close();

    delete matrix_buffer;
    delete result_buffer;
    return 0;

}
