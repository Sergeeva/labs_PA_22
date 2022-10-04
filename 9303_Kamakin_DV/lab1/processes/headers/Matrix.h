#ifndef UNTITLED_MATRIX_H
#define UNTITLED_MATRIX_H


#include <vector>
#include <ostream>

class Matrix {

private:
    std::vector<std::vector<int>> matrix;
    int width;
    int height;

public:
    Matrix(const int height, const int width) : matrix(std::vector<std::vector<int>>(height,
                                                                                     std::vector<int>(width))),
                                                width(width),
                                                height(height) {}

    int get_width() const;

    int get_height() const;

    int get(int height, int width) const;

    void set(int height, int width, int value);

    Matrix operator+(const Matrix &matrix) const;

};


#endif //UNTITLED_MATRIX_H
