#pragma once

#include <vector>
#include <fstream>

class Matrix {
public:
    Matrix(int _M, int _N) : M(_M), N(_N) {
        firstMatrix.resize(_M);
        for(int i = 0; i < _M; i++)
            firstMatrix[i].resize(_N);
        secondMatrix.resize(_M);
        for(int i = 0; i < _M; i++)
            secondMatrix[i].resize(_N);
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
        for (int i = 0; i < matrix.M; i++) {
            for (int j = 0; j < matrix.N; j++)
                out << matrix.firstMatrix[i][j] << ' ';

            out << '\n';
        }

        out << '\n';

        return out;
    }

    void fillMatrix(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1,9);

        for(int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                firstMatrix[i][j] = dist(gen);
                secondMatrix[i][j] = dist(gen);
            }
        }
    }

    void sumMatrix(Matrix* A){
        for(int i = 0; i < A->M; i++){
            for(int j = 0; j < A->N; j++)
                this->firstMatrix[i][j] = A->firstMatrix[i][j] + A->secondMatrix[i][j];
        }
    }

private:
	std::vector<std::vector<int>> firstMatrix;
	std::vector<std::vector<int>> secondMatrix;
	int M, N;
};