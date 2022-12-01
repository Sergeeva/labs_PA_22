#include <iostream>
#include "matrix.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <filesystem>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

enum {N = 1000, M = 1000};
int threads_count = 7;


void Create(Matrix& a, Matrix& b)
{
    a =  Matrix(N,M);
    b =  Matrix(N,M);


}
void Simple_Sum(Matrix& a, Matrix& b)
{
    a.add(b);
}
void Print_result(Matrix& result)
{
    result.fileOutput("result.txt");
}

void Simple(Matrix& a,Matrix& b,  int index)
{
    for(int i  = index; i < N; i += threads_count) {
        for(int g = 0; g < M; g++ ) {
            a.matrix[i][g] += b.matrix[i][g];
        }
    }
}

int main()
{    
    Matrix first, second;
    std::thread create(Create, std::ref(first),std::ref(second));
    create.join();
    first.fileOutput("first.txt");
    second.fileOutput("second.txt");
    //std::thread sum(Simple_Sum, std::ref(first),std::ref(second));
    //sum.join();
    auto sta = std::chrono::steady_clock::now();
    std::vector<std::thread> sum_threads = std::vector<std::thread>(threads_count);
     for (int index = 0; index < threads_count; index++) {
        sum_threads[index] = std::thread(Simple, std::ref(first), std::ref(second),index);
    }
    for(auto& thr: sum_threads) {
        thr.join();
    }
    std::chrono::duration<double> dur = std::chrono::steady_clock::now() - sta;
    std::cout << "Time for addition " << dur.count() << " seconds" << std::endl;
    std::thread print(Print_result, std::ref(first));
    print.join();

    return 0;
}
