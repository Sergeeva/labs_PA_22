#include <mutex>
#include <thread>
#include <iostream>

#include "thread.h"
void startThreadMode(int M, int N, int T){
    std::mutex mutex;
    int offsetM = 0;
    int offsetN = 0;
    std::vector<std::thread*> threads;
    threads.reserve(T);

    while(M % T != 0) {
        offsetM++;
        M++;
    }

    while(N % T != 0){
        offsetN++;
        N++;
    }

    Matrix A(M, N, offsetM, offsetN);
    Matrix B(M, N, offsetM, offsetN);
    Matrix C(M, N, offsetM, offsetN);

    std::thread threadCreate(fillMatrix, std::ref(A), std::ref(B), std::ref(mutex));
    threadCreate.join();

    if(T > 1) {
        for (int i = 0; i < T; i++) {
            auto *tempThread = new std::thread(sumMatrix, std::ref(A), std::ref(B), std::ref(C), std::ref(mutex), i, T);
            threads.push_back(tempThread);
        }
    }

    auto begin = std::chrono::steady_clock::now();
    if(T == 1)
        sumOneMatrix(A, B, C);
    else {
        for (int i = 0; i < T; i++)
            threads[i]->join();
    }
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << T << " threads: Time - " << duration.count() << " milliseconds\n";

    std::thread threadPrint(printMatrix, std::ref(A), std::ref(B), std::ref(C), std::ref(mutex));
    threadPrint.join();

    for(int i = 0; i < T; i++)
        delete threads[i];

}

void fillMatrix(Matrix& A, Matrix& B, std::mutex& mutex){
    mutex.lock();
    A.fillMatrix();
    B.fillMatrix();
    mutex.unlock();
}

void sumMatrix(Matrix& A, Matrix& B, Matrix& C, std::mutex& mutex, int tempThread, int T){
    int step = A.matrix.size() / T;
    int rem = A.matrix.size() % T;
    C.sumMatrix(A, B, tempThread, T, tempThread * step, (tempThread+1) * step + (tempThread == T - 1 ? rem : 0));
}

void sumOneMatrix(Matrix& A, Matrix& B, Matrix& C){
    C = A + B;
}

void printMatrix(Matrix& A, Matrix& B, Matrix& C, std::mutex& mutex){
    mutex.lock();
    std::fstream ao, bo, co;
    ao.open("aoutput.json", std::ios_base::out);
    bo.open("boutput.json", std::ios_base::out);
    co.open("coutput.json", std::ios_base::out);

    if(!ao.is_open() || !bo.is_open() || !co.is_open()){
        std::cerr << "File is undefined\n";
        std::exit(-1);
    }

    ao << A;
    bo << B;
    co << C;

    ao.close();
    bo.close();
    co.close();
}