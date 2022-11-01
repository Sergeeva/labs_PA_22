#include <thread>
using namespace std;

namespace STRASSEN
{

    MATRIX::Matrix<int> simpleMultiply(
        MATRIX::Matrix<int> &A,
        MATRIX::Matrix<int> &B)
    {
        auto a_size = A.size();
        auto b_size = B.size();

        MATRIX::Matrix<int> C(a_size.rows, b_size.cols);

        for (int row = 0; row < a_size.rows; row++)
        {
            for (int col = 0; col < b_size.cols; col++)
            {
                for (int inner = 0; inner < a_size.cols; inner++)
                {
                    C.setValue(col, row, C.getValue(col, row) + A.getValue(inner, row) * B.getValue(col, inner));
                }
            }
        }
        return C;
    }

    MATRIX::Matrix<int> concat(
        MATRIX::Matrix<int> m11,
        MATRIX::Matrix<int> m12,
        MATRIX::Matrix<int> m21,
        MATRIX::Matrix<int> m22)
    {
        int cols = m11.size().cols;
        int rows = m11.size().rows;

        MATRIX::Matrix<int> res(rows * 2, cols * 2);

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                res.setValue(col, row, m11.getValue(col, row));
                res.setValue(col + cols, row, m12.getValue(col, row));
                res.setValue(col, row + rows, m21.getValue(col, row));
                res.setValue(col + cols, row + rows, m22.getValue(col, row));
            }
        }

        return res;
    }

    vector<MATRIX::Matrix<int>> split(
        MATRIX::Matrix<int> &source)
    {

        int rows = source.size().rows;
        int cols = source.size().cols;
        vector<MATRIX::Matrix<int>> matrices;

        MATRIX::Matrix<int> m11(rows / 2, cols / 2), m12(rows / 2, cols / 2), m21(rows / 2, cols / 2), m22(rows / 2, cols / 2);

        for (int row = 0; row < rows / 2; row++)
        {
            for (int col = 0; col < cols / 2; col++)
            {
                m11.setValue(col, row, source.getValue(col, row));
                m12.setValue(col, row, source.getValue(col + cols / 2, row));
                m21.setValue(col, row, source.getValue(col, row + rows / 2));
                m22.setValue(col, row, source.getValue(col + cols / 2, row + rows / 2));
            }
        }

        matrices.push_back(m11);
        matrices.push_back(m12);
        matrices.push_back(m21);
        matrices.push_back(m22);

        return matrices;
    }

    void multiply(MATRIX::Matrix<int> A_mat, MATRIX::Matrix<int> B_mat, MATRIX::Matrix<int> &C, int n)
    {
        if (n > 64)
        {
            auto a = split(A_mat);

            auto b = split(B_mat);

            auto a11 = a.at(0);
            auto a12 = a.at(1);
            auto a21 = a.at(2);
            auto a22 = a.at(3);

            auto b11 = b.at(0);
            auto b12 = b.at(1);
            auto b21 = b.at(2);
            auto b22 = b.at(3);

            MATRIX::Matrix<int> p1, p2, p3, p4, p5, p6, p7;

            n = n >> 1;

            thread p1_thread(multiply, a11 + a22, b11 + b22, ref(p1), n);
            thread p2_thread(multiply, a21 + a22, b11, ref(p2), n);
            thread p3_thread(multiply, a11, b12 - b22, ref(p3), n);
            thread p4_thread(multiply, a22, b21 - b11, ref(p4), n);
            thread p5_thread(multiply, a11 + a12, b22, ref(p5), n);
            thread p6_thread(multiply, a21 - a11, b11 + b12, ref(p6), n);
            thread p7_thread(multiply, a12 - a22, b21 + b22, ref(p7), n);

            p1_thread.join();
            p2_thread.join();
            p3_thread.join();
            p4_thread.join();
            p5_thread.join();
            p6_thread.join();
            p7_thread.join();

            auto c11 = p1 + p4 - p5 + p7;
            auto c12 = p3 + p5;
            auto c21 = p2 + p4;
            auto c22 = p1 - p2 + p3 + p6;

            C = concat(c11, c12, c21, c22);
        }
        else
        {
            C = simpleMultiply(A_mat, B_mat);
        }
    }

}
