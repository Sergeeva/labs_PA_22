#include <fstream>
#include "Matrix.h"
using namespace std;

Matrix::Matrix(const int rows, const int columns): matrix(vector<vector<int>>(rows, vector<int>(columns))), columns(columns), rows(rows) {}

//память, в которую записываю матрицу
int *Matrix::writeByPtr(int *ptr) const {

    ptr[0] = rows;

    ptr[1] = columns;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            ptr[i*columns+j+2] = matrix[i][j];

            //cout<<i+j+2<<endl;
           
        }
    }

     //cout<<endl;

    return ptr+rows+columns+2;
}

//Из памяти читает матрицу в класс/ копирую внутрь объекта
pair<Matrix, int *> Matrix::readByPtr(int *ptr) 
{

    int rows = ptr[0];

    int columns = ptr[1];

    auto result = Matrix(columns, rows);

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.columns; j++) {
            result.matrix[i][j] = ptr[i*columns+j+2];
        }
    }
    return make_pair(result, ptr+rows+columns+2);//указатель на след.матрицу
}

size_t Matrix::get_rows() const
{
    return this->rows;
}

size_t Matrix::get_columns() const 
{
    return this->columns;
}

bool Matrix::check_dimensions(const Matrix& other) const
{
    if (this->get_rows()!= other.get_rows() or this->get_columns() != other.get_columns()) {
        return false;
    }
    return true;
}


Matrix Matrix::operator+(const Matrix &matrix) const 
{
    auto Matrixnew = Matrix(*this);

    if (Matrixnew.check_dimensions(matrix)){

        for (int i = 0; i < Matrixnew.rows; i++) {
            for (int j = 0; j < Matrixnew.columns; j++) {
                Matrixnew.matrix[i][j] += matrix.matrix[i][j];
            }
        }
    }
    else
    {
        throw invalid_argument("Сложение не выполнить. Размер матриц не совпадает.");
    }
    return Matrixnew;
}

//оператор ввода из потока(забираем)
istream &operator>>(istream &in, Matrix &matrix) 
{

    in >> matrix.rows;
    in >> matrix.columns;

    matrix.matrix.resize(matrix.rows);

    for (int i = 0; i < matrix.rows; ++i) {
        matrix.matrix[i].resize(matrix.columns);
        for (int j = 0; j < matrix.columns; ++j) {
            in >> matrix.matrix[i][j];
        }
    }

    return in;
}


//считывание из потока
ostream &operator<<(ostream &out, const Matrix &matrix)
 {
    out << matrix.rows << ' ';
    out << matrix.columns << '\n';

    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = 0; j < matrix.columns; ++j) {
            out << matrix.matrix[i][j];
            if (j < matrix.columns - 1) {
                out << ' ';
            }
        }
        out << '\n';
    }

    return out;
}

//создается файл name, туда записывается матрица
void Matrix::writeMatrix(const Matrix &matrix, const string& name) 
{
    ofstream outputFile(name);
    outputFile << matrix;
    outputFile.close();
}


void Matrix::readMatrix2(Matrix &Matrix1, Matrix &Matrix2) 
{
    ifstream inputMatrix2("input.txt");

    inputMatrix2 >> Matrix1;
    inputMatrix2 >> Matrix2;

    inputMatrix2.close();
}


Matrix Matrix::parallelSum(const Matrix &matrix, const int count_thread) const 
{
    int size = matrix.columns * matrix.rows;

    if (count_thread <= 0 || count_thread >= size) 
    {
        throw runtime_error("Недопустимое количество потоков"); 
    }
    
    
    int elementsPerThread = (size / count_thread) + (size % count_thread > 0 ? 1 : 0);//кол-во элементов в один поток 

    auto result = Matrix(rows, columns);

    vector<thread> threads;

    for (int i = 0; i < size; i += elementsPerThread) 
    {
        thread thread(sumPart, ref(*this), ref(matrix), i, elementsPerThread, ref(result));
        threads.emplace_back(move(thread));
    }

//проходимся по вектору
    for (auto &thread: threads) 
    {
        thread.join();
    }

    return result;
}

void Matrix::sumPart(const Matrix &Matrix1, const Matrix &Matrix2, int start, int length, Matrix &result)
{
    int size = result.columns * result.rows;

    for (int i = start; i < start + length && i < size; ++i) 
    {
        int h = i / result.columns;//индекс в строке
        int w = i % result.columns;
        result.matrix[h][w] = Matrix1.matrix[h][w] + Matrix2.matrix[h][w];
    }
}