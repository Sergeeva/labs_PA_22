#include <fstream>
#include <thread>

#include "ThreadLab.h"
#include "labStructs.h"

#define ROW 10
#define COL 10

#define DURATION_TIME

#ifdef DURATION_TIME
#include <chrono>
#include <iostream>
#endif



void generateMatrix(labStruct::CustomQueue* const, const labStruct::uint&);
void sumMatrix     (labStruct::CustomQueue* const, labStruct::CustomQueue* const, const labStruct::uint&);
void resultMatrix  (labStruct::CustomQueue* const, const labStruct::uint&);



void startLab(const labStruct::uint& count)
{
    auto matrixQueue = new labStruct::CustomQueue;
    auto resultQueue = new labStruct::CustomQueue;

#ifdef DURATION_TIME
    auto begin = std::chrono::steady_clock::now();
#endif

    std::thread firstThread (generateMatrix, matrixQueue, count);
    std::thread secondThread(sumMatrix,      matrixQueue, resultQueue, count);
    std::thread thirdThread(resultMatrix,    resultQueue, count);

    firstThread.join();
    secondThread.join();

#ifdef DURATION_TIME
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << duration.count() << " ms\n";
#endif

    thirdThread.join();

    delete matrixQueue;
    delete resultQueue;
}



void generateMatrix(labStruct::CustomQueue* const queue, const labStruct::uint& count)
{
    labStruct::uint generateCount = 0;

    while (generateCount != count)
    {
        auto matrix = new labStruct::Matrix(ROW, COL);

        for (labStruct::uint i = 0; i < matrix->firstMatrix.size(); ++i)
        {
            matrix->firstMatrix [i] = 1; //labStruct::random::getRandomNumber();
            matrix->secondMatrix[i] = 2; //labStruct::random::getRandomNumber();
        }

        queue->push(matrix);

        ++generateCount;
    }
}



void sumMatrix(labStruct::CustomQueue* const matrixsQueue, labStruct::CustomQueue* const resultQueue, const labStruct::uint& count)
{
    labStruct::uint sumCount = 0;

    while (sumCount != count)
    {
        auto matrixs = matrixsQueue->pop();

        for (labStruct::uint i = 0; i < matrixs->firstMatrix.size(); ++i)
            matrixs->firstMatrix[i] += matrixs->secondMatrix[i];   

        resultQueue->push(matrixs);

        ++sumCount;
    }
}



void resultMatrix(labStruct::CustomQueue* const queue, const labStruct::uint& count)
{
    labStruct::uint outputCount = 0;
    std::ofstream resultFile("./result.txt");

    while (outputCount != count)
    {
        auto result = queue->pop();

        if (resultFile)
        {
            for (labStruct::uint j = 0; j < COL; ++j)
            {
                for (labStruct::uint i = j; i < result->firstMatrix.size(); i += COL)
                    resultFile << result->firstMatrix[i] << ' ';

                resultFile << '\n';
            }
            resultFile << '\n';
        }

        ++outputCount;
        delete result;
    }
    
    if (resultFile.is_open())
        resultFile.close();
}