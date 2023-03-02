#pragma once

#include <vector>
#include <random>
#include <iostream>

class Matrix {
public:
    Matrix(int _M, int _N, bool flag = false) : M(_M), N(_N){
        matrix.resize(this->M);

        for(int i = 0; i < M; i++)
            matrix[i].resize(this->N);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1,9);

        if(!flag) {
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    matrix[i][j] = dist(gen);
        }
        else{
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    matrix[i][j] = 0;
        }
    }

	friend std::ostream& operator<<(std::ostream& out, Matrix& matrix) {
        for (int i = 0; i < matrix.M; i++) {
            for (int j = 0; j < matrix.N; j++)
                out << matrix.matrix[i][j] << ' ';

            out << '\n';
        }

        out << '\n';

        return out;

	}

	friend Matrix operator+(const Matrix& left, const Matrix& right) {
		Matrix result(left.M, left.N);

		for (int i = 0; i < result.M; i++)
			for (int j = 0; j < result.M; j++)
				result.matrix[i][j] = left.matrix[i][j] + right.matrix[i][j];

		return result;
	}

	friend Matrix operator-(const Matrix& left, const Matrix& right) {
		Matrix result(left.M, left.N);

		for (int i = 0; i < result.M; i++)
			for (int j = 0; j < result.M; j++)
				result.matrix[i][j] = left.matrix[i][j] - right.matrix[i][j];

		return result;
	}

	friend Matrix operator*(const Matrix& left, const Matrix& right) {
		Matrix result(left.M, left.N);
		for (int i = 0; i < left.M; i++)
			for (int j = 0; j < left.M; j++)
				for (int k = 0; k < left.M; k++)
					result.matrix[i][j] += left.matrix[i][k] * right.matrix[k][j];

		return result;
	}

    friend bool operator==(const Matrix& left, const Matrix& right){
        bool flag = true;
        for(int i = 0; i < left.M; i++)
            for(int j = 0; j < left.N; j++){
                if(left.matrix[i][j] != right.matrix[i][j])
                    flag = false;
            }

        return flag;
    }

    int M;
    int N;
    std::vector<std::vector<int>> matrix;
};