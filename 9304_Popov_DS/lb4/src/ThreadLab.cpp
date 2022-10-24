#include <fstream>
#include <thread>

#include "ThreadLab.h"
#include "labStructs.h"



#undef DURATION_TIME
#define SINGLETHREAD
#define MULTITHREAD
#define STRASSEN

#ifdef DURATION_TIME
#include <chrono>
#include <iostream>
#endif



constexpr unsigned DEGREE = 2;
constexpr unsigned SIDE = (1 << DEGREE);
constexpr unsigned THREADS = 4;



void matrixMultiplication(const std::shared_ptr<labStruct::Cube>& cubePtr);
void matrixStrassenMultiplication(const std::shared_ptr<labStruct::Cube>& cubePtr);



void startLab()
{
    using namespace labStruct;

    auto firstM  = std::make_shared<Matrix>(Matrix(SIDE, SIDE));
    auto secondM = std::make_shared<Matrix>(Matrix(SIDE, SIDE));

    generateMatrixData(firstM.get());
    generateMatrixData(secondM.get());

    auto cube1 = std::make_shared<Cube>(Cube(firstM, secondM));
    auto cube2 = std::make_shared<Cube>(Cube(firstM, secondM));
    auto cube3 = std::make_shared<Cube>(Cube(firstM, secondM));

#ifdef MULTITHREAD
#ifdef DURATION_TIME
auto begin = std::chrono::steady_clock::now();
#endif

    matrixMultiplication(cube1);

    cube1->result();

#ifdef DURATION_TIME
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Multithread solution: " << duration.count() << " ms\n";
#endif   
#endif

#ifdef SINGLETHREAD
#ifdef DURATION_TIME
begin = std::chrono::steady_clock::now();
#endif

    for (uint32 k = 0; k < SIDE; ++k)
        for (uint32 j = 0; j < SIDE; ++j)
            for (uint32 i = 0; i < SIDE; ++i)
                cube2->_result->_data[k][j] += cube2->_first->_data[k][i] * cube2->_second->_data[i][j];

    cube2->result();

#ifdef DURATION_TIME
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Single-threaded solution: " << duration.count() << " ms\n";
#endif  
#endif


#ifdef STRASSEN
#ifdef DURATION_TIME
begin = std::chrono::steady_clock::now();
#endif

    matrixStrassenMultiplication(cube3);

    cube3->result();

#ifdef DURATION_TIME
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Strassen solution: " << duration.count() << " ms\n";
#endif  
#endif
}



void matrixMultiplication(const std::shared_ptr<labStruct::Cube>& cubePtr)
{
    using namespace labStruct;

    auto threadsOnRow = (THREADS / 2);
    auto threadsOnCol = 2u;

    auto rowBit = cubePtr->_first->_rowSize  / threadsOnRow;
    auto colBit = cubePtr->_second->_colSize / threadsOnCol;

    auto multiplication = [&rowBit, &colBit](const std::shared_ptr<labStruct::Cube> cube,
                                             const uint32 rowStart,
                                             const uint32 colStart)
    {
        for (uint32 k = rowStart; k < rowStart + rowBit; ++k)
            for (uint32 j = colStart; j < colStart + colBit; ++j)
                for (uint32 i = 0; i < SIDE; ++i)
                    cube->_result->_data[k][j] += cube->_first->_data[k][i] * cube->_second->_data[i][j];
    };

    std::vector<std::thread> threadPull;

    for (uint i = 0; i < threadsOnRow; ++i)
        for (uint j = 0; j < threadsOnCol; ++j)
            threadPull.emplace_back(multiplication, cubePtr, i * rowBit, j * colBit);
        
    for (auto& thread : threadPull)
        thread.join();
}



void matrixStrassenMultiplication(const std::shared_ptr<labStruct::Cube>& cubePtr)
{
    using namespace labStruct;

    auto sumMatrix = [](uint32 side, const Matrix& m1, const Matrix& m2) -> Matrix
    {
        Matrix sM(side, side);
        for (uint32 i = 0; i < side; ++i)
            for (uint32 j = 0; j < side; ++i)
                sM._data[i][j] =  m1._data[i][j] + m2._data[i][j];

        return sM;
    };

    auto multiplicationMatrix = [](uint32 side, const Matrix& m1, const Matrix& m2) -> Matrix
    {
        Matrix mM(side, side);
        for (uint32 k = 0; k < side; ++k)
            for (uint32 j = 0; j < side; ++j)
                for (uint32 i = 0; i < side; ++i)
                    mM._data[k][j] += mM._data[k][i] * mM._data[i][j];

        return mM;
    };


    auto RL = [&cubePtr, &sumMatrix, &multiplicationMatrix](uint32 side,   // size
                                                            uint32 row,    // X
                                                            uint32 col,    // Y
                                                            auto&& RL) -> Matrix
    {
        if (side > 2)
        {
            auto newSide = side / 2;
            auto a1 = RL(newSide, row, col, RL);
            auto a2 = RL(newSide, row + newSide, col, RL);
            auto a3 = RL(newSide, row, col + newSide, RL);
            auto a4 = RL(newSide, row + newSide, col + newSide, RL);

            
            
        }
        else
        {
            auto p1 = (cubePtr->_first-> _data[row][col] + cubePtr->_first ->_data[row + 1][col + 1]) *
                      (cubePtr->_second->_data[row][col] + cubePtr->_second->_data[row + 1][col + 1]);

            auto p2 = (cubePtr->_first-> _data[row + 1][col] + cubePtr->_first ->_data[row + 1][col + 1]) *
                       cubePtr->_second->_data[row][col];

            auto p3 = (cubePtr->_second-> _data[row][col + 1] - cubePtr->_second ->_data[row + 1][col + 1]) *
                       cubePtr->_first->_data[row][col];

            auto p4 = (cubePtr->_second-> _data[row + 1][col] - cubePtr->_second ->_data[row][col]) *
                       cubePtr->_first->_data[row + 1][col + 1];

            auto p5 = (cubePtr->_first-> _data[row][col] + cubePtr->_first ->_data[row][col + 1]) *
                       cubePtr->_second->_data[row + 1][col + 1];

            auto p6 = (cubePtr->_first-> _data[row + 1][col] - cubePtr->_first ->_data[row][col]) *
                      (cubePtr->_second->_data[row][col] + cubePtr->_second->_data[row][col + 1]);

            auto p7 = (cubePtr->_first-> _data[row][col + 1] - cubePtr->_first ->_data[row + 1][col + 1]) *
                      (cubePtr->_second->_data[row + 1][col] + cubePtr->_second->_data[row + 1][col + 1]);

            auto c1 = p1 + p4 - p5 + p7;
            auto c2 = p3 + p5;
            auto c3 = p2 + p4;
            auto c4 = p1 - p2 + p3 + p6;

            Matrix m(2, 2);
            m._data[0][0] = c1;
            m._data[0][1] = c2;
            m._data[1][0] = c3;
            m._data[1][1] = c4;

            return m;
        }
    };

    RL(SIDE, 0, 0, RL);
}
