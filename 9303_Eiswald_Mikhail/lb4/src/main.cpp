#include <iostream>
#include <fstream>
#include <thread>
#include <utility>
#include <vector>

#include "args/args.h"
#include "matrix/matrix.h"

using namespace std;

//#define DEBUG

void index_to_row_col(int index, int& row_index, int& col_index, int row_len) {
    row_index = index / row_len;
    col_index = index % row_len;
}

int row_col_to_index(int row_index, int col_index, int row_len) {
    return row_index*row_len + col_index;
}

void generate_matrices(void* mem1, void* mem2, int i, int j, int k, int max_abs) {
    new (mem1) Matrix(i, j, max_abs);
    new (mem2) Matrix(j, k, max_abs);
}

void multiply_matrices(const Matrix& matrix1, const Matrix& matrix2, Matrix& result,
                          int start_index, int end_index) {
    for(int index = start_index; index < end_index; ++index) {
        int i, k;
        index_to_row_col(index, i, k, result.get_cols_num());
        for(int j = 0; j < matrix1.get_cols_num(); ++j) {
            result.set(i, k, result.get(i, k) + matrix1.get(i, j)*matrix2.get(j, k));
        }
    }
}

void sum_matrices(const Matrix& matrix1, const Matrix& matrix2, Matrix& result,
                  int start_index, int end_index) {
    int i, j;
    for(int index = start_index; index < end_index; ++index) {
        index_to_row_col(index, i, j, matrix1.get_cols_num());
        result.set(i, j, matrix1.get(i, j) + matrix2.get(i, j));
    }
}

void naive_multiplication(int i, int j, int k, int max_element_abs, int threads_num) {
    cout << "Naive multiplication:\n";
    void* matrix1_buf = (void*) new char[sizeof(Matrix)];
    void* matrix2_buf = (void*) new char[sizeof(Matrix)];
    thread thread_gen = thread(generate_matrices, matrix1_buf, matrix2_buf, i, j, k, max_element_abs);
    thread_gen.join();
    Matrix& matrix1 = *(Matrix*)matrix1_buf;
    Matrix& matrix2 = *(Matrix*)matrix2_buf;
    if(i <= 8 && j <= 8 && k <= 8) {
        cout << matrix1 << matrix2 << "\n";
    }
    Matrix result(i, k);

    vector<pair<int, int>> indices(threads_num);
    for(int index = 0; index < threads_num; ++index) {
        indices[index].first = (i*k/(double)threads_num)*index;
    }
    for(int index = 0; index < threads_num - 1; ++index) {
        indices[index].second = indices[index+1].first;
    }
    indices[threads_num-1].second = i*k;

    #ifdef DEBUG
        for(auto& interval: indices) {
            cout << "[" << interval.first << ";" << interval.second << ") ";
        }
        cout << endl;
    #endif

    auto start = chrono::high_resolution_clock::now();
    vector<thread> threads(threads_num);
    for(int index = 0; index < threads_num; ++index) {
        threads[index] = thread(multiply_matrices, ref(matrix1), ref(matrix2), ref(result),
                                indices[index].first, indices[index].second);
    }
    for(auto& thr : threads) {
        thr.join();
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    if(i <= 8 && k <= 8) cout << result;
    cout << "Elapsed time: " << duration.count() << " ms\n";

    thread print_thread = thread([](const Matrix& matrix){
            ofstream out;
            out.open("output-naive.txt");
            out << matrix;
            out.close();
    }, result);
    print_thread.join();

    cout << "Done. Check output-naive.txt for results.\n";

    matrix1.~Matrix();
    matrix2.~Matrix();
    delete [] (char*) matrix1_buf;
    delete [] (char*) matrix2_buf;
}

void mult(const Matrix& matrix1, const Matrix& matrix2, Matrix& result) {
    if(matrix1.get_cols_num() == 1) {
        result.set(0, 0, matrix1.get(0, 0)*matrix2.get(0, 0));
        return;
    }
    int side = matrix1.get_rows_num();
    Matrix a11 = matrix1.slice(0, side/2, 0, side/2);
    Matrix a12 = matrix1.slice(0, side/2, side/2, side);
    Matrix a21 = matrix1.slice(side/2, side, 0, side/2);
    Matrix a22 = matrix1.slice(side/2, side, side/2, side);
    Matrix b11 = matrix2.slice(0, side/2, 0, side/2);
    Matrix b12 = matrix2.slice(0, side/2, side/2, side);
    Matrix b21 = matrix2.slice(side/2, side, 0, side/2);
    Matrix b22 = matrix2.slice(side/2, side, side/2, side);
    Matrix d(side/2, side/2);
    Matrix d1(side/2, side/2);
    Matrix d2(side/2, side/2);
    Matrix h1(side/2, side/2);
    Matrix h2(side/2, side/2);
    Matrix v1(side/2, side/2);
    Matrix v2(side/2, side/2);

    Matrix a11_plus_a22(side, side);
    Matrix b11_plus_b22(side, side);
    Matrix a12_minus_a22(side, side);
    Matrix b21_plus_b22(side, side);
    Matrix a21_minus_a11(side, side);
    Matrix b11_plus_b12(side, side);
    Matrix a11_plus_a12(side, side);
    Matrix a21_plus_a22(side, side);
    Matrix b21_minus_b11(side, side);
    Matrix b12_minus_b22(side, side);

    thread thr_a11_plus_a22 = thread([&](){
        a11_plus_a22 = a11 + a22;
    });
    thread thr_b11_plus_b22 = thread([&](){
        b11_plus_b22 = b11 + b22;
    });
    thread thr_a12_minus_a22 = thread([&](){
        a12_minus_a22 = a12 - a22;
    });
    thread thr_b21_plus_b22 = thread([&](){
        b21_plus_b22 = b21 + b22;
    });
    thread thr_a21_minus_a11 = thread([&](){
        a21_minus_a11 = a21 - a11;
    });
    thread thr_b11_plus_b12 = thread([&](){
        b11_plus_b12 = b11 + b12;
    });
    thread thr_a11_plus_a12 = thread([&](){
        a11_plus_a12 = a11 + a12;
    });
    thread thr_a21_plus_a22 = thread([&](){
        a21_plus_a22 = a21 + a22;
    });
    thread thr_b21_minus_b11 = thread([&](){
        b21_minus_b11 = b21 - b11;
    });
    thread thr_b12_minus_b22 = thread([&](){
        b12_minus_b22 = b12 + b22;
    });

    thr_a11_plus_a12.join();
    thr_a11_plus_a22.join();
    thr_a12_minus_a22.join();
    thr_a21_minus_a11.join();
    thr_a21_plus_a22.join();
    thr_b11_plus_b12.join();
    thr_b11_plus_b22.join();
    thr_b12_minus_b22.join();
    thr_b21_minus_b11.join();
    thr_b21_plus_b22.join();

    mult(a11_plus_a22, b11_plus_b22, d);
    mult(a12_minus_a22, b21_plus_b22, d1);
    mult(a21_minus_a11, b11_plus_b12, d2);
    mult(a11_plus_a12, b22, h1);
    mult(a21_plus_a22, b11, h2);
    mult(a22, b21_minus_b11, v1);
    mult(a11, b12_minus_b22, v2);

    Matrix res11 = d + d1 + v1 - h1;
    Matrix res12 = v2 + h1;
    Matrix res21 = v1 + h2;
    Matrix res22 = d + d2 + v2 - h2;
#ifdef DEBUG
    cout << "\n" << res11 << res12 << res21 << res22;
#endif
    for(int i = 0; i < side/2; ++i) {
        for(int k = 0; k < side/2; ++k) {
            result.set(i, k, res11.get(i, k));
        }
    }
    for(int i = 0; i < side/2; ++i) {
        for(int k = side/2; k < side; ++k) {
            result.set(i, k, res12.get(i, k - side/2));
        }
    }
    for(int i = side/2; i < side; ++i) {
        for(int k = 0; k < side/2; ++k) {
            result.set(i, k, res21.get(i - side/2, k));
        }
    }
    for(int i = side/2; i < side; ++i) {
        for(int k = side/2; k < side; ++k) {
            result.set(i, k, res22.get(i - side/2, k - side/2));
        }
    }
#ifdef DEBUG
    cout << "\n result =\n" << result << "\n";
#endif
}

void mult_serial(const Matrix& matrix1, const Matrix& matrix2, Matrix& result) {
    if(matrix1.get_cols_num() == 1) {
        result.set(0, 0, matrix1.get(0, 0)*matrix2.get(0, 0));
        return;
    }
    int side = matrix1.get_rows_num();
#ifdef DEBUG
    cout << "side=" << side << "\n";
    cout << matrix1 << matrix2 << "\n";
#endif
    Matrix a11 = matrix1.slice(0, side/2, 0, side/2);
    Matrix a12 = matrix1.slice(0, side/2, side/2, side);
    Matrix a21 = matrix1.slice(side/2, side, 0, side/2);
    Matrix a22 = matrix1.slice(side/2, side, side/2, side);
    Matrix b11 = matrix2.slice(0, side/2, 0, side/2);
    Matrix b12 = matrix2.slice(0, side/2, side/2, side);
    Matrix b21 = matrix2.slice(side/2, side, 0, side/2);
    Matrix b22 = matrix2.slice(side/2, side, side/2, side);
    Matrix d(side/2, side/2);
    Matrix d1(side/2, side/2);
    Matrix d2(side/2, side/2);
    Matrix h1(side/2, side/2);
    Matrix h2(side/2, side/2);
    Matrix v1(side/2, side/2);
    Matrix v2(side/2, side/2);
    mult(a11 + a22, b11 + b22, d);
    mult(a12 - a22, b21 + b22, d1);
    mult(a21 - a11, b11 + b12, d2);
    mult(a11 + a12, b22, h1);
    mult(a21 + a22, b11, h2);
    mult(a22, b21 - b11, v1);
    mult(a11, b12 - b22, v2);

#ifdef DEBUG
    cout << "\nd = \n" << d << "d1 = \n" << d1 << "d2 = \n" << d2 << "h1 = \n" << h1 <<
            "h2 = \n" << h2 << "v1 = \n" << v1 << "v2 = \n" << v2;
#endif
    Matrix res11 = d + d1 + v1 - h1;
    Matrix res12 = v2 + h1;
    Matrix res21 = v1 + h2;
    Matrix res22 = d + d2 + v2 - h2;
#ifdef DEBUG
    cout << "\n" << res11 << res12 << res21 << res22;
#endif
    for(int i = 0; i < side/2; ++i) {
        for(int k = 0; k < side/2; ++k) {
            result.set(i, k, res11.get(i, k));
        }
    }
    for(int i = 0; i < side/2; ++i) {
        for(int k = side/2; k < side; ++k) {
            result.set(i, k, res12.get(i, k - side/2));
        }
    }
    for(int i = side/2; i < side; ++i) {
        for(int k = 0; k < side/2; ++k) {
            result.set(i, k, res21.get(i - side/2, k));
        }
    }
    for(int i = side/2; i < side; ++i) {
        for(int k = side/2; k < side; ++k) {
            result.set(i, k, res22.get(i - side/2, k - side/2));
        }
    }
#ifdef DEBUG
    cout << "\n result =\n" << result << "\n";
#endif
}

void mult_var_thr(const Matrix& matrix1, const Matrix& matrix2, Matrix& result) {
    if(matrix1.get_cols_num() == 1) {
        result.set(0, 0, matrix1.get(0, 0)*matrix2.get(0, 0));
        return;
    }
    int side = matrix1.get_rows_num();
#ifdef DEBUG
    cout << "side=" << side << "\n";
    cout << matrix1 << matrix2 << "\n";
#endif
    Matrix a11 = matrix1.slice(0, side/2, 0, side/2);
    Matrix a12 = matrix1.slice(0, side/2, side/2, side);
    Matrix a21 = matrix1.slice(side/2, side, 0, side/2);
    Matrix a22 = matrix1.slice(side/2, side, side/2, side);
    Matrix b11 = matrix2.slice(0, side/2, 0, side/2);
    Matrix b12 = matrix2.slice(0, side/2, side/2, side);
    Matrix b21 = matrix2.slice(side/2, side, 0, side/2);
    Matrix b22 = matrix2.slice(side/2, side, side/2, side);
    // cout << a11 << a12 << a21 << a22;
    Matrix d(side/2, side/2);
    Matrix d1(side/2, side/2);
    Matrix d2(side/2, side/2);
    Matrix h1(side/2, side/2);
    Matrix h2(side/2, side/2);
    Matrix v1(side/2, side/2);
    Matrix v2(side/2, side/2);
    /*mult(a11 + a22, b11 + b22, d);
    mult(a12 - a22, b21 + b22, d1);
    mult(a21 - a11, b11 + b12, d2);
    mult(a11 + a12, b22, h1);
    mult(a21 + a22, b11, h2);
    mult(a22, b21 - b11, v1);
    mult(a11, b12 - b22, v2);*/
    Matrix a11_plus_a22 = a11 + a22;
    Matrix b11_plus_b22 = b11 + b22;
    thread thread_d = thread(mult_var_thr, ref(a11_plus_a22), ref(b11_plus_b22), ref(d));
    Matrix a12_minus_a22 = a12 - a22;
    Matrix b21_plus_b22 = b21 + b22;
    thread thread_d1 = thread(mult_var_thr, ref(a12_minus_a22), ref(b21_plus_b22), ref(d1));
    Matrix a21_minus_a11 = a21 - a11;
    Matrix b11_plus_b12 = b11 + b12;
    thread thread_d2 = thread(mult_var_thr, ref(a21_minus_a11), ref(b11_plus_b12), ref(d2));
    Matrix a11_plus_a12 = a11 + a12;
    thread thread_h1 = thread(mult_var_thr, ref(a11_plus_a12), ref(b22), ref(h1));
    Matrix a21_plus_a22 = a21 + a22;
    thread thread_h2 = thread(mult_var_thr, ref(a21_plus_a22), ref(b11), ref(h2));
    Matrix b21_minus_b11 = b21 - b11;
    thread thread_v1 = thread(mult_var_thr, ref(a22), ref(b21_minus_b11), ref(v1));
    Matrix b12_minus_b22 = b12 - b22;
    thread thread_v2 = thread(mult_var_thr, ref(a11), ref(b12_minus_b22), ref(v2));

    thread_d.join();
    thread_d1.join();
    thread_d2.join();
    thread_h1.join();
    thread_h2.join();
    thread_v1.join();
    thread_v2.join();
#ifdef DEBUG
    cout << "\nd = \n" << d << "d1 = \n" << d1 << "d2 = \n" << d2 << "h1 = \n" << h1 <<
            "h2 = \n" << h2 << "v1 = \n" << v1 << "v2 = \n" << v2;
#endif
    Matrix res11 = d + d1 + v1 - h1;
    Matrix res12 = v2 + h1;
    Matrix res21 = v1 + h2;
    Matrix res22 = d + d2 + v2 - h2;
#ifdef DEBUG
    cout << "\n" << res11 << res12 << res21 << res22;
#endif
    for(int i = 0; i < side/2; ++i) {
        for(int k = 0; k < side/2; ++k) {
            result.set(i, k, res11.get(i, k));
        }
    }
    for(int i = 0; i < side/2; ++i) {
        for(int k = side/2; k < side; ++k) {
            result.set(i, k, res12.get(i, k - side/2));
        }
    }
    for(int i = side/2; i < side; ++i) {
        for(int k = 0; k < side/2; ++k) {
            result.set(i, k, res21.get(i - side/2, k));
        }
    }
    for(int i = side/2; i < side; ++i) {
        for(int k = side/2; k < side; ++k) {
            result.set(i, k, res22.get(i - side/2, k - side/2));
        }
    }
#ifdef DEBUG
    cout << "\n result =\n" << result << "\n";
#endif
}

void strassen_multiplication(int i, int j, int k, int max_element_abs) {
    cout << "Strassen multiplication:\n";
    void* matrix1_buf = (void*) new char[sizeof(Matrix)];
    void* matrix2_buf = (void*) new char[sizeof(Matrix)];
    thread thread_gen = thread(generate_matrices, matrix1_buf, matrix2_buf, i, j, k, max_element_abs);
    thread_gen.join();
    Matrix& matrix1_orig = *(Matrix*)matrix1_buf;
    Matrix& matrix2_orig = *(Matrix*)matrix2_buf;
    if(i <= 8 && j <= 8 && k <= 8) {
        cout << matrix1_orig << matrix2_orig << "\n";
    }
    int side = max( pow(2, ceil(log2(i))),
                    max(
                        pow(2, ceil(log2(j))),
                        pow(2, ceil(log2(k)))
                    ));
    Matrix matrix1(side, side);
    Matrix matrix2(side, side);
    for(int __i = 0; __i < i; ++__i) {
        for(int __j = 0; __j < j; ++__j) {
            matrix1.set(__i, __j, matrix1_orig.get(__i, __j));
        }
    }
    for(int __j = 0; __j < j; ++__j) {
        for(int __k = 0; __k < k; ++__k) {
            matrix2.set(__j, __k, matrix2_orig.get(__j, __k));
        }
    }
#ifdef DEBUG
    if(side <= 8) cout << matrix1 << matrix2;
#endif
    Matrix result(side, side);

    auto start = chrono::high_resolution_clock::now();
    // mult_var_thr(matrix1, matrix2, result);
    // mult(matrix1, matrix2, result);
    mult_serial(matrix1, matrix2, result);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    result = result.slice(0, i, 0, k);
    if(i <= 8 && k <= 8) cout << result;
    cout << "Elapsed time: " << duration.count() << " ms\n";

    thread print_thread = thread([](const Matrix& matrix){
            ofstream out;
            out.open("output-strassen.txt");
            out << matrix;
            out.close();
    }, result);
    print_thread.join();

    cout << "Done. Check output-strassen.txt for results.\n";
    delete [] (char*) matrix1_buf;
    delete [] (char*) matrix2_buf;
}

int main(int c, char** v) {
    int params[] = {2, 3, 2, 10, 10};
    parse_args(c, v, params, 5);
    int i = params[0];
    int j = params[1];
    int k = params[2];
    int max_element_abs = params[3];
    int threads_num = params[4];
    printf("Multiplying generated %dx%d and %dx%d matrices with elements up to %d using %d threads\n",
               i, j, j, k, max_element_abs, threads_num);
    naive_multiplication(i, j, k, max_element_abs, threads_num);
    strassen_multiplication(i, j, k, max_element_abs);
    return 0;
}
