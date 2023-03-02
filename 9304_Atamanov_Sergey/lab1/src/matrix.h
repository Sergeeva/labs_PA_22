#ifndef PA_MATRIX_H
#define PA_MATRIX_H

#include <vector>
#include <random>
#include <string>
#include <fstream>

class Matrix {
public:
    Matrix(int _M, int _N, int _offsetM = 0, int _offsetN = 0) : M(_M), N(_N), offsetM(_offsetM), offsetN(_offsetN){
        matrix.reserve(this->M);

        for(int i = 0; i < M; i++)
            matrix[i].reserve(this->N);
    }

    void fillMatrix(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1,9);

        for(int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                matrix[i][j] = dist(gen);
    }

    void sumMatrix(Matrix& A, Matrix& B, int tempThread, int T, int step, int rem){
        for(int i = tempThread * (A.M / T); i < (tempThread + 1) * (A.M / T); i++){
            for(int j = 0; j < A.N; j++)
                this->matrix[i][j] = A.matrix[i][j] + B.matrix[i][j];
        }
        /*for(int i = step; i < rem; ++i){
            for(int j = 0; j < A.matrix.at(0).size(); ++j){
                matrix.at(i).emplace_back(A.matrix.at(i).at(j) + B.matrix.at(i).at(j));
            }
        }*/
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix){
        os << "{\n";
        for(int i = 0; i < matrix.M - matrix.offsetM; i++) {
            os << "\t\"" << i << "\" : \"";
            for (int j = 0; j < matrix.N-1 - matrix.offsetN; j++)
                os << matrix.matrix[i][j] << ",";
            if(i < matrix.M-1)
                os << matrix.matrix[i][matrix.N-1 - matrix.offsetN] << "\",\n";
            else
                os << matrix.matrix[i][matrix.N-1 - matrix.offsetN] << "\"\n";
        }
        os << "}";
        return os;
    }

    friend std::fstream& operator>>(std::fstream& is, Matrix& matrix){
        std::string tempLine;
        int countN;
        std::string tempString;
        std::getline(is, tempLine);
        for(int i = 0; i < matrix.M; i++) {
            tempString.clear();
            std::getline(is, tempLine);
            auto strBegin = tempLine.begin();
            strBegin += 2;
            countN = 0;
            while(*strBegin != '\"'){
                tempString += *strBegin;
                strBegin++;
            }
            strBegin++;
            countN = std::stoi(tempString);
            tempString.clear();
            strBegin += 4;
            countN = 0;
            while(*strBegin != '\"'){
                if(*strBegin == ','){
                    matrix.matrix[i][countN] = std::stoi(tempString);
                    tempString.clear();
                    countN++;
                    strBegin++;
                }

                tempString += *strBegin;
                strBegin++;
            }
            matrix.matrix[i][countN] = std::stoi(tempString);
            tempString.clear();
        }


        return is;
    }

    friend Matrix operator+(const Matrix& A, const Matrix& B){
        Matrix result(A.M, A.N, A.offsetM, A.offsetN);

        for(int i = 0; i < A.M; i++){
            for(int j = 0; j < A.N; j++)
                result.matrix[i][j] = A.matrix[i][j] + B.matrix[i][j];
        }

        return result;
    }

    int M;
    int N;
    int offsetM;
    int offsetN;
    std::vector<std::vector<int>> matrix;
};

#endif //PA_MATRIX_H
