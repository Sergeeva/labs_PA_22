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

void sum_matrices(std::vector<std::vector<int>>& matrix1, std::vector<std::vector<int>>& matrix2, std::vector<std::vector<int>>& result){
    for (long long int i = 0; i < matrix1.size(); i++){
        std::vector<int> row_result(matrix1[0].size());
        std::transform(matrix1[i].begin(), matrix1[i].end(), matrix2[i].begin(), row_result.begin(), std::plus<>());
        result[i] = row_result;
    }
}

void write_result(std::vector<std::vector<int>>& result){
    std::ofstream file;
    file.open("Result.txt");
    for (int i = 0; i < result.size(); i++){
        for(int j = 0; j < result[i].size(); j++){
            file << result[i][j] << ' ';
        }
        file << std::endl;
    }
    file.close();
}

int main(){
    std::vector<std::vector<int>> matrix1, matrix2;

    clock_t start_time = clock();
    std::thread thread_read = std::thread{read_matrices, std::ref(matrix1), std::ref(matrix2)};
    thread_read.join();
    clock_t duration = clock() - start_time;
    std::cout << "Read duration - " << duration / (double) CLOCKS_PER_SEC << "s" << std::endl;

    std::vector<std::vector<int>> result(matrix1.size());
    start_time = clock();
    std::thread thread_sum = std::thread{sum_matrices, std::ref(matrix1), std::ref(matrix2), std::ref(result)};
    thread_sum.join();
    duration = clock() - start_time;
    std::cout << "Sum duration - " << duration / (double) CLOCKS_PER_SEC << "s" << std::endl;

    start_time = clock();
    std::thread thread_write = std::thread{write_result, std::ref(result)};
    thread_write.join();
    duration = clock() - start_time;
    std::cout << "Write duration - " << duration / (double) CLOCKS_PER_SEC << "s" << std::endl;
    return 0;
}
