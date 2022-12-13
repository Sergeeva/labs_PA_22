#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>
using namespace std;

struct Matrix {
    
    vector<vector<int>> matrix;

    int columns = 0;
    int rows = 0;

    int id = -1;

    Matrix() = default;

    Matrix(const Matrix &matrix) = default;

    Matrix(int columns, int rows);

    ~Matrix() = default;

    bool check_dimensions(const Matrix& other) const;

    void set_element(int rows, int columns, int val);

    size_t get_rows() const;

    size_t get_columns() const;

    Matrix operator+(const Matrix &matrix) const;

    Matrix parallelSum(const Matrix &matrix, int count_thread) const;

    static void sumPart(const Matrix &Matrix1, const Matrix &Matrix2, int start, int length, Matrix &result);

    friend istream &operator>>(istream &in, Matrix &matrix);

    friend ostream &operator<<(ostream &out, const Matrix &matrix);

    static void writeMatrix(const Matrix &matrix, const string& name);

    static void readMatrix2(Matrix &Matrix1, Matrix &Matrix2);

    static Matrix generate(int rows, int columns);


};