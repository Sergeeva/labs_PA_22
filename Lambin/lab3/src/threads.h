#ifndef THREADS_H
#define THREADS_H

#include "buffer.h"

class Threads {
public:
    Threads(int, char *);
    ~Threads() = default;
    void run();

private:
    void generation();
    void calculation();
    void writing();

    int matrixAmount;
    char *outputFileName;
    std::unique_ptr<Buffer<std::pair<Matrix, Matrix>>> inputBuffer;
    std::unique_ptr<Buffer<Matrix>> outputBuffer;

};

#endif //THREADS_H
