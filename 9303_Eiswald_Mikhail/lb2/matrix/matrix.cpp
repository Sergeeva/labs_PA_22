#include "matrix.h"

Matrix::Matrix(int rows_num, int cols_num): rows_num(rows_num), cols_num(cols_num) {
    data = *(new vector<vector<int>>(rows_num, vector<int>(cols_num)));
    chars_for_element = floor(log10(numeric_limits<int>::max())) + 1 + 1;
}

Matrix::Matrix(int rows_num, int cols_num, int max_module) :
        rows_num(rows_num), cols_num(cols_num) {
    this->data = *(new vector<vector<int>>(rows_num, vector<int>(cols_num)));
    srand((long unsigned)this + rand());
    for(int i = 0; i < rows_num; ++i) {
        for(int j = 0; j < cols_num; ++j) {
            data[i][j] = (rand() % (2*max_module + 1)) - max_module;
        }
    }
    chars_for_element = floor(log10(max_module)) + 1 + 1;
}

Matrix::Matrix(int* buf) {
    rows_num = buf[0];
    cols_num = buf[1];
    buf += 2;
    data = *(new vector<vector<int>>(rows_num, vector<int>(cols_num)));
    for(int i = 0; i < rows_num; ++i) {
        for(int j = 0; j < cols_num; ++j) {
            data[i][j] = *buf;
            ++buf;
        }
    }
    chars_for_element = floor(log10(numeric_limits<int>::max())) + 1 + 1;
}

int Matrix::get_rows_num() const {
    return rows_num;
}

int Matrix::get_cols_num() const {
    return cols_num;
}

int Matrix::get(int row_index, int col_index) const {
    return data[row_index][col_index];
}

void Matrix::set(int row_index, int col_index, int value) {
    data[row_index][col_index] = value;
}

int *Matrix::serialize() const {
    int* buf = new int[2 + rows_num*cols_num];
    buf[0] = rows_num;
    buf[1] = cols_num;
    for(int i = 2; i < 2 + rows_num*cols_num; ++i) {
        buf[i] = data[(i-2)/cols_num][(i-2)%cols_num];
    }
    return buf;
}

Matrix Matrix::operator+(const Matrix &other) const {
    Matrix result = Matrix(rows_num, cols_num);
    for(int i = 0; i < rows_num; ++i) {
        for(int j = 0; j < cols_num; ++j) {
            result.set(i, j, data[i][j] + other.get(i, j));
        }
    }
    return result;
}

#define start_border(row_number, rows_num) \
    (rows_num == 1 ? '|' : row_number == 0 ? '/' : row_number == rows_num-1 ? '\\' : '|' )
#define end_border(row_number, rows_num) \
    (rows_num == 1 ? '|' : row_number == 0 ? '\\' : row_number == rows_num-1 ? '/' : '|' )

ostream& operator<<(ostream& out, const Matrix& self) {
    for(int i = 0; i < self.rows_num; ++i) {
        out << start_border(i, self.rows_num) << " ";
        for(int j = 0; j < self.cols_num; ++j) {
            out << setw(self.chars_for_element);
            out << self.data[i][j] << " ";
        }
        out << end_border(i, self.rows_num) << "\n";
    }
    return out;
}
