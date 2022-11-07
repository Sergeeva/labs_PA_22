#include <iostream>
#include <fstream>
#include <thread>
#include <utility>
#include <mutex>
#include <chrono>

#include "args/args.h"
#include "matrix/matrix.h"
#include "queue/queue.h"

using namespace std;

void generate_matrices(int rows_num, int cols_num, int max_module,
                       Queue<pair<Matrix&, Matrix&>>& queue, mutex& mtx) {
    pair<Matrix&, Matrix&>* arguments = new pair<Matrix&, Matrix&>( // &?
                *new Matrix(rows_num, cols_num, max_module),
                *new Matrix(rows_num, cols_num, max_module)
    );
    queue.enq(arguments);
    if(rows_num <= 5 && cols_num <= 5) {
        unique_lock<mutex> lock = unique_lock<mutex>(mtx);
        cout << arguments->first << arguments->second << "\n";
    }
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

void add_matrices(Queue<pair<Matrix&, Matrix&>>& input_queue,
                  Queue<Matrix>& output_queue, int thread_count) {
    auto args = input_queue.deq();
    int rows_num = args->first.get_rows_num();
    int cols_num = args->first.get_cols_num();
    Matrix* matrix = new Matrix(rows_num, cols_num);
    //*matrix = args->first + args->second;
    //====
    vector<int> start_points = vector<int>(thread_count);
    vector<int> end_points = vector<int>(thread_count);

    for(int i = 0; i < thread_count; ++i) {
        end_points[i] = (i+1)*rows_num*cols_num/thread_count;
    }
    start_points[0] = 0;
    for(int i = 1; i < thread_count; ++i) {
        start_points[i] = end_points[i-1];
    }

    vector<thread> sum_threads = vector<thread>(thread_count);
    for(int i = 0; i < thread_count; ++i) {
        sum_threads[i] = thread(sum_elements, i,
                                ref(args->first), ref(args->second),
                                ref(start_points), ref(end_points),
                                ref(*matrix));
    }
    for(auto& thr: sum_threads) {
        thr.join();
    }
    //====
    output_queue.enq(matrix);
}

void print_matrices(Queue<Matrix>& queue, mutex& mtx, ofstream& outfile) {
    Matrix matrix = *queue.deq();
    unique_lock<mutex> lock = unique_lock<mutex>(mtx);
    outfile << matrix;
}

int main(int argc, char** argv) {
    srand(time(nullptr));
    int params[] = {10, 5, 5, 10, (numeric_limits<int>::max() - 1)/2};
    parse_args(argc, argv, params, 5,
               "Usage: %s <number of queue users of each kind> <rows> <columns> <number of summing threads> <max element module>\n");
    printf("Generating and adding %dx%d matrices with elements up to %d using %d threads %d times\n",
           params[1], params[2], params[4], params[3], params[0]);
    const int iterations = params[0];
    const int rows_num = params[1];
    const int cols_num = params[2];
    const int threads_count = params[3];
    const int max_elem = params[4];

    Queue<pair<Matrix&, Matrix&>> args_queue{10};
    Queue<Matrix> ans_queue{10};
    mutex cout_mutex, outfile_mutex;
    ofstream outfile;
    outfile.open("output.txt");
    outfile.close();
    outfile.open("output.txt", ios_base::app);

    thread generator_threads[iterations];
    thread sum_threads[iterations];
    thread print_threads[iterations];

    auto start = chrono::high_resolution_clock::now();
    for(int i = 0; i < iterations; ++i) {
        generator_threads[i] = thread(generate_matrices, rows_num, cols_num, max_elem,
                                      ref(args_queue), ref(cout_mutex));
        sum_threads[i] = thread(add_matrices, ref(args_queue), ref(ans_queue),
                                threads_count);
        print_threads[i] = thread(print_matrices, ref(ans_queue),
                                  ref(outfile_mutex), ref(outfile));
    }

    for(int i = 0; i < iterations; ++i) {
        generator_threads[i].join();
        sum_threads[i].join();
        print_threads[i].join();
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Elapsed time: " << duration.count() << " ms\n";

    outfile.close();
}
