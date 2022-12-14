#pragma once

#include <vector>
#include <random>
#include <memory>


namespace labStruct
{
    class Matrix;
    using uint32 = uint32_t;

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

        friend Matrix operator+(const Matrix& left,
                                const Matrix& right);

        friend Matrix operator-(const Matrix& left,
                                const Matrix& right);

        friend Matrix operator*(const Matrix& left,
                                const Matrix& right);

        friend bool operator==(const Matrix& left,
                                 const Matrix& right);

        friend bool operator!=(const Matrix& left,
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

    bool operator==(const Matrix& left,
                    const Matrix& right)
    {
        if (left._side != right._side)
            return false;

        for (uint32 i = 0; i < left._side; ++i)
            for (uint32 j = 0; j < left._side; ++j)
                if (left._data[i][j] != right._data[i][j])
                    return false;
        
        return true;
    }

    bool operator!=(const Matrix& left,
                    const Matrix& right)
    {
        return !(left == right);
    }


    void generateMatrixData(Matrix* const ptr)
    {
        for (auto& col : ptr->_data)
            for (auto&  element : col)
                element = std::rand() % 10 + 1;
    }
}