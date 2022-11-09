#include "matrix.h"

#include <utility>

Matrix::Matrix(size_t height, size_t width) :
        matrix_(std::vector<std::vector<int>>(height, std::vector<int>(width))),
        width_(width),
        height_(height) {}

Matrix::Matrix(size_t height, size_t width, std::vector<std::vector<int>> matrix) :
        matrix_(std::move(matrix)),
        width_(width),
        height_(height) {}

size_t Matrix::get_width() const {
    return width_;
}

size_t Matrix::get_height() const {
    return height_;
}

int Matrix::get(size_t height, size_t width) const {
    return matrix_[height][width];
}

int Matrix::set(size_t height, size_t width, int value) {
    return matrix_[height][width] = value;
}

Matrix Matrix::operator+(const Matrix& matrix) const {
    auto newMatrix = Matrix(*this);

    for (size_t i = 0; i < newMatrix.get_height(); ++i) {
        for (size_t j = 0; j < newMatrix.get_width(); ++j) {
            newMatrix.matrix_[i][j] += matrix.get(i, j);
        }
    }

    return newMatrix;
}

Matrix Matrix::operator-(const Matrix& matrix) const {
    auto newMatrix = Matrix(*this);

    for (size_t i = 0; i < newMatrix.get_height(); ++i) {
        for (size_t j = 0; j < newMatrix.get_width(); ++j) {
            newMatrix.matrix_[i][j] -= matrix.get(i, j);
        }
    }

    return newMatrix;
}

bool Matrix::operator==(const Matrix& matrix) const {
    return get_width() == matrix.get_width()
           && get_height() == matrix.get_height()
           && matrix_ == matrix.matrix_;
}

Matrix Matrix::multiply(const Matrix& matrix, size_t threads_count) const {
    assert_correct_threads_count(threads_count);
    assert_correct_matrices_sizes_for_multiplying(matrix);

    size_t height = get_height();
    size_t rows_count_per_thread = (height + threads_count - 1) / threads_count;
    auto result_matrix = Matrix(height, matrix.get_width());
    std::vector<std::thread> threads;

    for (size_t offset = 0; offset < height; offset += rows_count_per_thread) {
        std::thread alone_thread(
                multiply_part,
                offset,
                rows_count_per_thread,
                std::ref(*this),
                std::ref(matrix),
                std::ref(result_matrix)
        );
        threads.push_back(std::move(alone_thread));
    }

    for (auto& thread: threads) {
        thread.join();
    }

    return result_matrix;
}

void Matrix::assert_correct_threads_count(size_t threads_count) const {
    if (threads_count <= 0 || threads_count > get_height()) {
        throw std::invalid_argument("Invalid threads count: " + std::to_string(threads_count));
    }
}

void Matrix::assert_correct_matrices_sizes_for_multiplying(
        const Matrix& matrix
) const {
    if (get_width() != matrix.get_height()) {
        throw std::invalid_argument(
                "Invalid matrices dimensions. First matrix width = " +
                std::to_string(get_width()) +
                ". Second matrix height = " +
                std::to_string(matrix.get_height())
        );
    }
}

void Matrix::multiply_part(
        size_t begin_row,
        size_t length,
        const Matrix& first_matrix,
        const Matrix& second_matrix,
        Matrix& result_matrix
) {
    auto after_last_row = begin_row + length;
    for (size_t i = begin_row; i < std::min(after_last_row, first_matrix.get_height()); ++i) {
        for (size_t k = 0; k < second_matrix.get_width(); ++k) {
            int row_sum = 0;
            for (size_t j = 0; j < second_matrix.get_height(); ++j) {
                row_sum += first_matrix.get(i, j) * second_matrix.get(j, k);
            }
            result_matrix.set(i, k, row_sum);
        }

    }
}

Matrix Matrix::strassen_multiply(const Matrix& matrix, size_t depth) const {
    assert_correct_matrices_sizes_for_strassen_multiplying(matrix);

    if (get_width() <= STRASSEN_MIN_MATRIX_SIZE) {
        return multiply(matrix, THREADS_COUNT);
    } else if (depth < MAX_DEPTH) {
        auto future = std::async(
                std::launch::async,
                strassen_multiply_inner,
                std::ref(*this),
                std::ref(matrix),
                depth
        );
        return future.get();
    }

    return strassen_multiply_inner(*this, matrix, depth);
}

void Matrix::assert_correct_matrices_sizes_for_strassen_multiplying(
        const Matrix& matrix
) const {
    if (
            is_power_of_two(get_width()) && is_power_of_two(matrix.get_height())
            && get_width() == get_height() == matrix.get_width() == matrix.get_height()
            ) {
        throw std::invalid_argument(
                "Invalid matrices dimensions. First matrix shape = (" +
                std::to_string(get_width()) + ", " + std::to_string(get_height()) +
                "). Second matrix shape = (" +
                std::to_string(matrix.get_height()) + ", " + std::to_string(matrix.get_width()) + ")"
        );
    }
}


bool Matrix::is_power_of_two(size_t number) {
    return number != 0 && (number & (number - 1)) == 0;
}

Matrix Matrix::strassen_multiply_inner(
        const Matrix& first_matrix,
        const Matrix& second_matrix,
        size_t max_depth
) {

    auto a = first_matrix.split_to_2x2();
    auto b = second_matrix.split_to_2x2();

    auto new_max_depth = max_depth + 1;
    auto p1 = (a[0][0] + a[1][1]).strassen_multiply(b[0][0] + b[1][1], new_max_depth);
    auto p2 = (a[1][0] + a[1][1]).strassen_multiply(b[0][0], new_max_depth);
    auto p3 = a[0][0].strassen_multiply(b[0][1] - b[1][1], new_max_depth);
    auto p4 = a[1][1].strassen_multiply(b[1][0] - b[0][0], new_max_depth);
    auto p5 = (a[0][0] + a[0][1]).strassen_multiply(b[1][1], new_max_depth);
    auto p6 = (a[1][0] - a[0][0]).strassen_multiply(b[0][0] + b[0][1], new_max_depth);
    auto p7 = (a[0][1] - a[1][1]).strassen_multiply(b[1][0] + b[1][1], new_max_depth);

    auto c11 = p1 + p4 - p5 + p7;
    auto c12 = p3 + p5;
    auto c21 = p2 + p4;
    auto c22 = p1 - p2 + p3 + p6;

    return merge_from_2x2(c11, c12, c21, c22);
}

std::vector<std::vector<Matrix>> Matrix::split_to_2x2() const {
    size_t width_middle = get_width() / 2;
    size_t height_middle = get_height() / 2;

    auto a11 = copy_part(0, height_middle, 0, width_middle);
    auto a12 = copy_part(0, height_middle, width_middle, get_width());

    auto a21 = copy_part(height_middle, get_height(), 0, width_middle);
    auto a22 = copy_part(height_middle, get_height(), width_middle, get_width());

    return {{a11, a12},
            {a21, a22}};
}

Matrix Matrix::copy_part(size_t first_row, size_t after_last_row, size_t first_column, size_t after_last_column) const {
    std::vector<std::vector<int>> matrix_part;
    for (size_t i = first_row; i < after_last_row; ++i) {
        auto begin = matrix_[i].begin() + (long) first_column;
        auto end = matrix_[i].begin() + (long) after_last_column;
        matrix_part.emplace_back(begin, end);
    }
    return {matrix_part.size(), matrix_part[0].size(), matrix_part};
}


Matrix Matrix::merge_from_2x2(const Matrix& c11, const Matrix& c12, const Matrix& c21, const Matrix& c22) {
    std::vector<std::vector<int>> merged = c11.matrix_;

    for (size_t i = 0; i < merged.size(); ++i) {
        merged[i].insert(merged[i].end(), c12.matrix_[i].begin(), c12.matrix_[i].end());
    }

    merged.insert(merged.end(), c21.matrix_.begin(), c21.matrix_.end());

    size_t height_middle = c11.get_height();
    for (size_t i = height_middle; i < merged.size(); ++i) {
        size_t c22_index = i - height_middle;
        merged[i].insert(merged[i].end(), c22.matrix_[c22_index].begin(), c22.matrix_[c22_index].end());
    }

    auto width = c11.get_width() + c12.get_width();
    auto height = c11.get_height() + c21.get_height();

    return {height, width, merged};
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
