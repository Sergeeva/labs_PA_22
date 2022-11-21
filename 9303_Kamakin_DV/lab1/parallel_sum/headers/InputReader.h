#ifndef UNTITLED_INPUTREADER_H
#define UNTITLED_INPUTREADER_H

#include "Runnable.h"
#include "Configuration.h"
#include "MatricesHolder.h"

class InputReader : public Runnable {

private:

    MatricesHolder &holder;
    const std::string &file_name;

    void read() const;

    static void read_matrix(std::ifstream &source_file, Matrix &matrix);

public:

    InputReader(MatricesHolder &holder, const std::string &file_name) : holder(holder), file_name(file_name) {};

    void run() final;

};


#endif //UNTITLED_INPUTREADER_H
