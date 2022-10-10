#ifndef THREADS_H
#define THREADS_H

#include "buffer.h"

class Threads {
public:
    Threads(int, char *);
    ~Threads();
    void run();

private:
    void generation();
    void calculation();
    void writing();

    char *outputFileName;
    Buffer<std::pair<Matrix, Matrix>> *inputBuffer;
    Buffer<Matrix> *outputBuffer;

};

#endif //THREADS_H
