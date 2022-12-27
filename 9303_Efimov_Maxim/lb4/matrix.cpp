#include "matrix.h"

Matrix::Matrix(){}

Matrix::Matrix(int N, int M)
{
    std::random_device seed;
    std::mt19937 generator(seed());
    std::uniform_int_distribution<> distrib(0, 9);

    matrix = std::vector<std::vector<int>> (N);
    for (auto &vec : matrix) {
        vec.reserve(M);
        for (int i = 0;i < M;i++) {
            vec.push_back(distrib(generator));
        }
    }
}

void Matrix::add(Matrix& term)
{
    for(auto row = 0; row < matrix.size(); row++) {
        for(auto column = 0; column < matrix[row].size(); column++) {
            matrix[row][column] += term.matrix[row][column];
        }
    }
}

int * Matrix::serialization()
{
    auto count = matrix.size() * matrix[0].size();
    int *buf = new int[count];
    int index = 0;
    for(auto row = 0; row < matrix.size(); row++) {
        for(auto column = 0; column < matrix[row].size(); column++) {
            buf[index] = matrix[row][column];
            index++;
            }
        }
    return buf;
}

void Matrix::deserialization(int* buf,int N, int M)
{
    matrix = std::vector<std::vector<int>> (N);
    for (auto &vec : matrix) {
        vec.resize(M);
    }
    int index = 0;
    for(auto row = 0; row < matrix.size(); row++) {
        for(auto column = 0; column < matrix[row].size(); column++) {
            matrix[row][column] = buf[index];
            index++;
        }
    }
}

void Matrix::fileOutput(char* fileName)
{
        std::ofstream fout(fileName);
        for(auto row = 0; row < matrix.size(); row++) {
            for(auto column = 0; column < matrix[row].size(); column++) {
                fout << matrix[row][column] << " ";
                }
            fout << std::endl;
            }
        fout.close();
}

void Matrix::set_zero()
{
        for(auto row = 0; row < matrix.size(); row++) {
        for(auto column = 0; column < matrix[row].size(); column++) {
            matrix[row][column] = 0;
        }
    }
}
Matrix Matrix::operator=(Matrix copy)
{
    matrix =   copy.matrix;
    return *this;

}

Matrix operator*(Matrix first, Matrix second){
    int count_rows    = first.matrix.size();
    int count_columns = second.matrix[0].size();
    assert(first.matrix.size() == second.matrix.size());
    assert(first.matrix[0].size() == second.matrix[0].size());


    Matrix result(count_rows, count_columns);
    for(int i  = 0; i < count_rows; i++) {   
        for(int g = 0; g < count_columns;g++){
            result.matrix[i][g] = 0;
            for( int k =0; k < first.matrix[0].size(); k++){
                result.matrix[i][g] += first.matrix[i][k] *  second.matrix[k][g];
            }
        }
    }
    return result;
}

Matrix operator+(Matrix first, Matrix second){
    int count_rows    = first.matrix.size();
    int count_columns = first.matrix[0].size();
    Matrix result(count_rows, count_columns);
    result.set_zero();
    for(auto row = 0; row < result.matrix.size(); row++) {
    for(auto column = 0; column < result.matrix[row].size(); column++) {
            result.matrix[row][column] =first.matrix[row][column] + second.matrix[row][column];
        }
    }
    return result;
}

Matrix operator-(Matrix first, Matrix second){
    int count_rows    = first.matrix.size();
    int count_columns = first.matrix[0].size();
    Matrix result(count_rows, count_columns);
    result.set_zero();
    for(auto row = 0; row < result.matrix.size(); row++) {
    for(auto column = 0; column < result.matrix[row].size(); column++) {
            result.matrix[row][column] =first.matrix[row][column] - second.matrix[row][column];
        }
    }
    return result;
}

void Matrix::shape(int new_size){
    while (matrix.size() < new_size)
    {
        matrix.push_back(std::vector<int>());
    }
    for(auto row = 0; row < matrix.size(); row++) {
        while (matrix[row].size() < new_size)
        {   
            matrix[row].push_back(0);
        }
    }
}
    