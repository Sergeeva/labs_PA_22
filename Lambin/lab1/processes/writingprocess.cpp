#include "writingprocess.h"

WritingProcess::WritingProcess(char *fileName) {
    sharedMemory = new SharedMemory(WRITE);
    outputFileName = fileName;
}

WritingProcess::~WritingProcess() {
    delete sharedMemory;
}

void WritingProcess::run() {
    if (!sharedMemory->checkMemory()) {
        std::cerr << "Shared memory error: unable to allocate memory\n";
        return;
    }

    sharedMemory->setProcessStatus(WRITE, RUN);
//    std::cout << "Writing process start\n";

    try {
        writeMatrix();
    } catch (std::runtime_error &exception) {
        sharedMemory->crash();
        std::cerr << exception.what() << '\n';
    }

    sharedMemory->setProcessStatus(WRITE, TERMINATE);
//    std::cout << "Writing process terminate\n";
}

void WritingProcess::writeMatrix() {
    auto output = [&] (std::ostream &out) -> int {
        for (int y = 0; y < sharedMemory->getMatrixHeight(A); y++) {
            while (y == sharedMemory->getMatrixHeight(C)) {
                std::cout << y << ' ' << sharedMemory->getMatrixHeight(C) << '\n';
                if (sharedMemory->isCrash()) {
                    return -1;
                }
            }
            for (int x = 0; x < sharedMemory->getMatrixWidth(A); x++) {
                out << sharedMemory->getValue(C, x, y) << ' ';
            }
            out << '\n';
        }
        return 0;
    };

    if (outputFileName != nullptr) {
        std::ofstream file(outputFileName);
        if (!file.is_open()) {
            sharedMemory->crash();
            throw std::runtime_error("Writing process error: invalid file");
        }
        output(file);
        file.close();
    } else {
        output(std::cout);
    }
}
