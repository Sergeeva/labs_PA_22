#ifndef UNTITLED_MATRIX_H
#define UNTITLED_MATRIX_H


#include <vector>
#include <ostream>
#include <iostream>
#include <atomic>

class Matrix {

private:
    std::vector<std::vector<int>> matrix;
    int width = 1;
    int height = 1;
    std::atomic<bool> changed = false;

public:
    Matrix() : changed(std::atomic<bool>(false)) {};

    Matrix(const Matrix &matrix) : matrix(matrix.matrix),
                                   width(matrix.width),
                                   height(matrix.height),
                                   changed(std::atomic<bool>(false)) {}

    Matrix(int height,
           int width) : height(height),
                        width(width),
                        changed(std::atomic<bool>(false)),
                        matrix(std::vector<std::vector<int>>(height, std::vector<int>(width))) {};

    explicit Matrix(std::vector<std::vector<int>> *matrix,
                    int squared_matrix_size) : matrix(*matrix),
                                               height(squared_matrix_size),
                                               changed(std::atomic<bool>(false)),
                                               width(squared_matrix_size) {}

    void set_width(int width);

    void set_height(int height);

    void mark_changed();

    bool is_changed() const;

    bool is_not_changed() const;

    int get_width() const;

    int get_height() const;

    int get(int height, int width) const;

    void set(int height, int width, int value);

    void set(int index, const std::vector<int> &row);

};


#endif //UNTITLED_MATRIX_H
