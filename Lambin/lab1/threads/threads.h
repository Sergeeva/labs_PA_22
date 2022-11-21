#ifndef THREADS_THREADS_H
#define THREADS_THREADS_H

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

enum status {
    START,
    READY,
    RUN,
    TERMINATE
};

template<typename T>
using Matrix = std::vector<std::vector<T>>;

class Threads {
public:
    Threads(std::vector<char *>);
    ~Threads() = default;
    void run();

private:
    static void reading(Matrix<double> *, Matrix<double> *, char**, status *, bool *);
    static void readMatrix(Matrix<double> *, char *);
    static void calculation(Matrix<double> *, Matrix<double> *, Matrix<double> *, status *, status *, bool *);
    static void calculateMatrix(Matrix<double> *, Matrix<double> *, Matrix<double> *, bool *);
    static void writing(Matrix<double> *, char *, status *, status *, bool *);
    static void writeMatrix(Matrix<double> *, char *, status *, bool *);

    char *inputFileName[2];
    char *outputFileName;
    Matrix<double> matrixA;
    Matrix<double> matrixB;
    Matrix<double> matrixC;
    status readingStatus;
    status calculationStatus;
    status writingStatus;
    bool isCrash;

};

#endif //THREADS_THREADS_H
