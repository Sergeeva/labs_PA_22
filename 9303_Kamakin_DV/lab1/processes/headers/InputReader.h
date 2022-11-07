#ifndef UNTITLED_INPUTREADER_H
#define UNTITLED_INPUTREADER_H

#include "Runnable.h"
#include "Configuration.h"
#include "SharedMemory.h"

class InputReader : public Runnable {

private:

    void read_to(SharedMemory &shared_memory) const;

    template<class ElemConsumer>
    void read_matrix(std::ifstream &source_file, const ElemConsumer &consumer) const;

public:
    void run() final;

};


#endif //UNTITLED_INPUTREADER_H
