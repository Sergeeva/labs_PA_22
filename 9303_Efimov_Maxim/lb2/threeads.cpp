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



#define size_bufer 5
enum {N = 3, M = 5};
int threads_count = 5;
std::pair<Matrix, Matrix> bufer_create_and_sum[size_bufer]; 
Matrix bufer_sum_and_print[size_bufer]; 
Semafore full_create(size_bufer);
Semafore empty_create(0);

Semafore full_print(0);
Semafore empty_print(size_bufer);



void Create()
{
    int global = 0;
    int current = 0;
    while (true) { 
        full_create.down(); 
        bufer_create_and_sum[current].first  =  Matrix(N,M);
        bufer_create_and_sum[current].second =  Matrix(N,M);
        sleep(1);
        empty_create.up();
        current++;
        global ++ ;
        std::cout << "Create: " << global << std::endl; 
        if (current == size_bufer - 1) current = 0;
    }
}

void Print_result()
{
    std::string fileName;
    int global = 0;
    int current = 0;
    while (true) {
        full_print.down();
        fileName = "result:" + std::to_string(global);
        std::cout << fileName << std::endl; 
        bufer_create_and_sum[current].first.fileOutput(fileName);
        empty_print.up();
        current++;
        global++;
        if (current == size_bufer - 1) current = 0;
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
        empty_create.down();
        empty_print.down();
        std::vector<std::thread> sum_threads = std::vector<std::thread>(threads_count);
        for (int index = 0; index < threads_count; index++) {
            sum_threads[index] = std::thread(Sum_threads,std::ref(bufer_create_and_sum[current].first), std::ref(bufer_create_and_sum[current].second), index);
            bufer_sum_and_print[current] =  bufer_create_and_sum[current].first;
        }
        for(auto& thr: sum_threads) {
            thr.join();
        }
        full_create.up();
        full_print.up();
        current++;
        if (current == size_bufer - 1) current = 0;
    }

}


int main()
{  
    std::thread create(Create);
    std::thread sum(Sum);
    std::thread print(Print_result);
    create.join();
    return 0;
}
