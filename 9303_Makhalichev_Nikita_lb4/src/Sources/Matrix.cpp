#include "../Headers/Matrix.h"


Matrix::Matrix(int rows, int cols): rows_(rows), cols_(cols), real_cols_(cols){
    std::srand(std::time(0));
    this->matrix_.resize(rows * cols);
}

int Matrix::get_rows(){
    return this->rows_;
}

void Matrix::set_rows(int rows){
    this->rows_ = rows;
}

int Matrix::get_cols(){
    return this->cols_;
}

void Matrix::set_cols(int cols){
    this->cols_ = cols;
}

void Matrix::set_real_cols(int cols){
    this->real_cols_ = cols;
}

int Matrix::get_element(int index){
    return this->matrix_[index];
}

void Matrix::set_element(int index, int element){
    this->matrix_[index] = element;
}

void Matrix::generate(){
    for (int i = 0; i < this->rows_ * this->cols_; i++)
        this->matrix_[i] = std::rand() % 16 - 8;
}

std::vector<Matrix> Matrix::get_parts(){
    int part_size = rows_ / 2;
    std::vector<Matrix> result;
    result.resize(4, Matrix(part_size, part_size));
    for (int i = 0; i < rows_; i++)
        for (int j = 0; j < cols_; j++)
            result[(i/part_size)*2 + j/part_size].set_element(part_size*(i%part_size) + j%part_size, matrix_[i*cols_ + j]);
    return result;
}

void Matrix::concat_parts(std::vector<Matrix> &parts){
    int part_size = parts[0].get_rows();
    for (int i = 0; i < rows_; i++)
        for (int j = 0; j < cols_; j++)
            matrix_[i*cols_ + j] = parts[(i/part_size)*2 + j/part_size].get_element(part_size*(i%part_size) + j%part_size);
}

void vector_sum(Matrix* first_matrix, Matrix* second_matrix, Matrix* result, int start_index, int end_index){
    for (int i = start_index; i < end_index; i++)
        result->set_element(i, first_matrix->get_element(i) + second_matrix->get_element(i));
}

Matrix sum(Matrix& first_matrix, Matrix& second_matrix, int threads_count){
    Matrix result(first_matrix.get_rows(), first_matrix.get_cols());
    int size = (first_matrix.get_rows() * first_matrix.get_cols()) / threads_count;
    int mod = (first_matrix.get_rows() * first_matrix.get_cols()) % threads_count;
    if (threads_count > 1){
        std::vector<std::thread> threads(threads_count);
        int start_index = 0;
        for (auto &thread: threads){
            int end_index = start_index + size;
            if (mod){
                end_index++;
                mod--;
            }
            thread = std::thread(vector_sum, &first_matrix, &second_matrix, &result, start_index, end_index);
            start_index = end_index;
        }
        for (auto &thread: threads)
            thread.join();
    } else {
        vector_sum(&first_matrix, &second_matrix, &result, 0, result.get_rows() * result.get_cols());
    }
    return result;
}

void vector_sub(Matrix* first_matrix, Matrix* second_matrix, Matrix* result, int start_index, int end_index){
    for (int i = start_index; i < end_index; i++)
        result->set_element(i, first_matrix->get_element(i) - second_matrix->get_element(i));
}

Matrix sub(Matrix& first_matrix, Matrix& second_matrix, int threads_count){
    Matrix result(first_matrix.get_rows(), first_matrix.get_cols());
    if (threads_count > 1){
        int size = (first_matrix.get_rows() * first_matrix.get_cols()) / threads_count;
        int mod = (first_matrix.get_rows() * first_matrix.get_cols()) % threads_count;
        std::vector<std::thread> threads(threads_count);
        int start_index = 0;
        for (auto &thread: threads){
            int end_index = start_index + size;
            if (mod){
                end_index++;
                mod--;
            }
            thread = std::thread(vector_sub, &first_matrix, &second_matrix, &result, start_index, end_index);
            start_index = end_index;
        }
        for (auto &thread: threads)
            thread.join();
    } else
        vector_sub(&first_matrix, &second_matrix, &result, 0, result.get_rows() * result.get_cols());
    return result;
}

void vector_mult(Matrix* first_matrix, Matrix* second_matrix, Matrix* result, int start_index, int end_index){
    for (int i = start_index; i < end_index; i++){
        int element = 0;
        int current_row = i / result->get_cols();
        int current_col = i % result->get_cols();
        for (int j = 0; j < first_matrix->get_cols(); j++)
            element += (first_matrix->get_element(current_row * first_matrix->get_cols() + j) * second_matrix->get_element(j*second_matrix->get_cols() + current_col));
        result->set_element(i, element);
    }
}

void Matrix::mult(Matrix& first_matrix, Matrix& second_matrix, int threads_count){
    int result_size = matrix_.size();
    if (threads_count > 1){
        int size = result_size / threads_count;
        int mod = result_size % threads_count;
        std::vector<std::thread> threads(threads_count);
        int start_index = 0;
        for (auto &thread: threads){
            int end_index = start_index + size;
            if (mod){
                end_index++;
                mod--;
            }
            thread = std::thread(vector_mult, &first_matrix, &second_matrix, this, start_index, end_index);
            start_index = end_index;
        }
        for (auto &thread: threads)
            thread.join();
    } else
        vector_mult(&first_matrix, &second_matrix, this, 0, this->matrix_.size());
}

void strassen_step(Matrix &first_matrix, Matrix &second_matrix, Matrix *result_matrix, int depth, int thread_count, bool is_parallel){
    if ((!depth) || (result_matrix->get_rows()*result_matrix->get_cols() <= 64)){
        result_matrix->mult(first_matrix, second_matrix, 1);
    } else {
        std::vector<Matrix> first_parts = first_matrix.get_parts();
        std::vector<Matrix> second_parts = second_matrix.get_parts();
        std::vector<Matrix> m;
        int part_size = first_parts[0].get_rows();
        m.resize(7, Matrix(part_size, part_size));
        if (is_parallel){
            std::vector<std::thread> m_threads(7);
            m_threads[0] = std::thread([&m, &depth, &thread_count](Matrix first_argument, Matrix second_argument)
                                          {strassen_step(first_argument, second_argument, &m[0], depth-1, thread_count, false);},
                                          sum(first_parts[0], first_parts[3], thread_count), sum(second_parts[0], second_parts[3], thread_count));
            m_threads[1] = std::thread([&m, &second_parts, &depth, &thread_count](Matrix first_argument)
                                          {strassen_step(first_argument, second_parts[0], &m[1], depth-1, thread_count, false);},
                                          sum(first_parts[2], first_parts[3], thread_count));
            m_threads[2] = std::thread([&m, &first_parts, &depth, &thread_count](Matrix second_argument)
                                          {strassen_step(first_parts[0], second_argument, &m[2], depth-1, thread_count, false);},
                                          sub(second_parts[1], second_parts[3], thread_count));
            m_threads[3] = std::thread([&m, &first_parts, &depth, &thread_count](Matrix second_argument)
                                          {strassen_step(first_parts[3], second_argument, &m[3], depth-1, thread_count, false);},
                                          sub(second_parts[2], second_parts[0], thread_count));
            m_threads[4] = std::thread([&m, &second_parts, &depth, &thread_count](Matrix first_argument)
                                          {strassen_step(first_argument, second_parts[3], &m[4], depth-1, thread_count, false);},
                                          sum(first_parts[0], first_parts[1], thread_count));
            m_threads[5] = std::thread([&m, &depth, &thread_count](Matrix first_argument, Matrix second_argument)
                                          {strassen_step(first_argument, second_argument, &m[5], depth-1, thread_count, false);},
                                          sub(first_parts[2], first_parts[0], thread_count), sum(second_parts[0], second_parts[1], thread_count));
            m_threads[6] = std::thread([&m, &depth, &thread_count](Matrix first_argument, Matrix second_argument)
                                          {strassen_step(first_argument, second_argument, &m[6], depth-1, thread_count, false);},
                                          sub(first_parts[1], first_parts[3], thread_count), sum(second_parts[2], second_parts[3], thread_count));
            for (auto &m_thread: m_threads)
                m_thread.join();
        } else {
            Matrix first_argument = sum(first_parts[0], first_parts[3], thread_count);
            Matrix second_argument = sum(second_parts[0], second_parts[3], thread_count);
            std::vector<std::thread> p_threads(7);
            strassen_step(first_argument, second_argument, &m[0], depth-1, thread_count, false);

            first_argument = sum(first_parts[2], first_parts[3], thread_count);
            strassen_step(first_argument, second_parts[0], &m[1], depth-1, thread_count, false);

            second_argument = sub(second_parts[1], second_parts[3], thread_count);
            strassen_step(first_parts[0], second_argument, &m[2], depth-1, thread_count, false);

            second_argument = sub(second_parts[2], second_parts[0], thread_count);
            strassen_step(first_parts[3], second_argument, &m[3], depth-1, thread_count, false);

            first_argument = sum(first_parts[0], first_parts[1], thread_count);
            strassen_step(first_argument, second_parts[3], &m[4], depth-1, thread_count, false);

            first_argument = sub(first_parts[2], first_parts[0], thread_count);
            second_argument = sum(second_parts[0], second_parts[1], thread_count);
            strassen_step(first_argument, second_argument, &m[5], depth-1, thread_count, false);

            first_argument = sub(first_parts[1], first_parts[3], thread_count);
            second_argument = sum(second_parts[2], second_parts[3], thread_count);
            strassen_step(first_argument, second_argument, &m[6], depth-1, thread_count, false);
        }
        std::vector<Matrix>c(4, Matrix(part_size, part_size));
        c[0] = sum(m[0], m[3], thread_count);
        c[0] = sub(c[0], m[4], thread_count);
        c[0] = sum(c[0], m[6], thread_count);
        c[1] = sum(m[2], m[4], thread_count);
        c[2] = sum(m[1], m[3], thread_count);

        c[3] = sub(m[0], m[1], thread_count);
        c[3] = sum(c[3], m[2], thread_count);
        c[3] = sum(c[3], m[5], thread_count);

        result_matrix->concat_parts(c);
    }
}

Matrix strassen(Matrix &first_matrix, Matrix &second_matrix, int depth, int thread_count){
    int old_first_rows = first_matrix.get_rows();
    int old_first_cols = first_matrix.get_cols();
    int old_second_cols = second_matrix.get_cols();
    int size1 = std::pow(2, std::ceil(std::log(old_first_rows)/std::log(2)));
    int size2 = std::pow(2, std::ceil(std::log(old_first_cols)/std::log(2)));
    int size3 = std::pow(2, std::ceil(std::log(old_second_cols)/std::log(2)));
    int size_max = std::max({size1, size2, size3});
    if ((size_max != old_first_rows) || (size_max != old_first_cols) || (size_max != old_second_cols)){
        Matrix new_first_matrix(size_max, size_max);
        Matrix new_second_matrix(size_max, size_max);
        for (int i = 0; i < first_matrix.get_cols(); i++){
            for (int j = 0; j < first_matrix.get_rows(); j++)
                new_first_matrix.set_element(j*size_max+i, first_matrix.get_element(j*first_matrix.get_cols()+i));
            for (int j = 0; j < second_matrix.get_cols(); j++)
                new_second_matrix.set_element(i* size_max + j, second_matrix.get_element(i*second_matrix.get_cols()+j));
        }
        first_matrix = new_first_matrix;
        second_matrix = new_second_matrix;
    }
    Matrix result_matrix(size_max, size_max);
    strassen_step(first_matrix, second_matrix, &result_matrix, depth, thread_count, true);
    result_matrix.set_rows(old_first_rows);
    result_matrix.set_cols(old_second_cols);
    result_matrix.set_real_cols(size_max);
    return result_matrix;
}

std::ostream& operator<<(std::ostream& stream, const Matrix& matrix){
    for (int i = 0; i < matrix.rows_; i++){
        for (int j = 0; j < matrix.cols_; j++)
            stream << std::setw(5) << matrix.matrix_[i*matrix.real_cols_ + j] << ' ';
        stream << '\n';
    }
    return stream;
}