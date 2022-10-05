#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>


void read_matrix(std::vector<std::vector<int>>& matrix, std::string filename){
    std::ifstream file;
    std::string line;
    std::string delimiter = " ";
    std::vector<int> row;
    file.open(filename);
    while (getline(file, line)){
        size_t pos;
        std::string num;
        row.clear();
        while ((pos = line.find(delimiter)) != std::string::npos) {
            num = line.substr(0, pos);
            row.push_back(stoi(num));
            line.erase(0, pos + delimiter.length());
        }
        matrix.push_back(row);
    }
    file.close();
}

void read_matrices(std::vector<std::vector<int>>& matrix1, std::vector<std::vector<int>>& matrix2){
    read_matrix(matrix1, "Matrix1.txt");
    read_matrix(matrix2, "Matrix2.txt");
}

void sum_matrices(std::vector<int>& matrix1, std::vector<int>& matrix2, std::vector<int>& result, int start_index, int end_index){
    for (long long int i = start_index; i <= end_index; i++){
            result[i] = matrix1[i] + matrix2[i];
    }
}

void convert_matrix(std::vector<std::vector<int>>& init_matrix, std::vector<int>& final_matrix){
    for (int i = 0; i < init_matrix.size(); i++){
        for (int j = 0; j < init_matrix[i].size(); j++){
            final_matrix.push_back(init_matrix[i][j]);
        }
    }
}

void write_result(std::vector<int>& result, int rows, int columns){
    std::ofstream file;
    file.open("Result2.txt");
    for (long int i = 0; i < rows; i++){
        for (long int j = 0; j < columns; j++)
            file << result[i * columns + j] << ' ';
        file << std::endl;
    }
    file.close();
}

int main(int argc, char *argv[]){
    int num_treads = atoi(argv[1]);
    std::vector<std::thread> sum_threads(num_treads);
    std::vector<std::vector<int>> matrix1, matrix2;
    std::vector<int> convert_matrix1, convert_matrix2;

    clock_t start_time = clock();
    std::thread thread_read = std::thread{read_matrices, std::ref(matrix1), std::ref(matrix2)};
    thread_read.join();
    clock_t duration = clock() - start_time;
    std::cout << "Read duration - " << duration / (double) CLOCKS_PER_SEC << "s" << std::endl;

    convert_matrix(matrix1, convert_matrix1);
    convert_matrix(matrix2, convert_matrix2);

    std::vector<int> result(convert_matrix1);
    int min_count;
    int min_thread_rows_size = convert_matrix1.size() / num_treads;
    int max_thread_rows_size = min_thread_rows_size + 1;
    if (convert_matrix1.size() % num_treads == 0){
        min_count = num_treads;
    }
    else{
        for (int i = 0; i < convert_matrix1.size(); i++) {
            if ((convert_matrix1.size() - min_thread_rows_size * i) % max_thread_rows_size == 0) {
                min_count = i;
                break;
            }
        }
    }

    start_time = clock();
    for (int i = 0; i < num_treads; i++){
        if (i < min_count){
            sum_threads[i] = std::thread(sum_matrices, std::ref(convert_matrix1), std::ref(convert_matrix2), std::ref(result),
                                         i * min_thread_rows_size, (i + 1) * min_thread_rows_size - 1);
        }
        else{
            sum_threads[i] = std::thread(sum_matrices, std::ref(convert_matrix1), std::ref(convert_matrix2), std::ref(result),
                                         min_count * min_thread_rows_size + (i - min_count) * max_thread_rows_size, min_count * min_thread_rows_size + (i - min_count + 1) * max_thread_rows_size - 1);
        }
    }

    for (int i = 0; i < sum_threads.size(); i++){
        sum_threads[i].join();
    }

    duration = clock() - start_time;
    std::cout << "Sum duration - " << duration / (double) CLOCKS_PER_SEC << "s" << std::endl;

    start_time = clock();
    std::thread thread_write = std::thread{write_result, std::ref(result), matrix1.size(), matrix1[0].size()};
    thread_write.join();
    duration = clock() - start_time;
    std::cout << "Write duration - " << duration / (double) CLOCKS_PER_SEC << "s" << std::endl;
    return 0;
}
