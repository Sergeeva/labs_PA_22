#include <fstream>
#include <thread>

#include "ThreadLab.h"
#include "labStructs.h"



#define DURATION_TIME
#undef SINGLETHREAD
#define MULTITHREAD
#define STRASSEN

#ifdef DURATION_TIME
#include <chrono>
#include <iostream>
std::chrono::_V2::steady_clock::time_point begin;
std::chrono::_V2::steady_clock::time_point end;
std::chrono::milliseconds duration;
#endif



constexpr unsigned DEGREE = 10;
constexpr unsigned SIDE = (1 << DEGREE);
constexpr unsigned THREADS = 4;



std::shared_ptr<labStruct::Matrix> matrixMultiplication(const std::shared_ptr<labStruct::Matrix>&,
                                                        const std::shared_ptr<labStruct::Matrix>&);

std::shared_ptr<labStruct::Matrix> matrixStrassenMultiplication(const std::shared_ptr<labStruct::Matrix>&,
                                                                const std::shared_ptr<labStruct::Matrix>&);

void outputMatrix(const std::shared_ptr<labStruct::Matrix>& matrix);



void startLab()
{
    using namespace labStruct;

    auto firstM  = std::make_shared<Matrix>(Matrix(SIDE));
    auto secondM = std::make_shared<Matrix>(Matrix(SIDE));

    generateMatrixData(firstM.get());
    generateMatrixData(secondM.get());

#ifdef MULTITHREAD
#ifdef DURATION_TIME
    begin = std::chrono::steady_clock::now();
#endif
    matrixMultiplication(firstM, secondM);
    //outputMatrix(matrixMultiplication(firstM, secondM));

#ifdef DURATION_TIME
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Multithread solution: " << duration.count() << " ms\n";
#endif   
#endif

#ifdef SINGLETHREAD
#ifdef DURATION_TIME
    begin = std::chrono::steady_clock::now();
#endif

    firstM * secondM;
    //outputMatrix(firstM * secondM);

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

    matrixStrassenMultiplication(firstM, secondM);
    //outputMatrix(matrixStrassenMultiplication(firstM, secondM));

#ifdef DURATION_TIME
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "Strassen solution: " << duration.count() << " ms\n";
#endif  
#endif
}



std::shared_ptr<labStruct::Matrix> matrixMultiplication(const std::shared_ptr<labStruct::Matrix>& firstMatrix,
                                                        const std::shared_ptr<labStruct::Matrix>& secondMatrix)
{
    using namespace labStruct;

    auto threadsOnRow = (THREADS / 2);
    auto threadsOnCol = 2u;

    auto rowBit = firstMatrix->_side  / threadsOnRow;
    auto colBit = secondMatrix->_side / threadsOnCol;

    auto resultMatrix = std::make_shared<Matrix>(Matrix(firstMatrix->_side));

    auto multiplication = [&rowBit, &colBit](const std::shared_ptr<Matrix>& firstMatrix,
                                             const std::shared_ptr<Matrix>& secondMatrix,
                                             const std::shared_ptr<Matrix>& resultMatrix,
                                             const uint32 rowStart,
                                             const uint32 colStart)
    {
        for (uint32 k = rowStart; k < rowStart + rowBit; ++k)
            for (uint32 j = colStart; j < colStart + colBit; ++j)
                for (uint32 i = 0; i < SIDE; ++i)
                    resultMatrix->_data[k][j] += firstMatrix->_data[k][i] * secondMatrix->_data[i][j];
    };

    std::vector<std::thread> threadPull;

    for (uint i = 0; i < threadsOnRow; ++i)
        for (uint j = 0; j < threadsOnCol; ++j)
            threadPull.emplace_back(multiplication, firstMatrix, secondMatrix, resultMatrix, i * rowBit, j * colBit);
        
    for (auto& thread : threadPull)
        thread.join();

    return resultMatrix;
}



std::shared_ptr<labStruct::Matrix> matrixStrassenMultiplication(const std::shared_ptr<labStruct::Matrix>& firstMatrix,
                                                                const std::shared_ptr<labStruct::Matrix>& secondMatrix)
{
    using namespace labStruct;

    auto splitMatrix = [](const Matrix& mainMatrix,
                          Matrix& a11,
                          Matrix& a12,
                          Matrix& a21,
                          Matrix& a22)
    {
        const uint sideSubMatrix = mainMatrix._side >> 1;


        for (uint32 i = 0; i < sideSubMatrix; ++i)
            for (uint32 j = 0; j < sideSubMatrix; ++j)
                a11._data[i][j] = mainMatrix._data[i][j];

        for (uint32 i = 0; i < sideSubMatrix; ++i)
            for (uint32 j = sideSubMatrix; j < mainMatrix._side; ++j)
                a12._data[i][j - sideSubMatrix] = mainMatrix._data[i][j];

        for (uint32 i = sideSubMatrix; i < mainMatrix._side; ++i)
            for (uint32 j = 0; j < sideSubMatrix; ++j)
                a21._data[i - sideSubMatrix][j] = mainMatrix._data[i][j];

        for (uint32 i = sideSubMatrix; i < mainMatrix._side; ++i)
            for (uint32 j = sideSubMatrix; j < mainMatrix._side; ++j)
                a22._data[i - sideSubMatrix][j - sideSubMatrix] = mainMatrix._data[i][j];
    };


    auto collectMatrix = [](const Matrix& a11,
                            const Matrix& a12,
                            const Matrix& a21,
                            const Matrix& a22) -> Matrix
    {
        auto result = Matrix(a11._side << 1);

        for (uint32 i = 0; i < a11._side; ++i)
            for (uint32 j = 0; j < a11._side; ++j)
                result._data[i][j] = a11._data[i][j];

        for (uint32 i = a21._side; i < result._side; ++i)
            for (uint32 j = 0; j < a21._side; ++j)
                result._data[i][j] = a21._data[i - a21._side][j];

        for (uint32 i = 0; i < a12._side; ++i)
            for (uint32 j = a12._side; j < result._side; ++j)
                result._data[i][j] = a12._data[i][j - a12._side];

        for (uint32 i = a22._side; i < result._side; ++i)
            for (uint32 j = a22._side; j < result._side; ++j)
                result._data[i][j] = a22._data[i - a22._side][j - a22._side];

        return result;
    };


    auto RL = [&splitMatrix, &collectMatrix](const Matrix& a,
                                             const Matrix& b,
                                             uint32& side,
                                             auto&& RL) -> Matrix
    {
        if (side <= 64)
            return a * b;

        side =  side >> 1;
        auto a11 = Matrix(side);
        auto a12 = Matrix(side);
        auto a21 = Matrix(side);
        auto a22 = Matrix(side);

        auto b11 = Matrix(side);
        auto b12 = Matrix(side);
        auto b21 = Matrix(side);
        auto b22 = Matrix(side);

        splitMatrix(a, a11, a12, a21, a22);
        splitMatrix(b, b11, b12, b21, b22);

        auto p1 = RL(a11 + a22, b11 + b22, side, RL);
        auto p2 = RL(a21 + a22, b11, side, RL);
        auto p3 = RL(a11, b12 - b22, side, RL);
        auto p4 = RL(a22, b21 - b11, side, RL);
        auto p5 = RL(a11 + a12, b22, side, RL);
        auto p6 = RL(a21 - a11, b11 + b12, side, RL);
        auto p7 = RL(a12 - a22, b21 + b22, side, RL);

        auto c11 = (p1 + p4) + (p7 - p5);
        auto c12 = p3 + p5;
        auto c21 = p2 + p4;
        auto c22 = (p1 - p2) + (p3 + p6);

        return collectMatrix(c11, c12, c21, c22);
    };


    auto newSide = SIDE;

    auto threadL = [&RL, &newSide](Matrix& first,
                                   Matrix& second,
                                   Matrix& result)
    {
        result._data = RL(first, second, newSide, RL)._data;
    };

    

    // auto t11 = Matrix(newSide);
    // auto t12 = Matrix(newSide);
    // auto t13 = Matrix(newSide);
    // auto t14 = Matrix(newSide);

    // auto t21 = Matrix(newSide);
    // auto t22 = Matrix(newSide);
    // auto t23 = Matrix(newSide);
    // auto t24 = Matrix(newSide);

    // splitMatrix(*firstMatrix.get(), t11, t12, t13, t14);
    // splitMatrix(*secondMatrix.get(), t21, t22, t23, t24);

    // auto result1 = Matrix(newSide);
    // auto result2 = Matrix(newSide);
    // auto result3 = Matrix(newSide);
    // auto result4 = Matrix(newSide);

    auto result = Matrix(firstMatrix->_side);
    threadL(*firstMatrix.get(), *secondMatrix.get(), result);

    return std::make_shared<Matrix>(result);
}



void outputMatrix(const std::shared_ptr<labStruct::Matrix>& matrix)
{
    for (const auto& r : matrix->_data)
    {
        for (const auto& e : r)
            std::cout << e << ' ';
        std::cout << '\n';
    }
    std::cout << '\n';
}