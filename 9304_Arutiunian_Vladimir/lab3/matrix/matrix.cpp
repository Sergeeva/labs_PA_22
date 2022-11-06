#include "matrix.h"

Matrix::Matrix(size_t height, size_t width) : matrix_(
        std::vector<std::vector<int>>(height, std::vector<int>(width))),
                                              width_(width),
                                              height_(height) {}

size_t Matrix::get_width() const {
    return width_;
}

size_t Matrix::get_height() const {
    return height_;
}

size_t Matrix::get_dimension() const {
    return width_ * height_;
}

int Matrix::get(size_t height, size_t width) const {
    return matrix_[height][width];
}

int Matrix::set(size_t height, size_t width, int value) {
    return matrix_[height][width] = value;
}

Matrix Matrix::operator+(const Matrix& matrix) const {
    auto new_matrix = Matrix(*this);

    for (size_t i = 0; i < new_matrix.get_height(); ++i) {
        for (size_t j = 0; j < new_matrix.get_width(); ++j) {
            new_matrix.matrix_[i][j] += matrix.get(i, j);
        }
    }

    return new_matrix;
}

Matrix Matrix::parallel_sum(const Matrix& matrix, size_t threads_count) const {
    assert_correct_threads_count(threads_count);
    size_t dimension = matrix.get_dimension();

    size_t elements_count_per_thread = (dimension + threads_count - 1) / threads_count;

    auto summed_matrix = Matrix(height_, width_);

    std::vector<std::thread> threads;
    threads.reserve(threads_count);

    for (size_t i = 0; i < dimension; i += elements_count_per_thread) {
        threads.emplace_back(
                sum_part_of_vector,
                std::ref(summed_matrix),
                std::ref(*this),
                std::ref(matrix),
                i,
                elements_count_per_thread
        );
    }

    for (auto& thread: threads) {
        thread.join();
    }

    return summed_matrix;
}

void Matrix::assert_correct_threads_count(size_t threads_count) const {
    if (threads_count <= 0 || threads_count >= get_dimension()) {
        throw std::invalid_argument("Invalid threads count = " + std::to_string(threads_count));
    }
}

void Matrix::sum_part_of_vector(
        Matrix& summed_matrix,
        const Matrix& first,
        const Matrix& second,
        int begin_index,
        int length
) {
    size_t dimension = summed_matrix.get_dimension();
    for (size_t i = begin_index; i < begin_index + length && i < dimension; ++i) {
        size_t height = i / summed_matrix.get_width();
        size_t width = i % summed_matrix.get_width();
        int sum = first.get(height, width) + second.get(height, width);
        summed_matrix.set(height, width, sum);
    }
}

std::istream& operator>>(std::istream& in, Matrix& matrix) {
    in >> matrix.height_;
    in >> matrix.width_;

    size_t height = matrix.get_height();
    size_t width = matrix.get_width();

    matrix.matrix_.resize(height);

    for (size_t i = 0; i < height; ++i) {
        matrix.matrix_[i].resize(width);
        for (size_t j = 0; j < width; ++j) {
            in >> matrix.matrix_[i][j];
        }
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
    size_t height = matrix.get_height();
    size_t width = matrix.get_width();

    out << height << ' ';
    out << width << '\n';

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            out << matrix.get(i, j);
            if (j < width - 1) {
                out << ' ';
            }
        }
        out << '\n';
    }

    return out;
}
