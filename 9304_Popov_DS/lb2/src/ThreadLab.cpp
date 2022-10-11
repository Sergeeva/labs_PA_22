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



void generateMatrix(labStruct::CustomQueue<labStruct::Matrix*>* const, const labStruct::uint&);
void sumMatrix(labStruct::CustomQueue<labStruct::Matrix*>* const,
               labStruct::CustomQueue<std::vector<labStruct::uint>>* const,
               const labStruct::uint&);
void resultMatrix  (labStruct::CustomQueue<std::vector<labStruct::uint>>* const, const labStruct::uint&);



void startLab(const labStruct::uint& count)
{
    auto matrixQueue = new labStruct::CustomQueue<labStruct::Matrix*>;
    auto resultQueue = new labStruct::CustomQueue<std::vector<labStruct::uint>>;

#ifdef DURATION_TIME
    auto begin = std::chrono::steady_clock::now();
#endif

    std::thread firstThread (generateMatrix, matrixQueue, count);
    std::thread secondThread(sumMatrix,      matrixQueue, resultQueue, count);
    std::thread thirdThread (resultMatrix,   resultQueue, count);

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



void generateMatrix(labStruct::CustomQueue<labStruct::Matrix*>* const queue, const labStruct::uint& count)
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



void sumMatrix(labStruct::CustomQueue<labStruct::Matrix*>* const matrixsQueue,
               labStruct::CustomQueue<std::vector<labStruct::uint>>* const resultQueue,
               const labStruct::uint& count)
{
    labStruct::uint sumCount = 0;

    while (sumCount != count)
    {
        auto matrixs = matrixsQueue->pop();

        std::vector<labStruct::uint> result(matrixs->firstMatrix.size());

        for (labStruct::uint i = 0; i < matrixs->firstMatrix.size(); ++i)
            result[i] = matrixs->firstMatrix[i] + matrixs->secondMatrix[i];   

        resultQueue->push(result);

        ++sumCount;

        delete matrixs;
    }
}



void resultMatrix(labStruct::CustomQueue<std::vector<labStruct::uint>>* const queue, const labStruct::uint& count)
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
                for (labStruct::uint i = j; i < result.size(); i += COL)
                    resultFile << result[i] << ' ';

                resultFile << '\n';
            }
            resultFile << '\n';
        }

        ++outputCount;
    }
    
    if (resultFile.is_open())
        resultFile.close();
}