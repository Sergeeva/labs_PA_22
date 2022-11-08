#include "../Headers/Matrix.h"


Matrix::Matrix(int rows, int cols): rows_(rows), cols_(cols){
    this->matrix_.resize(rows * cols);
}

int Matrix::get_rows(){
    return this->rows_;
}

int Matrix::get_cols(){
    return this->cols_;
}

int Matrix::get_element(int index){
    return this->matrix_[index];
}

void Matrix::set_element(int index, int element){
    this->matrix_[index] = element;
}

void Matrix::generate(){
    for (int i = 0; i < this->rows_ * this->cols_; i++)
        this->matrix_[i] = std::rand() % 1000 - 500;
}

void vector_sum(Matrix* first_matrix, Matrix* second_matrix, Matrix* result, int start_index, int end_index){
    for (int i = start_index; i < end_index; i++)
        result->set_element(i, first_matrix->get_element(i) + second_matrix->get_element(i));
}

void Matrix::sum(Matrix& first_matrix, Matrix& second_matrix, int threads_count){
    std::vector<std::thread> threads(threads_count);
    int size = (this->rows_ * this->cols_) / threads_count;
    int mod = (this->rows_ * this->cols_) % threads_count;
    int start_index = 0;
    for (auto &thread: threads){
        int end_index = start_index + size;
        if (mod){
            end_index++;
            mod--;
        }
        thread = std::thread(vector_sum, &first_matrix, &second_matrix, this, start_index, end_index);
        start_index = end_index;
    }
    for (auto &thread: threads)
        thread.join();
}

std::ostream& operator<<(std::ostream& stream, const Matrix& matrix){
    for(int i = 0; i < matrix.rows_ * matrix.cols_; i++) {
        stream << std::setw(5) << matrix.matrix_[i] << ' ';
        if (((i + 1) % matrix.cols_) == 0)
            stream << '\n';
    }
    return stream;
}