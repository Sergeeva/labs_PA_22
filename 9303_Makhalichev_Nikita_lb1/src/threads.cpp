#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <chrono>


using namespace std;

void read_matrix(vector<int>* matrix, string filename){
    int input = 0;
    ifstream matrix_file;
    matrix_file.open(filename);
    while (matrix_file >> input)
        matrix->push_back(input);
    matrix_file.close();
}

void read_matrices(vector<int>* first_matrix, vector<int>* second_matrix){
    read_matrix(first_matrix, "first_matrix.txt");
    read_matrix(second_matrix, "second_matrix.txt");
}

void sum_matrices(vector<int>* first_matrix, vector<int>* second_matrix, vector<int>* result_matrix, long long int start_index, long long int size){
    for (long long int i = start_index; i < start_index + size; i++)
    	if (i < first_matrix->size())
            (*result_matrix)[i] = (*first_matrix)[i] + (*second_matrix)[i];
        else break;
}

void write_result(vector<int>* result_matrix, long int rows, long int cols){
    ofstream matrix_file;
    matrix_file.open("result_matrix.txt");
    for (long int i = 0; i < rows; i++){
        for (long int j = 0; j < cols; j++)
            matrix_file << (*result_matrix)[i*cols + j] << ' ';
        matrix_file << endl;
    }
    matrix_file.close();
}

int main(int argc, char *argv[]){
    if (argc < 4){
    	cout << "Not enough arguments" << endl;
    	return 0;
    }
    long int rows = atoi(argv[1]);
    long int cols = atoi(argv[2]);
    int N = atoi(argv[3]);
    vector<int> first_matrix, second_matrix;
    vector<thread> sum_threads(N);
    
    auto start = chrono::high_resolution_clock::now();
    thread thread_read(read_matrices, &first_matrix, &second_matrix);
    thread_read.join();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Reading duration - " << duration.count() << "ms" << endl;
    
    vector<int> result_matrix(first_matrix.size());
    long long int size = (first_matrix.size() - 1) / N + 1;
    
    start = chrono::high_resolution_clock::now();
    for (long long int i = 0; i < N; i++)
    	sum_threads[i] = thread(sum_matrices, &first_matrix, &second_matrix, &result_matrix, i*size, i*size+size);
    for (long long int i = 0; i < sum_threads.size(); i++)
    	sum_threads[i].join();
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Sum duration - " << duration.count() << "ms" << endl;
    
    start = chrono::high_resolution_clock::now();
    thread thread_write(write_result, &result_matrix, rows, cols);
    thread_write.join();
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Writing duration - " << duration.count() << "ms" << endl;
    return 0;
}
