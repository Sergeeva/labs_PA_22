#ifndef UNTITLED_INPUTREADER_H
#define UNTITLED_INPUTREADER_H

#include "Runnable.h"
#include "Configuration.h"
#include "MatricesHolder.h"

class InputReader : public Runnable {

private:

    MatricesHolder &holder;

    void read() const;

    static void read_matrix(std::ifstream &source_file, Matrix &matrix) ;

public:

    InputReader(MatricesHolder &holder) : holder(holder) {};

    void run() final;

};


#endif //UNTITLED_INPUTREADER_H
