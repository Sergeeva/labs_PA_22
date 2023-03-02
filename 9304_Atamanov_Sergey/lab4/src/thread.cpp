#include "thread.h"

int M = 1000;
int N = 1000;

void startLab4() {

    Matrix firstMatrix(M, N);
    Matrix secondMatrix(M, N);
    Matrix thirdMatrix(M, N, true);
    Matrix shtrassenMatrix(M, N);
    Matrix matrix(M, N);

    std::mutex mutex;

    auto beginOne = std::chrono::steady_clock::now();
    referenceMatrix(firstMatrix, secondMatrix, matrix);
    auto endOne = std::chrono::steady_clock::now();
    auto durationOne = std::chrono::duration_cast<std::chrono::milliseconds>(endOne - beginOne);

    auto beginUsual = std::chrono::steady_clock::now();
    auto *threads = new std::thread[8];
    for (int i = 0; i < 8; i++)
        threads[i] = std::thread(multiplyMatrix, std::ref(firstMatrix), std::ref(secondMatrix), std::ref(thirdMatrix), i);

    for (int i = 0; i < 8; i++)
        threads[i].join();

    auto endUsual = std::chrono::steady_clock::now();
    auto durationUsual = std::chrono::duration_cast<std::chrono::milliseconds>(endUsual - beginUsual);

    auto beginShtrassen = std::chrono::steady_clock::now();
    std::vector<Matrix *> As;
    std::vector<Matrix *> Bs;
    std::vector<Matrix *> Cs;
    std::vector<Matrix *> Ms;
    std::atomic<int> count = 0;

    As.reserve(4);
    Bs.reserve(4);
    Cs.reserve(4);
    Ms.reserve(8);

    for (int i = 0; i < 4; i++) {
        As[i] = new Matrix(M / 2, N / 2);
        Bs[i] = new Matrix(M / 2, N / 2);
        Cs[i] = new Matrix(M / 2, N / 2);
    }

    for (int i = 0; i < 7; i++)
        Ms[i] = new Matrix(M / 2, N / 2);

    distributeMatrices(As, firstMatrix);
    distributeMatrices(Bs, secondMatrix);

    auto *threadsShtrassen = new std::thread[8];
    for (int i = 0; i < 8; i++)
        threadsShtrassen[i] = std::thread(multiplyShtrassenMatrix, std::ref(As), std::ref(Bs),
                                          std::ref(Ms), i);

    for (int i = 0; i < 8; i++)
        threadsShtrassen[i].join();

    *(Cs[0]) = *(Ms[0]) + *(Ms[3]) - *(Ms[4]) + *(Ms[6]);
    *(Cs[1]) = *(Ms[2]) + *(Ms[4]);
    *(Cs[2]) = *(Ms[1]) + *(Ms[3]);
    *(Cs[3]) = *(Ms[0]) - *(Ms[1]) + *(Ms[2]) + *(Ms[5]);

    auto endShtrassen = std::chrono::steady_clock::now();
    auto durationShtrassen = std::chrono::duration_cast<std::chrono::milliseconds>(endShtrassen - beginShtrassen);

    collectMatrix(Cs, shtrassenMatrix);

    std::cout << "One thread time - " << durationOne.count() << ", Usual algorithm time - " << durationUsual.count() << ", Shtrassen time - " << durationShtrassen.count() << "\n";

    if (thirdMatrix == matrix && shtrassenMatrix == matrix)
        std::cout << "Right answer\n";
    else
        std::cout << "Wrong answer\n";

    std::fstream output;
    output.open("outputThread.txt", std::ios_base::out);

    output << "Reference answer:\n" << matrix;
    output << "MultiThread answer:\n" << thirdMatrix;
    output << "Shtrassen answer:\n" << shtrassenMatrix;

}

void referenceMatrix(Matrix A, Matrix B, Matrix& C){
    for(int i = 0; i < A.M; i++) {
        for (int j = 0; j < A.N; j++) {
            C.matrix[i][j] = 0;
            for (int k = 0; k < A.N; k++) {
                C.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];
            }
        }
    }
}

void multiplyMatrix(Matrix& A, Matrix& B, Matrix& C, int n) {

    for(int i = n; i < A.M; i += 8) {
        for (int j = 0; j < A.N; j++) {
            C.matrix[i][j] = 0;
            for (int k = 0; k < A.N; k++) {
                C.matrix[i][j] += A.matrix[i][k] * B.matrix[k][j];
            }
        }
    }
}

void multiplyShtrassenMatrix(std::vector<Matrix*>& As, std::vector<Matrix*>& Bs, std::vector<Matrix*>& Ms, int thread){

    switch (thread) {
        case 0:
            referenceMatrix(*(As[0]) + *(As[3]), *(Bs[0]) + *(Bs[3]), *(Ms[0]));
            break;
        case 1:
            referenceMatrix(*(As[2]) + *(As[3]), *(Bs[0]), *(Ms[1]));
            break;
        case 2:
            referenceMatrix(*(As[0]), *(Bs[1]) - *(Bs[3]), *(Ms[2]));
            break;
        case 3:
            referenceMatrix(*(As[3]), *(Bs[2]) - *(Bs[0]), *(Ms[3]));
            break;
        case 4:
            referenceMatrix(*(As[0]) + *(As[1]), *(Bs[3]), *(Ms[4]));
            break;
        case 5:
            referenceMatrix(*(As[2]) - *(As[0]), *(Bs[0]) + *(Bs[1]), *(Ms[5]));
            break;
        case 6:
            referenceMatrix(*(As[1]) - *(As[3]), *(Bs[2]) + *(Bs[3]), *(Ms[6]));
            break;
    }
}

void distributeMatrices(std::vector<Matrix*>& matrixs, Matrix& matrix){

    for(int i = 0; i < matrix.M / 2; i++)
        for(int j = 0; j < matrix.N / 2; j++)
            matrixs[0]->matrix[i][j] = matrix.matrix[i][j];

    for(int i = matrix.M / 2; i < matrix.M; i++)
        for(int j = 0; j < matrix.N / 2; j++)
            matrixs[2]->matrix[i - matrix.M / 2][j] = matrix.matrix[i][j];

    for(int i = 0; i < matrix.M / 2; i++)
        for(int j = matrix.N / 2; j < matrix.N; j++)
            matrixs[1]->matrix[i][j - matrix.N / 2] = matrix.matrix[i][j];

    for(int i = matrix.M / 2; i < matrix.M; i++)
        for(int j = matrix.N / 2; j < matrix.N; j++)
            matrixs[3]->matrix[i - matrix.M / 2][j - matrix.N / 2] = matrix.matrix[i][j];
}

void collectMatrix(std::vector<Matrix*>& matrixs, Matrix& matrix){
    for(int i = 0; i < matrix.M / 2; i++)
        for(int j = 0; j < matrix.N / 2; j++)
    matrix.matrix[i][j] = matrixs[0]->matrix[i][j];

    for(int i = matrix.M / 2; i < matrix.M; i++)
        for(int j = 0; j < matrix.N / 2; j++)
            matrix.matrix[i][j] = matrixs[2]->matrix[i - matrix.M / 2][j];

    for(int i = 0; i < matrix.M / 2; i++)
        for(int j = matrix.N / 2; j < matrix.N; j++)
            matrix.matrix[i][j] = matrixs[1]->matrix[i][j - matrix.N / 2];

    for(int i = matrix.M / 2; i < matrix.M; i++)
        for(int j = matrix.N / 2; j < matrix.N; j++)
            matrix.matrix[i][j] = matrixs[3]->matrix[i - matrix.M / 2][j - matrix.N / 2];
}