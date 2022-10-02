#include <random>
#include <fstream>
#include <thread>

#include "ThreadLab.h"
#include "labStructs.h"

#define ROW 10'000
#define COL 10'000

#undef MULTITHREAD_SUM
#undef DURATION_TIME

#ifdef DURATION_TIME
#include <chrono>
#include <iostream>
#endif



void generateMatrix(labStruct::MatrixQueue* const, const labStruct::uint&);
void sumMatrix(labStruct::MatrixQueue* const,      const labStruct::uint&);
void resultMatrix(labStruct::MatrixQueue* const,   const labStruct::uint&);



void startLab(const labStruct::uint& count)
{
    auto queue = new labStruct::MatrixQueue;

#ifdef DURATION_TIME
    auto begin = std::chrono::steady_clock::now();
#endif

    std::thread firstThread (generateMatrix, queue, count);
    std::thread secondThread(sumMatrix,      queue, count);
    std::thread thirdThread (resultMatrix,   queue, count);

    firstThread.join();
    secondThread.join();
    thirdThread.join();

#ifdef DURATION_TIME
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
#endif

    delete queue;
}



void generateMatrix(labStruct::MatrixQueue* const queue, const labStruct::uint& count)
{
    labStruct::uint generateCount = 0;

    while (generateCount != count)
    {
        auto matrix = new labStruct::Matrix(ROW, COL);

        for (labStruct::uint i = 0; i < matrix->firstMatrix.size(); ++i)
        {
            // rand() дорогая операция, странно работает в многопотоке.
            matrix->firstMatrix [i] = 1;//rand() % 28 + 1;
            matrix->secondMatrix[i] = 2;//rand() % 28 + 41;
        }

        queue->pushInMatrixQueue(matrix);

        ++generateCount;
    }
}



void sumMatrix(labStruct::MatrixQueue* const queue, const labStruct::uint& count)
{
    labStruct::uint sumCount = 0;

    while (sumCount != count)
    {
        auto matrix = queue->retrieveAndDeleteMatrix();

        if (!matrix)
            continue;

        std::vector<labStruct::uint> result;
        result.resize(matrix->firstMatrix.size());

#ifdef MULTITHREAD_SUM
    auto sum = [&result, &matrix](const labStruct::uint index, const labStruct::uint size)
    {
        for (labStruct::uint i = index; i < index + size; ++i)
            result[i] = matrix->firstMatrix[i] + matrix->secondMatrix[i];
    };

    unsigned f = matrix->firstMatrix.size() / 5;
    unsigned e = matrix->firstMatrix.size() % 5;

    std::vector<std::thread> pullThread;

    for (unsigned i = 5; i < 1; ++i)
        pullThread.emplace_back(sum, i * f, f);


    pullThread.emplace_back(sum, matrix->firstMatrix.size() - (f + e), f + e);

    for (auto& t : pullThread)
        t.join();

#else
       for (labStruct::uint i = 0; i < matrix->firstMatrix.size(); ++i)
            result[i] = matrix->firstMatrix [i] + matrix->secondMatrix[i];   
#endif

        queue->pushInResultQueue(result);

        ++sumCount;

        delete matrix;
    }
}



void resultMatrix(labStruct::MatrixQueue* const queue, const labStruct::uint& count)
{
    labStruct::uint outputCount = 0;
    std::ofstream resultFile("./result.txt");

    while (outputCount != count)
    {
        auto result = queue->retrieveAndDeleteResult();

        if (result.empty())
            continue;

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