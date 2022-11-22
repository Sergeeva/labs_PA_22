#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include "Buffer.h"
#include "Matrix.h"

using namespace std;

void matrices_generate(Buffer& matrix1_buffer, Buffer& matrix2_buffer, int count_iterations, int rows, int columns)
{
    for (int i = 0; i < count_iterations; i++)
    {

        Matrix matrix1(rows, columns), matrix2(rows, columns);
        string input = "input.txt";
        string input2 = "input2.txt";

        Matrix:: generate(input, rows, columns);
        Matrix:: generate(input2, rows, columns);

        ifstream inputMatrix("input.txt");
        ifstream inputMatrix2("input2.txt");

        inputMatrix >> matrix1;
        inputMatrix2 >> matrix2;

        inputMatrix.close();
        inputMatrix2.close();

        matrix1_buffer.produce(matrix1);
        matrix2_buffer.produce(matrix2);
    }
}

void matrices_sum(Buffer& matrix1_buffer, Buffer& matrix2_buffer, Buffer& result_buffer, int count_iterations, int rows, int columns, int count_threads)
{
    for (int i = 0; i < count_iterations; i++)
    {
        Matrix matrix1 = matrix1_buffer.consume();
        Matrix matrix2 = matrix2_buffer.consume();
        Matrix result =  matrix1.parallelSum(matrix2, count_threads);
        result_buffer.produce(result);
    }
}

void write_result(Buffer& result_buffer, int count_iterations, ostream& res)
{
    for (int i = 0; i < count_iterations; i++)
    {
        Matrix result = result_buffer.consume();
        res << result << '\n';
    }
};

int main(int argc, char *argv[]){
   
    int rows;
    int columns;
    int count_threads;
    int count_iterations;

    cin >> rows ;
    cin >> columns ;
    cin >> count_threads ;
    cin >> count_iterations ;
    
    
    srand(time(0));
    ofstream res_matrix;
    res_matrix.open("result.txt");
    
    Buffer matrix1_buffer(10), matrix2_buffer(10), result_buffer(10);
    
    thread generate = thread(matrices_generate, ref(matrix1_buffer), ref(matrix2_buffer), count_iterations, rows, columns);

    thread sum = thread(matrices_sum, ref(matrix1_buffer), ref(matrix2_buffer), ref(result_buffer), count_iterations, rows, columns, count_threads);
    
    thread write = thread(write_result, ref(result_buffer), count_iterations, ref(res_matrix));
    
    generate.join();
    sum.join();
    write.join();
    
    return 0;
}