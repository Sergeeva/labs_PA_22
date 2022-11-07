#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "./Matrix.cpp"
#include <thread>


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
                mat.setValue(j, i, rand() % 10);
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

    void multiplyByRow(Matrix<int> &A,
                  Matrix<int> &B,
                  Matrix<int> &C, vector<int> &queue)
    {

        cout << "thread[" << this_thread::get_id << "] --- start working\n";

        auto a_size = A.size();
        auto b_size = B.size();

        while (!queue.empty())
        {

            int row = queue.back();
            queue.pop_back();

            for (int col = 0; col < C.size().cols; col++)
            {
                for (int i = 0; i < a_size.cols; i++)
                {
                    C.setValue(col, row, C.getValue(col, row) + A.getValue(i, row) * B.getValue(col, i));
                }
            }
        }
        cout << "thread[" << this_thread::get_id << "] --- stop working\n";
    }

    Matrix<int> parallelSimpleMultiply(Matrix<int> &A,
                                        Matrix<int> &B)
    {
        vector<thread> threads;
        Matrix<int> C(A.size().rows, A.size().cols);

        vector<int> queue;

        for (int row = 0; row < A.size().rows; row++)
        {
            queue.push_back(A.size().rows - 1 - row);
        }

        for (int i = 0; i < 8; i++)
        {
            threads.push_back(thread(multiplyByRow, ref(A), ref(B), ref(C), ref(queue)));
        }

        for (auto &_thread : threads)
        {
            _thread.join();
        }

        return C;
    }

}
