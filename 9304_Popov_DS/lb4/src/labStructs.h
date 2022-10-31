#pragma once

#include <vector>
#include <random>
#include <cstdint>

#include <iostream>
#include <memory>


namespace labStruct
{
    class Matrix;
    using uint32 = uint32_t;
    using matrixPtr = std::shared_ptr<labStruct::Matrix>;

    class Matrix
    {
        using col = std::vector<int>;
        using row = std::vector<col>;
    public:
        explicit Matrix(const uint32& side) :
        _side(side)
        {
            _data.resize(_side, col(_side));
        }

        friend matrixPtr operator+(const matrixPtr& left,
                                   const matrixPtr& right);

        friend matrixPtr operator-(const matrixPtr& left,
                                   const matrixPtr& right);

        friend matrixPtr operator*(const matrixPtr& left,
                                   const matrixPtr& right);

        friend Matrix operator+(const Matrix& left,
                                const Matrix& right);

        friend Matrix operator-(const Matrix& left,
                                const Matrix& right);

        friend Matrix operator*(const Matrix& left,
                                const Matrix& right);

        const uint32 _side;
        row _data;
    };

    Matrix operator+(const Matrix& left,
                     const Matrix& right)
    {
        if (left._side == right._side)
        {
            Matrix newMatrix(right._side);

            for (uint32 i = 0; i < newMatrix._side; ++i)
                for (uint32 j = 0; j < newMatrix._side; ++j)
                    newMatrix._data[i][j] = left._data[i][j] + right._data[i][j];

            return newMatrix;
        }

        return Matrix(0);
    }

    Matrix operator-(const Matrix& left,
                     const Matrix& right)
    {
        if (left._side == right._side)
        {
            Matrix newMatrix(right._side);

            for (uint32 i = 0; i < newMatrix._side; ++i)
                for (uint32 j = 0; j < newMatrix._side; ++j)
                    newMatrix._data[i][j] = left._data[i][j] - right._data[i][j];

            return newMatrix;
        }

        return Matrix(0);
    }

    Matrix operator*(const Matrix& left,
                     const Matrix& right)
    {
        if (left._side == right._side)
        {
            Matrix newMatrix(right._side);

            for (uint32 k = 0; k < newMatrix._side; ++k)
                for (uint32 j = 0; j < newMatrix._side; ++j)
                    for (uint32 i = 0; i < newMatrix._side; ++i)
                        newMatrix._data[k][j] += left._data[k][i] * right._data[i][j];

            return newMatrix;
        }

        return Matrix(0);
    }


    matrixPtr operator+(const matrixPtr& left,
                        const matrixPtr& right)
    {
        if (left->_side == right->_side)
        {
            auto newMatrix = std::make_shared<Matrix>(Matrix(right->_side));

            for (uint32 i = 0; i < newMatrix->_side; ++i)
                for (uint32 j = 0; j < newMatrix->_side; ++j)
                    newMatrix->_data[i][j] = left->_data[i][j] + right->_data[i][j];

            return newMatrix;
        }

        return nullptr;
    }

    matrixPtr operator-(const matrixPtr& left,
                        const matrixPtr& right)
    {
        if (left->_side == right->_side)
        {
            auto newMatrix = std::make_shared<Matrix>(Matrix(right->_side));

            for (uint32 i = 0; i < newMatrix->_side; ++i)
                for (uint32 j = 0; j < newMatrix->_side; ++j)
                    newMatrix->_data[i][j] = left->_data[i][j] - right->_data[i][j];

            return newMatrix;
        }

        return nullptr;
    }

    matrixPtr operator*(const matrixPtr& left,
                        const matrixPtr& right)
    {
        if (left->_side == right->_side)
        {
            auto newMatrix = std::make_shared<Matrix>(Matrix(right->_side));

            for (uint32 k = 0; k < newMatrix->_side; ++k)
                for (uint32 j = 0; j < newMatrix->_side; ++j)
                    for (uint32 i = 0; i < newMatrix->_side; ++i)
                        newMatrix->_data[k][j] += left->_data[k][i] * right->_data[i][j];

            return newMatrix;
        }

        return nullptr;
    }



    void generateMatrixData(Matrix* const ptr)
    {
        for (auto& col : ptr->_data)
            for (auto&  element : col)
                element = std::rand() % 10 + 1;
    }
}