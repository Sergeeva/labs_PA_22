#ifndef UNTITLED_INPUTREADER_H
#define UNTITLED_INPUTREADER_H

#include "Runnable.h"
#include "Configuration.h"
#include "MatricesHolder.h"
#include "SynchronizedQueue.h"

class InputReader : public Runnable {

private:

    SynchronizedQueue<MatricesHolder> &queue;

    const std::string &file_name;

    const int iterations_count;

    static void read_matrix(std::ifstream &source_file, Matrix &matrix);

public:

    InputReader(SynchronizedQueue<MatricesHolder> &queue,
                const std::string &file_name,
                const int iterations_count) : queue(queue),
                                              file_name(file_name),
                                              iterations_count(iterations_count) {};

    void run() final;

};


#endif //UNTITLED_INPUTREADER_H
