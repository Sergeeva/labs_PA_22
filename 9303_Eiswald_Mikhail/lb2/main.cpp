#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

#include "args/args.h"
#include "matrix/matrix.h"
#include "sync-buffer/syncbuffer.h"

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
    srand(time(nullptr));
    int params[] = {15, 5, 5, 10, (numeric_limits<int>::max() - 1)/2};
    parse_args(argc, argv, params, 5,
               "Usage: %s <iterations> <rows> <columns> <threads> <max element module>\n");
    printf("Generating and adding %dx%d matrices with elements up to %d using %d threads %d times\n",
           params[1], params[2], params[4], params[3], params[0]);
    const int iterations = params[0];
    const int rows_num = params[1];
    const int cols_num = params[2];
    const int threads_count = params[3];
    const int max_elem = params[4];
    SyncBuffer arg1_buffer(10);
    SyncBuffer arg2_buffer(10);
    SyncBuffer result_buffer(10);

    thread generator = thread([&](){
        for(int i = 0; i < iterations; ++i) {
            arg1_buffer.push(Matrix(rows_num, cols_num, max_elem));
            arg2_buffer.push(Matrix(rows_num, cols_num, max_elem));
        }
    });

    thread summator = thread([&](){
        vector<int> start_points = vector<int>(threads_count);
        vector<int> end_points = vector<int>(threads_count);

        for(int i = 0; i < threads_count; ++i) {
            end_points[i] = (i+1)*rows_num*cols_num/threads_count;
        }
        start_points[0] = 0;
        for(int i = 1; i < threads_count; ++i) {
            start_points[i] = end_points[i-1];
        }

        for(int i = 0; i < iterations; ++i) {
            vector<thread> sum_threads = vector<thread>(threads_count);
            Matrix arg1 = arg1_buffer.pop();
            Matrix arg2 = arg2_buffer.pop();
            // cout << arg1 << arg2 << "\n";
            Matrix* result = new Matrix(rows_num, cols_num);
            for(int i = 0; i < threads_count; ++i) {
                sum_threads[i] = thread(sum_elements, i, ref(arg1), ref(arg2),
                                        ref(start_points), ref(end_points),
                                        ref(*result));
            }
            for(auto& thr: sum_threads) {
                thr.join();
            }
            result_buffer.push(*result);
        }
    });

    thread printer = thread([&](){
        ofstream out;
        out.open("output.txt", ios_base::app);
        for(int i = 0; i < iterations; ++i) {
            Matrix matrix = result_buffer.pop();
            out << matrix;
        }
        out.close();
    });

    generator.join();
    summator.join();
    printer.join();
    cout << "Done. Check output.txt for results.\n";
    return 0;
}
