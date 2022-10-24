#include "Task3.h"
using namespace std;

int main() 
{
  
    int minThreads = 8;
    int maxThreads = 8;
    
    for (int i = minThreads; i <= maxThreads; i += 1) {

        long long ms = start(i);

        cout<<"Threads: " << i << "\t time:" << to_string(ms) << endl;
    }

    return 0;
}

void readMatrix2(Matrix &Matrix1, Matrix &Matrix2) {
    ifstream inputMatrix2("input.txt");

    inputMatrix2 >> Matrix1;
    inputMatrix2 >> Matrix2;

    inputMatrix2.close();
}

void writeMatrix(const Matrix &matrix, string& name) {
    ofstream outputFile(name);

    outputFile << matrix;
    outputFile.close();
}

long long start(int count_thread) {


    auto Matrix1 = Matrix();
    auto Matrix2 = Matrix();

    thread readThread(function<void (Matrix&, Matrix&)> (readMatrix2), ref(Matrix1), ref(Matrix2));

    promise<Matrix> resultPromise;
    auto resultFuture = resultPromise.get_future();

    readThread.join();

    auto start = chrono::steady_clock::now();
    thread sumThread(sum, ref(Matrix1), ref(Matrix2), ref(resultPromise), count_thread);

    auto result = resultFuture.get();
    auto end = chrono::steady_clock::now();

    string name = "task3";

    thread printThread(function<void (Matrix&, string&)> (writeMatrix), ref(result), ref(name));
    
    sumThread.join();
    printThread.join();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

void sum(const Matrix &Matrix1, const Matrix &Matrix2, promise<Matrix> &resultPromise, int count_thread) {

    auto result = Matrix1.parallelSum(Matrix2, count_thread);

    resultPromise.set_value(result);
}