#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

#include "args/args.h"
#include "matrix/matrix.h"

void generate_matrices(void* mem1, void* mem2, int* params) {
    new (mem1) Matrix(params[0], params[1], params[2]);
    new (mem2) Matrix(params[0], params[1], params[2]);
}

void sum_elements(int thread_index, const Matrix& m1, const Matrix& m2,
                  const vector<int>& start_indices, const vector<int>& end_indices,
                  Matrix& result) {
    const int cols_num = m1.get_cols_num();
    for(int i = start_indices[thread_index]; i < end_indices[thread_index]; ++i) {
        result.set(i/cols_num, i%cols_num,
                   m1.get(i/cols_num, i%cols_num) + m2.get(i/cols_num, i%cols_num));
    }
}

int main(int argc, char *argv[]) {
    int params[] = {5, 5, (numeric_limits<int>::max() - 1)/2, 10};
    parse_args(argc, argv, params, 4);
    printf("Generating and adding %dx%d matrices with elements up to %d using %d threads\n",
           params[0], params[1], params[2], params[3]);
    void* matrix1_buf = (void*) new char[sizeof(Matrix)];
    void* matrix2_buf = (void*) new char[sizeof(Matrix)];
    thread thread_gen = thread(generate_matrices, matrix1_buf, matrix2_buf, params);
    thread_gen.join();
    Matrix& matrix1 = *(Matrix*) matrix1_buf;
    Matrix& matrix2 = *(Matrix*) matrix2_buf;
    if(params[0] <= 10 && params[1] <= 10) {
        cout << matrix1 << "\n" << matrix2;
    }
    const int threads_count = params[3];
    vector<int> start_points = vector<int>(threads_count);
    vector<int> end_points = vector<int>(threads_count);
    for(int i = 0; i < threads_count; ++i) {
        end_points[i] = (i+1)*params[0]*params[1]/threads_count;
    }
    start_points[0] = 0;
    for(int i = 1; i < threads_count; ++i) {
        start_points[i] = end_points[i-1];
    }

    Matrix* matrix_result = new Matrix(params[0], params[1]);
    vector<thread> sum_threads = vector<thread>(threads_count);
    auto start = chrono::high_resolution_clock::now();
    for(int i = 0; i < threads_count; ++i) {
        sum_threads[i] = thread(sum_elements, i, ref(matrix1), ref(matrix2),
                                ref(start_points), ref(end_points),
                                ref(*matrix_result));
    }
    for(auto& thr: sum_threads) {
        thr.join();
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Elapsed time: " << duration.count() << " ms\n";

    thread print_thread = thread([](const Matrix& matrix){
        ofstream out;
        out.open("output.txt");
        out << matrix;
        out.close();
    }, *matrix_result);
    print_thread.join();

    cout << "Done. Check output.txt for results.\n";

    matrix1.~Matrix();
    matrix2.~Matrix();
    delete matrix_result;
    delete [] (char*) matrix1_buf;
    delete [] (char*) matrix2_buf;
    return 0;
}
