#ifndef MATRIX_H
#define MATRIX_H

#include "definition.h"

template<typename T>
class Matrix {
public:
    Matrix() = default;
    Matrix(int);
    ~Matrix() = default;
    int size() const;
    void fill();
    std::vector<T> &operator[] (int);
    Matrix<T> operator+ (Matrix<T> &);
    Matrix<T> operator- (Matrix<T> &);
    Matrix<T> operator* (Matrix<T> &);
    std::tuple<Matrix<T>, Matrix<T>, Matrix<T>, Matrix<T>> split();
    static Matrix<T> join(Matrix<T>, Matrix<T>, Matrix<T>, Matrix<T>);

private:
    std::pair<int, int> getFragment(int) const;

    std::vector<std::vector<T>> _data;
    int _size;
    int _threadAmount;

};

template<typename T>
Matrix<T>::Matrix(int size) : _size(size) {
    _data.resize(size);
    for (int i = 0; i < size; i++) {
        _data[i].resize(size);
    }
    _threadAmount = MAX_THREAD_AMOUNT < this->size() ? MAX_THREAD_AMOUNT : this->size();
}

template<typename T>
std::vector<T> &Matrix<T>::operator[](int index) {
    return _data[index];
}

template<typename T>
int Matrix<T>::size() const {
    return _size;
}

template<typename T>
void Matrix<T>::fill() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, 99);

    int size = this->size();
    std::vector<std::thread> threads(_threadAmount);
    for (int index = 0; index < _threadAmount; index++) {
        auto [start, end] = getFragment(index);
        threads[index] = std::thread([&] (int _start, int _end) -> void {
            for (int i = _start; i < _end; i++) {
                for (int j = 0; j < size; j++) {
                    (*this)[i][j] = distribution(gen);
                }
            }
        }, start, end);
    }

    for (int index = 0; index < _threadAmount; index++) {
        threads[index].join();
    }
}

template<typename T>
Matrix<T> Matrix<T>::operator+(Matrix<T> &other) {
    if (this->size() != other.size()) {
        throw std::runtime_error("Matrix error: different sizes of matrices");
    }

    Matrix resultMatrix(this->size());
    for (int i = 0; i < this->size(); i++) {
        for (int j = 0; j < (*this)[i].size(); j++) {
            resultMatrix[i][j] = this->_data[i][j] + other._data[i][j];
        }
    }
    return resultMatrix;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(Matrix<T> &other) {
    if (this->size() != other.size()) {
        throw std::runtime_error("Matrix error: different sizes of matrices");
    }

    Matrix resultMatrix(this->size());
    for (int i = 0; i < this->size(); i++) {
        for (int j = 0; j < (*this)[i].size(); j++) {
            resultMatrix[i][j] = this->_data[i][j] - other._data[i][j];
        }
    }
    return resultMatrix;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(Matrix<T> &other) {
    if (this->size() != other.size()) {
        throw std::runtime_error("Matrix error: different sizes of matrices");
    }

    int size = this->size();
    Matrix resultMatrix(size);
    std::vector<std::thread> threads(_threadAmount);
    for (int index = 0; index < _threadAmount; index++) {
        auto [start, end] = getFragment(index);
        threads[index] = std::thread([&] (int _start, int _end) -> void {
            for (int offset = _start; offset < _end; offset++) {
                for (int i = 0; i < size; i++) {
                    int k = (size + i - offset) % size;
                    resultMatrix[i][k] = 0;
                    for (int j = 0; j < size; j++) {
                        resultMatrix[i][k] += (*this)[i][j] * other[j][k];
                    }
                }
            }
        }, start, end);
    }

    for (int index = 0; index < _threadAmount; index++) {
        threads[index].join();
    }
    return resultMatrix;
}

template<typename T>
std::tuple<Matrix<T>, Matrix<T>, Matrix<T>, Matrix<T>> Matrix<T>::split() {
    if (this->size() % 2) {
        throw std::runtime_error("Matrix error: matrix size is odd");
    }

    int size = this->size();
    std::tuple<Matrix<T>, Matrix<T>, Matrix<T>, Matrix<T>> result = std::make_tuple(
        Matrix<T>(size / 2),
        Matrix<T>(size / 2),
        Matrix<T>(size / 2),
        Matrix<T>(size / 2)
    );
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i < size / 2) {
                if (j < size / 2) {
                    std::get<0>(result)[i][j] = (*this)[i][j];
                } else {
                    std::get<1>(result)[i][j - size / 2] = (*this)[i][j];
                }
            } else {
                if (j < size / 2) {
                    std::get<2>(result)[i - size / 2][j] = (*this)[i][j];
                } else {
                    std::get<3>(result)[i - size / 2][j - size / 2] = (*this)[i][j];
                }
            }
        }
    }
    return result;
}

template<typename T>
Matrix<T> Matrix<T>::join(Matrix<T> upLeft, Matrix<T> upRight, Matrix<T> downLeft, Matrix<T> downRight) {
    int size = upLeft.size() * 2;
    Matrix<T> result = Matrix<T>(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i < size / 2) {
                if (j < size / 2) {
                    result[i][j] = upLeft[i][j];
                } else {
                    result[i][j] = upRight[i][j - size / 2];
                }
            } else {
                if (j < size / 2) {
                    result[i][j] = downLeft[i - size / 2][j];
                } else {
                    result[i][j] = downRight[i - size / 2][j - size / 2];
                }
            }
        }
    }
    return result;
}

template<typename T>
std::pair<int, int> Matrix<T>::getFragment(int index) const {
    int size = this->size();
    int start, end;
    if (index < size % _threadAmount) {
        start = (size / _threadAmount + 1) * index;
        end = start + (size / _threadAmount + 1);
    } else {
        start = (size / _threadAmount + 1) * (size % _threadAmount) + (size / _threadAmount) * (index - size % _threadAmount);
        end = start + (size / _threadAmount);
    }
    return std::make_pair(start, end);
}

#endif //MATRIX_H
