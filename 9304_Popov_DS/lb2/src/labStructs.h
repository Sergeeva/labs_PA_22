#pragma once

#include <list>
#include <queue>
#include <vector>
#include <cstdint>
#include <mutex>



namespace labStruct
{

using uint = uint32_t;

struct Matrix {

    explicit Matrix(const uint& row, const uint& column)
    {
        const uint size =  row * column;
        firstMatrix.resize(size);
        secondMatrix.resize(size);
    }

    std::vector<uint> firstMatrix;
    std::vector<uint> secondMatrix;
};


struct MatrixQueue {
    void pushInMatrixQueue(Matrix* const matrix)
    {
        std::lock_guard<std::mutex> ul(matrixMutex);
        matrixQueue.push(matrix);
    }

    Matrix* retrieveAndDeleteMatrix()
    {
        Matrix* fronElement = nullptr;

        std::lock_guard<std::mutex> ul(matrixMutex);

        if (!matrixQueue.empty())
        {
            fronElement = matrixQueue.front();
            matrixQueue.pop();
        }

        return fronElement;
    }

    void pushInResultQueue(const std::vector<uint>& matrix)
    {
        std::lock_guard<std::mutex> ul(resultMutex);
        resultQueue.push(matrix);
    }

    std::vector<uint> retrieveAndDeleteResult()
    {
        std::vector<uint> result;

        std::lock_guard<std::mutex> ul(resultMutex);

        if (!resultQueue.empty())
        {
            result = resultQueue.front();
            resultQueue.pop();
        }

        return result;
    }

private:
    std::queue<Matrix*> matrixQueue;
    std::queue<std::vector<uint>> resultQueue;
    std::mutex matrixMutex;
    std::mutex resultMutex;
};

}