#pragma once

#include <vector>
#include <random>
#include <cstdint>
#include <atomic>

#include <iostream>
#include <memory>
#include <cassert>


namespace labStruct
{
    using uint32 = uint32_t;



    template<typename T = uint32>
    class Matrix
    {
        using col = std::vector<T>;
        using row = std::vector<col>;
    public:
        explicit Matrix(const uint32& rowSize, const uint32& colSize) :
        _rowSize(rowSize),
        _colSize(colSize)
        {
            assert(_rowSize > 0 && _colSize > 0);

            _data.resize(rowSize);

            for (auto& c : _data)
                c.resize(colSize);
        }

        const uint32 _rowSize;
        const uint32 _colSize;
        row _data; 
    };



    template<typename T>
    class Cube
    {
        using matrixPtr = std::shared_ptr<Matrix<T>>;
    public:
        explicit Cube(matrixPtr firstMatrix, matrixPtr secondMatrix) :
        _first(firstMatrix),
        _second(secondMatrix)
        {
            assert(_first->_colSize == _second->_rowSize && 
                   _second->_colSize == _first->_rowSize);

            _result = std::make_shared<Matrix<T>>(Matrix(_first->_rowSize, _second->_colSize));
        }

        matrixPtr _first;
        matrixPtr _second;
        matrixPtr _result;

    private:
    };


    template<typename T>
    void generateMatrixData(Matrix<T>* ptr)
    {
        for (auto& col : ptr->_data)
            for (auto&  element : col)
                element = 1;
    }
}