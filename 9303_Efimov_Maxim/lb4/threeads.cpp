#include "matrix.h"
#include "semafore.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <filesystem>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <unistd.h>
#include <algorithm>


#define size_bufer 5
enum {N = 7, M = 7};
int threads_count = 5;




void Multi_threads(Matrix& first, Matrix& second, Matrix& result,  int index)
{
    int count_rows    = result.matrix.size();
    int count_columns = result.matrix[0].size();
    for(int i  = index; i < count_rows; i += threads_count) {   
        for(int g = 0; g < count_columns;g++){
            result.matrix[i][g] = 0;
            for( int k =0; k < first.matrix[0].size(); k++){
                result.matrix[i][g] += first.matrix[i][k] *  second.matrix[k][g];
            }
        }
    }
}
std::vector<Matrix> split(Matrix &source){
    int rows = source.matrix.size();
    int cols = source.matrix[0].size();

    Matrix m11(rows / 2, cols / 2), m12(rows / 2, cols / 2), m21(rows / 2, cols / 2), m22(rows / 2, cols / 2);
    for (int row = 0; row < rows / 2; row++)
    {
        for (int col = 0; col < cols / 2; col++)
        {
            m11.matrix[col][row] = source.matrix[col][row];
            m12.matrix[col][row] = source.matrix[col ][row + rows/2];
            m21.matrix[col][row] = source.matrix[col + cols/2][row];
            m22.matrix[col][row] = source.matrix[col + cols/2][row + rows / 2];
        }
    }
    std::vector<Matrix> parts;
    parts.push_back(m11);
    parts.push_back(m12);
    parts.push_back(m21);
    parts.push_back(m22);
    return parts;
}


Matrix concat(Matrix& m11, Matrix& m12, Matrix& m21, Matrix& m22) {
        int rows = m11.matrix.size();
        int cols = m11.matrix[0].size();
        Matrix res(rows * 2, cols * 2);

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                res.matrix[col][row]                = m11.matrix[col][row];
                res.matrix[col][row + rows]         = m12.matrix[col][row];
                res.matrix[col + cols][row]         = m21.matrix[col][row];
                res.matrix[col + cols][row + rows] = m22.matrix[col][row];
            }
        }

        return res;
    }
void algorthm(Matrix A_mat, Matrix B_mat, Matrix &result, int n)
{
    if (n > 4)
    {
        std::vector<Matrix> a = split(A_mat);
        std::vector<Matrix> b = split(B_mat);

        Matrix a11 = a[0];
        Matrix a12 = a[1];
        Matrix a21 = a[2];
        Matrix a22 = a[3];

        Matrix b11 = b[0];
        Matrix b12 = b[1];
        Matrix b21 = b[2];
        Matrix b22 = b[3];
        n = n / 2;
        Matrix p1(n,n), p2(n,n), p3(n,n), p4(n,n), p5(n,n), p6(n,n), p7(n,n);
        p1.set_zero();p2.set_zero();p3.set_zero();p4.set_zero();p5.set_zero();p6.set_zero();p7.set_zero();

        std::thread p1_thread(algorthm, a11 + a22, b11 + b22, std::ref(p1), n);
        std::thread p2_thread(algorthm, a21 + a22, b11, std::ref(p2), n);
        std::thread p3_thread(algorthm, a11, b12 - b22, std::ref(p3), n);
        std::thread p4_thread(algorthm, a22, b21 - b11, std::ref(p4), n);
        std::thread p5_thread(algorthm, a11 + a12, b22, std::ref(p5), n);
        std::thread p6_thread(algorthm, a21 - a11, b11 + b12, std::ref(p6), n);
        std::thread p7_thread(algorthm, a12 - a22, b21 + b22, std::ref(p7), n);

        p1_thread.join();
        p2_thread.join();
        p3_thread.join();
        p4_thread.join();
        p5_thread.join();
        p6_thread.join();
        p7_thread.join();

        Matrix c11 = p1 + p4;
        c11 = c11 - p5;
        c11 = c11 + p7;
        Matrix c12 = p3 + p5;
        Matrix c21 = p2 + p4;
        Matrix c22 = p1;
        c22 = c22 - p2;;
        c22 = c22 + p3;
        c22 = c22 + p6;

        result= concat(c11, c12, c21, c22);
    }
    else
    {
        result= A_mat *  B_mat;
    }
}
void SimpleMulti(Matrix& first, Matrix& second,Matrix& result) 
{
    
    std::vector<std::thread> sum_threads = std::vector<std::thread>(threads_count);
    for (int index = 0; index < threads_count; index++) {
        sum_threads[index] = std::thread(Multi_threads,std::ref(first), std::ref(second), std::ref(result), index);
    }
    for(auto& thr: sum_threads) {
        thr.join();
    }
    result.fileOutput("simple_milti");
}


int main()
{  
    int rows_first = M, columns_first = N;
    int rows_second  = columns_first, columns_second = M;
    Matrix first (rows_first,columns_first);
    Matrix second(rows_second,columns_second);
    Matrix result(rows_first,columns_second);
    Matrix test = first * second;
    first.fileOutput("First");
    second.fileOutput("Second");
    test.fileOutput("test_*");


    result.set_zero();
    SimpleMulti(first,second,result);
    result.fileOutput("simple_mylti");
    int size_max = std::max({rows_first, rows_second, columns_first, columns_second});    
    int new_size = 2;
    while (size_max > new_size)
    {
        new_size*= 2;
    }
    first.shape(new_size);
    second.shape(new_size);
    result.shape(new_size);
    first.fileOutput("first with zero");
    second.fileOutput("second with zero ");
    result.set_zero();
    algorthm(first, second, result, M);
    result.fileOutput("algorithm");
    return 0;
}
