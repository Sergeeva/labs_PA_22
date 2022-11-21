#ifndef UNTITLED_MATRIX_H
#define UNTITLED_MATRIX_H


#include <vector>
#include <ostream>

class Matrix {

private:
    std::vector<std::vector<int>> matrix;
    int width = 1;
    int height = 1;

public:
    Matrix() = default;

    Matrix(int height, int width) : matrix(std::vector<std::vector<int>>(height,
                                                                         std::vector<int>(width))),
                                    height(height),
                                    width(width) {};


    void set_width(int width);

    void set_height(int height);

    int get_width() const;

    int get_height() const;

    int get(int height, int width) const;

    void set(int height, int width, int value);

    Matrix operator+(const Matrix &matrix) const;

};


#endif //UNTITLED_MATRIX_H
