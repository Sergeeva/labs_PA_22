#include "Task2.h"
using namespace std;

int main()
{
    
    auto Matrix1 = Matrix();
    auto Matrix2 = Matrix();

    thread inputThread(Matrix::readMatrix2, ref(Matrix1), ref(Matrix2));

    promise<Matrix> resultPromise;

    auto resultFuture = resultPromise.get_future(); //обертка, над каким-либо значением или объектом(далее значением), вычисление или получение которого происходит отложено. 

    inputThread.join(); //остановили текущий поток (main) до конца выполнения inputThread


    thread sumThread(sum, ref(Matrix1), ref(Matrix2), ref(resultPromise));
    auto result = resultFuture.get();

    thread printThread(Matrix::writeMatrix, ref(result), "task2");

    sumThread.join();
    printThread.join();
    return 0;
}

void sum(const Matrix &Matrix1, const Matrix &Matrix2, promise<Matrix> &resultPromise)
{
    auto result = Matrix1 + Matrix2;
    resultPromise.set_value(result); //записываем
}