#include <fstream>
#include <thread>

#include "ThreadLab.h"
#include "labStructs.h"

#define ROW 300
#define COL 300

#undef DURATION_TIME

#ifdef DURATION_TIME
#include <chrono>
#include <iostream>
#endif



template<typename T>
void matrixMultiplication(std::shared_ptr<labStruct::Cube<T>> cubePtr)
{
    using namespace labStruct;

    auto availableStreams = std::thread::hardware_concurrency() - 2;

    auto rowBit = cubePtr->_first->_rowSize  / availableStreams;
    auto colBit = cubePtr->_second->_colSize / availableStreams;

    auto e_r = cubePtr->_first->_rowSize  % availableStreams;
    auto c_r = cubePtr->_second->_colSize % availableStreams;

    auto multiplication = [&rowBit, &colBit](std::shared_ptr<labStruct::Cube<T>> cube,
                                             const uint32 rowStart,
                                             const uint32 colStart,
                                             const uint32 e_r = 0,
                                             const uint32 c_r = 0)
    {
        std::cout << rowStart << ' ' << colStart << '\n';


        // Продумать алгоритм

        for (uint32 k = 0; k < rowBit * colBit; ++k)
        {
           
            for (uint32 j = colStart; j < colStart + colBit; ++j)
            {
                for (uint32 i = rowStart; i < rowStart + rowBit; ++i)
                    cube->_result[i][j] += cube->_first[i][j] * cube->_second[j][i];
            }
        }



    };

    std::vector<std::thread> threadPull;

    for (uint32 t = 0; t < availableStreams; ++t)
        threadPull.emplace_back(multiplication, cubePtr, t * rowBit, t * colBit);

    threadPull.emplace_back(multiplication, cubePtr, rowBit, colBit, e_r, c_r);

    for (auto& thread : threadPull)
        thread.join();


    std::cout << rowBit << ' ' << colBit << ' ' << e_r << ' ' << c_r << '\n';

}


void startLab()
{
    using namespace labStruct;

    auto firstM  = std::make_shared<Matrix<uint32>>(Matrix(ROW, COL));
    auto secondM = std::make_shared<Matrix<uint32>>(Matrix(ROW, COL));

    generateMatrixData(firstM.get());
    generateMatrixData(secondM.get());

    auto cube = std::make_shared<Cube<uint32>>(Cube(firstM, secondM));

#ifdef DURATION_TIME
auto begin = std::chrono::steady_clock::now();
#endif

    std::thread multiplication(matrixMultiplication<uint32>, cube);
    multiplication.join();

#ifdef DURATION_TIME
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << duration.count() << " ms\n";
#endif   

    // std::thread multiplicationStrassen(matrixMultiplication<uint32>, cube);
    // multiplicationStrassen.join();

}



