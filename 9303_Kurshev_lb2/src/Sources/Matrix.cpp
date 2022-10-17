#include "../Headers/Matrix.h"


Matrix::Matrix(int rows, int cols): rows(rows), columns(cols), matrix(std::vector<int>(rows * cols)){}

int Matrix::get_item(int item_index){
    return this->matrix[item_index];
}

void Matrix::set_item(int item_index, int element_value){
    this->matrix[item_index] = element_value;
}

void Matrix::matrix_generate(){
    for (int i = 0; i < this->rows * this->columns; i++)
        this->matrix[i] = std::rand() % 10000;
}

int Matrix::size() {
    return this->matrix.size();
}

void sum_matrices(Matrix& first_matrix, Matrix& second_matrix, Matrix& result, int start_index, int end_index){
    for (int i = start_index; i <= end_index; i++)
        result.set_item(i, first_matrix.get_item(i) + second_matrix.get_item(i));
}

void Matrix::sum(Matrix& first_matrix, Matrix& second_matrix, Matrix& result, int num_threads) {
    std::vector<std::thread> sum_threads(num_threads);
    int min_count = 0;
    int min_thread_rows_size = first_matrix.size() / num_threads;
    int max_thread_rows_size = min_thread_rows_size + 1;
    if (matrix.size() % num_threads == 0) {
        min_count = num_threads;
    }
    else {
        for (int i = 0; i < first_matrix.size(); i++) {
            if ((first_matrix.size() - min_thread_rows_size * i) % max_thread_rows_size == 0) {
                min_count = i;
                break;
            }
        }
    }
    for (int i = 0; i < num_threads; i++) {
        if (i < min_count) {
            sum_threads[i] = std::thread(sum_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         std::ref(result), i * min_thread_rows_size,
                                         (i + 1) * min_thread_rows_size - 1);
        } else {
            sum_threads[i] = std::thread(sum_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         std::ref(result),
                                         min_count * min_thread_rows_size + (i - min_count) * max_thread_rows_size,
                                         min_count * min_thread_rows_size + (i - min_count + 1) * max_thread_rows_size -
                                         1);
        }
    }

    for (auto & sum_thread : sum_threads){
        sum_thread.join();
    }
}

void Matrix::write_result() {
    std::ofstream file;
    file.open("Result.txt", std::ios_base::app);
    for (long int i = 0; i < this->rows; i++){
        for (long int j = 0; j < this->columns; j++)
            file << this->matrix[i * this->columns + j] << ' ';
        file << std::endl;
    }
    file << std::endl;
    file.close();
}


