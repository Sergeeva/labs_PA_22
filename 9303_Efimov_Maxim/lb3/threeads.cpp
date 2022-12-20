#include "matrix.h"
#include "list.h"
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
#include <atomic>





#define size_bufer 5
enum {N = 3, M = 5};
int threads_count = 5;
List create_and_sum;
List sum_and_print;
std::atomic<int> global_print{0};

void Create()
{
    int global = 0;
    int current = 0;
    while (true) { 
        Matrix matrix = Matrix(N,M);
        create_and_sum.produce(matrix);
        sleep(1);
        current++;
        global ++ ;
        std::cout << "Create: " << global << std::endl; 
        if (current == size_bufer - 1) current = 0;
    }
}

void Print_result()
{
    std::string fileName;
    while (true) {
        Matrix first  = *sum_and_print.consume().get();
        fileName = "result:" + std::to_string(global_print);
        std::cout << fileName << std::endl; 
        first.fileOutput(fileName);        
        std::cout << "print: " << global_print << std::endl; 
        global_print++;
    }
}

void Sum_threads(Matrix& a,Matrix& b,  int index)
{
    for(int i  = index; i < N; i += threads_count) {
        for(int g = 0; g < M; g++ ) {
            a.matrix[i][g] += b.matrix[i][g];
        }
    }
}

void Sum() 
{
    int current = 0;
    while (true) {
        Matrix first  = *(create_and_sum.consume().get());
        Matrix second = *(create_and_sum.consume().get());
        std::vector<std::thread> sum_threads = std::vector<std::thread>(threads_count);
        for (int index = 0; index < threads_count; index++) {
            sum_threads[index] = std::thread(Sum_threads,std::ref(first), std::ref(second), index);
        }
        for(auto& thr: sum_threads) {
            thr.join();
        }
        std::cout << "Sum: " << current << std::endl; 
        sum_and_print.produce(first);
        current++;
    }
}


int main()
{  
    int create_count = 2;
    int sum_count    = 2;
    int print_count  = 2;
    std::vector<std::thread> create_threads = std::vector<std::thread>(10);
    std::vector<std::thread> sum_threads = std::vector<std::thread>(10);
    std::vector<std::thread> print_threads = std::vector<std::thread>(10);

    for (int index = 0; index < create_count; index++) {
        create_threads[index] =   std::thread(Create);
    }

    for (int index = 0; index < sum_count; index++) {
        sum_threads[index] =   std::thread(Sum);
    }
    for (int index = 0; index < print_count; index++) {
        print_threads[index] =   std::thread(Print_result);
    }

    for(auto& thr: sum_threads) {
        thr.join(); 
    }
    return 0;
}
