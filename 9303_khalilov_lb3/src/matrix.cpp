#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <map>
#include <ctime>
#include <atomic>
#include <thread>
#include <memory>



using namespace std;

namespace MATRIX
{

    template <typename T>
    class Matrix
    {

        vector<vector<T>> *__matrix;
        pair<int, int> __size;

    public:
        Matrix()
        {
            this->__matrix = new vector<vector<T>>();
            this->__size = make_pair(0, 0);
        }
        Matrix(int m, int n)
        {
            this->__matrix = new vector<vector<T>>(m, vector<T>(n));
            this->__size = make_pair(m, n);
        }

        Matrix(Matrix *Mat)
        {

            this->__size = Mat->size();

            this->__matrix = new vector<vector<T>>(this->__size.first, vector<T>(this->__size.second));

            for (size_t i = 0; i < this->__size.first; i++)
            {
                for (size_t j = 0; j < this->__size.second; j++)
                {
                    this->setValue(i, j, Mat->getValue(i, j));
                }
            }
        }

        T getValue(int x, int y)
        {

            return this->__matrix->at(x).at(y);
        }

        void setValue(int x, int y, T value)
        {
            this->__matrix->at(x).at(y) = value;
        }

        void pushRow(vector<T> &row)
        {
            this->__matrix->push_back(row);
            this->__size.first++;
        }

        vector<T> &getRow(int i)
        {
            return this->__matrix->at(i);
        }

        pair<int, int> size()
        {
            if (this->__size.second == 0)
            {
                this->__size.second = this->__matrix->at(0).size();
            }
            return this->__size;
        }

        friend ostream &operator<<(ostream &os, Matrix<T> &mat)
        {

            pair<int, int> size = mat.size();

            for (size_t i = 0; i < size.first; i++)
            {
                for (size_t j = 0; j < size.second; j++)
                {
                    os << mat.getValue(i, j) << " ";
                }
                os << endl;
            }
            return os;
        }

        Matrix<T> operator+(Matrix<T> &matrix)
        {

            int n = this->size().first;
            int m = this->size().second;

            Matrix<T> res(n, m);

            for (int y = 0; y < m; y++)
            {
                for (int x = 0; x < n; x++)
                {
                    res.setValue(x, y, this->getValue(x, y) + matrix.getValue(x, y));
                }
            }

            return res;
        }
    };
}