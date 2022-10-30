#include <string>
#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <tuple>
#include <fstream>

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
    };

    Matrix<int> generate(int m, int n)
    {
        auto mat = Matrix<int>(m, n);

        for (int j = 0; j < m; j++)
        {
            for (int i = 0; i < n; i++)
            {
                mat.setValue(j, i, rand() % 100);
            }
        }
        return mat;
    }

    template <typename T>
    Matrix<T> sum(Matrix<T> &mat_1, Matrix<T> &mat_2)
    {
        if (mat_1.size().first != mat_2.size().first || mat_1.size().second != mat_2.size().second)
        {
            throw invalid_argument("matrix dimensions must match");
        }

        Matrix<T> res(mat_1.size().first, mat_1.size().second);


        pair<int, int> size = res.size();
        for (size_t i = 0; i < size.first; i++)
        {
            for (size_t j = 0; j < size.second; j++)
            {
                res.setValue(i, j, mat_1.getValue(i, j) + mat_2.getValue(i, j));
            }
        }

        return res;
    }

    template <typename T>
    Matrix<T> read(string filename)
    {
        ifstream file;
        string line;

        string separator = " ";
        int separator_len = separator.length();

        vector<T> row;
        Matrix<T> matrix;

        file.open(filename);
        while (getline(file, line))
        {
            size_t pos;
            string num;
            row.clear();

            while ((pos = line.find(separator)) != string::npos)
            {
                num = line.substr(0, pos);
                row.push_back(stoi(num));
                line.erase(0, pos + separator_len);
            }
            matrix.pushRow(row);
        }
        file.close();

        return matrix;
    }

    template <typename T>
    void write(Matrix<T> &mat, string filename)
    {
        ofstream file(filename);
        file << mat;
        file.close();
    }

    class Timer
    {

        clock_t timer;

    public:
        void start(string msg = "")
        {
            this->timer = clock();
            cout << msg;
        }

        void stop(string msg = "")
        {
            clock_t duration = clock() - this->timer;
            cout << msg;
            cout << duration / (double)CLOCKS_PER_SEC << "s" << endl;
        }
    };


    void sumVectors(vector<int> &vec_1, vector<int> &vec_2, vector<int> &vec_res)
    {
        int count = vec_1.size();

        for (size_t i = 0; i < count; i++)
        {
            vec_res.at(i) = vec_1.at(i) + vec_2.at(i);
        }
    };

}

// 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