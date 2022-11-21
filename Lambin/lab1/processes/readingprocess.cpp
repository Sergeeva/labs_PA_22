#include "readingprocess.h"

ReadingProcess::ReadingProcess(char *fileName1, char *fileName2) {
    sharedMemory = new SharedMemory(READ);
    inputFileName[0] = fileName1;
    inputFileName[1] = fileName2;
}

ReadingProcess::~ReadingProcess() {
    delete sharedMemory;
}

void ReadingProcess::run() {
    if (!sharedMemory->checkMemory()) {
        std::cerr << "Shared memory error: unable to allocate memory\n";
        return;
    }

    sharedMemory->setProcessStatus(READ, RUN);
//    std::cout << "Reading process start\n";

    try {
        readMatrix();
        readMatrix();
    } catch (std::runtime_error &exception) {
        sharedMemory->crash();
        std::cerr << exception.what() << '\n';
    }

    if (sharedMemory->getMatrixHeight(A) != sharedMemory->getMatrixHeight(B)
        || sharedMemory->getMatrixWidth(A) != sharedMemory->getMatrixWidth(B)) {
        sharedMemory->crash();
        throw std::runtime_error("Reading process error: different matrices\' sizes");
    }

    sharedMemory->setProcessStatus(READ, TERMINATE);
//    std::cout << "Reading process terminate\n";
}

void ReadingProcess::readMatrix() {
    std::ifstream file(inputFileName[fileCounter]);
    if (!file.is_open()) {
        sharedMemory->crash();
        throw std::runtime_error("Reading process error: invalid file");
    }

    int y = 0;
    double num;
    std::vector<double> line;
    size_t lineWidth = 0;
    while (file >> num) {
        if (line.size() == MAXSIZE) {
            sharedMemory->crash();
            throw std::runtime_error("Reading process error: input matrix sizes too large");
        }
        line.push_back(num);
        if (file.peek() == (int) '\n') {
            if (y == 0) {
                lineWidth = line.size();
            } else {
                if (line.size() != lineWidth) {
                    sharedMemory->crash();
                    throw std::runtime_error("Reading process error: input data is not a matrix");
                }
            }
            for (int x = 0; x < line.size(); x++) {
                sharedMemory->setValue(line[x], (matrix) fileCounter, x, y);
            }
            y++;
            line.clear();
        }
    }
    for (int x = 0; x < line.size(); x++) {
        sharedMemory->setValue(line[x], (matrix) fileCounter, x, y);
    }

    fileCounter++;
    file.close();
}
