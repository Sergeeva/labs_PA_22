#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <tuple>

using namespace std;

class MatrixDimension
{

public:
    MatrixDimension()
    {
        this->rows = 0;
        this->cols = 0;
    }

    MatrixDimension(int rows, int cols) : rows(rows), cols(cols)
    {
    }

    int rows;
    int cols;
};

namespace MATRIX
{

    template <typename T>
    class Matrix
    {

        vector<vector<T>> *__matrix;
        MatrixDimension __size;

    public:
        Matrix()
        {
            this->__matrix = new vector<vector<T>>();
            this->__size = MatrixDimension(0, 0);
        }

        Matrix(int rows, int cols)
        {
            this->__matrix = new vector<vector<T>>(rows, vector<T>(cols));
            this->__size = MatrixDimension(rows, cols);
        }

        Matrix(Matrix *Mat)
        {

            int rows = Mat->size().rows;
            int cols = Mat->size().cols;

            this->__matrix = new vector<vector<T>>(rows, vector<T>(cols));
            this->__size = MatrixDimension(rows, cols);

            for (size_t y = 0; y < rows; y++)
            {
                for (size_t x = 0; x < cols; x++)
                {
                    this->setValue(x, y, Mat->getValue(x, y));
                }
            }
        }

        void fixed()
        {
        }

        T getValue(int col, int row)
        {

            return this->__matrix->at(row).at(col);
        }

        void setValue(int col, int row, T value)
        {
            this->__matrix->at(row).at(col) = value;
        }

        void pushRow(vector<T> &row)
        {
            this->__matrix->push_back(row);
            this->__size.rows++;
        }

        vector<T> &getRow(int i)
        {
            return this->__matrix->at(i);
        }

        MatrixDimension size()
        {
            if (this->__size.cols == 0)
            {
                this->__size.cols = this->__matrix->at(0).size();
            }
            return this->__size;
        }

        friend ostream &operator<<(ostream &os, Matrix<T> &mat)
        {

            auto size = mat.size();

            for (size_t y = 0; y < size.rows; y++)
            {
                for (size_t x = 0; x < size.cols; x++)
                {
                    os << mat.getValue(x, y) << " ";
                }
                os << endl;
            }
            return os;
        }

        Matrix<T> operator+(Matrix<T> &matrix)
        {

            int rows = this->size().rows;
            int cols = this->size().cols;

            Matrix<T> res(rows, cols);

            for (int y = 0; y < rows; y++)
            {
                for (int x = 0; x < cols; x++)
                {
                    res.setValue(x, y, this->getValue(x, y) + matrix.getValue(x, y));
                }
            }

            return res;
        }

        Matrix<T> operator-(Matrix<T> &matrix)
        {

            int rows = this->size().rows;
            int cols = this->size().cols;

            Matrix<T> res(rows, cols);

            for (int y = 0; y < rows; y++)
            {
                for (int x = 0; x < cols; x++)
                {
                    res.setValue(x, y, this->getValue(x, y) - matrix.getValue(x, y));
                }
            }

            return res;
        }
    };
}