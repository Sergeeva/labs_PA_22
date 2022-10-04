#ifndef THREADS_THREADS_H
#define THREADS_THREADS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <utility>

template<typename T>
using Matrix = std::vector<std::vector<T>>;

class Threads {
public:
    Threads(int, std::vector<char *>);
    ~Threads() = default;
    void run();

private:
    static bool reading(Matrix<double> *, Matrix<double> *, char**);
    static void readMatrix(Matrix<double> *, char *);
    static bool calculation(Matrix<double> *, Matrix<double> *, Matrix<double> *, int);
    static void calculateMatrix(Matrix<double> *, Matrix<double> *, Matrix<double> *, int, int);
    static bool writing(Matrix<double> *, char *);
    static void writeMatrix(Matrix<double> *, char *);

    char *inputFileName[2];
    char *outputFileName;
    Matrix<double> matrixA;
    Matrix<double> matrixB;
    Matrix<double> matrixC;
    int threadsNum;

};

#endif //THREADS_THREADS_H
