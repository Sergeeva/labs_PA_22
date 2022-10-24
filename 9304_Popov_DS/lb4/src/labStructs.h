#pragma once

#include <vector>
#include <random>
#include <cstdint>

#include <iostream>
#include <memory>
#include <cassert>


namespace labStruct
{
    using uint32 = uint32_t;

    class Matrix
    {
        using col = std::vector<uint32>;
        using row = std::vector<col>;
    public:
        explicit Matrix(const uint32& rowSize, const uint32& colSize) :
        _rowSize(rowSize),
        _colSize(colSize)
        {
            _data.resize(rowSize);

            for (auto& c : _data)
                c.resize(colSize);
        }

        const uint32 _rowSize;
        const uint32 _colSize;
        row _data; 
    };



    class Cube
    {
        using matrixPtr = std::shared_ptr<Matrix>;
    public:
        explicit Cube(const matrixPtr& firstMatrix, const matrixPtr& secondMatrix) :
        _first(firstMatrix),
        _second(secondMatrix)
        {
            _result = std::make_shared<Matrix>(Matrix(_first->_rowSize, _second->_colSize));

            for (const auto& r : firstMatrix->_data)
            {
                for (const auto& e : r)
                    std::cout << e << ' ';
                std::cout << '\n';
            }
            std::cout << '\n';

            for (const auto& r : secondMatrix->_data)
            {
                for (const auto& e : r)
                    std::cout << e << ' ';
                std::cout << '\n';
            }
            std::cout << '\n';
        }

        matrixPtr _first;
        matrixPtr _second;
        matrixPtr _result;

        void result() const
        {
            for (const auto& r : _result->_data)
            {
                for (const auto& e : r)
                    std::cout << e << ' ';
                std::cout << '\n';
            }
            std::cout << '\n';
        }
    };



    void generateMatrixData(Matrix* const ptr)
    {
        for (auto& col : ptr->_data)
            for (auto&  element : col)
                element = std::rand() % 10 + 1;
    }
}