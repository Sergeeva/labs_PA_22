#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <tuple>
#include <fstream>
#include "./Matrix.cpp"

using namespace std;

namespace MATRIX
{
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
    void write(Matrix<T> &mat, string filename)
    {
        ofstream file(filename);
        file << mat;
        file.close();
    }

    void sumVectors(vector<int> &vec_1, vector<int> &vec_2, vector<int> &vec_res)
    {
        int count = vec_1.size();

        for (size_t i = 0; i < count; i++)
        {
            vec_res.at(i) = vec_1.at(i) + vec_2.at(i);
        }
    };
}




