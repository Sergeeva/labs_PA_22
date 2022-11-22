#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Matrix.h"
using namespace std;

class Buffer{

private:

    int size_max;
    int size_curr;
    condition_variable consumed;
    condition_variable produced;
    std::mutex mutex;
    std::queue<Matrix> queue;
    
public:
    Buffer(int max_size);
    Matrix consume();
    void produce(Matrix& matrix);
};
