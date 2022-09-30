#include "threads.h"

Threads::Threads(std::vector<char *> filesNames) {
    inputFileName[0] = filesNames[0];
    inputFileName[1] = filesNames[1];
    outputFileName = filesNames[2];
    readingStatus = START;
    calculationStatus = START;
    writingStatus = START;
    isCrash = false;
}

void Threads::run() {
    std::thread readingThread(Threads::reading,
        &matrixA, &matrixB, inputFileName, &readingStatus, &isCrash);
    std::thread calculationThread(Threads::calculation,
        &matrixA, &matrixB, &matrixC, &readingStatus, &calculationStatus, &isCrash);
    std::thread writingThread(Threads::writing,
        &matrixC, outputFileName, &calculationStatus, &writingStatus, &isCrash);

    readingThread.join();
    calculationThread.join();
    writingThread.join();

}

void Threads::reading(Matrix<double> *matrixA, Matrix<double> *matrixB,
                      char *fileName[2], status *readingStatus, bool *isCrash) {
    *readingStatus = RUN;

    try {
        readMatrix(matrixA, fileName[0]);
        readMatrix(matrixB, fileName[1]);

        if (matrixA->size() != matrixB->size() || (*matrixA)[0].size() != (*matrixB)[0].size()) {
            throw std::runtime_error("Reading thread error: different matrices\' sizes");
        }
    } catch (std::runtime_error &exception) {
        *isCrash = true;
        std::cerr << exception.what() << '\n';
    }

    *readingStatus = TERMINATE;
}

void Threads::readMatrix(Matrix<double> *matrix, char *fileName) {
    std::vector<double> line;
    size_t lineWidth = 0;

    auto matrixCheck = [&] () -> bool {
        if (matrix->empty()) {
            lineWidth = line.size();
        } else {
            if (line.size() != lineWidth) {
                return false;
            }
        }
        return true;
    };

    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Reading thread error: invalid file");
    }

    double num;
    while(file >> num) {
        line.push_back(num);
        if (file.peek() == (int) '\n') {
            if (!matrixCheck()) {
                throw std::runtime_error("Reading thread error: input data is not a matrix");
            }
            matrix->push_back(line);
            line.clear();
        }
    }
    if (!matrixCheck()) {
        throw std::runtime_error("Reading thread error: input data is not a matrix");
    }
    matrix->push_back(line);

    file.close();
}

void Threads::calculation(Matrix<double> *matrixA, Matrix<double> *matrixB, Matrix<double> *matrixC,
                          status *readingStatus, status *calculationStatus, bool *isCrash) {
    *calculationStatus = RUN;

    try {
        while (*readingStatus < RUN) {
            if (*isCrash) {
                return;
            }
        }
        calculateMatrix(matrixA, matrixB, matrixC, isCrash);
    } catch (std::runtime_error &exception) {
        *isCrash = true;
        std::cerr << exception.what() << '\n';
    }

    *calculationStatus = TERMINATE;
}

void Threads::calculateMatrix(Matrix<double> *matrixA, Matrix<double> *matrixB, Matrix<double> *matrixC, bool *isCrash) {
    while (matrixB->empty()) {
        if (*isCrash) {
            return;
        }
    }

    std::vector<double> line;
    for (int y = 0; y < matrixA->size(); y++) {
        while (y == matrixB->size()) {
            if (*isCrash) {
                return;
            }
        }
        for (int x = 0; x < (*matrixA)[0].size(); x++) {
            line.push_back((*matrixA)[y][x] + (*matrixB)[y][x]);
        }
        matrixC->push_back(line);
        line.clear();
    }
}

void Threads::writing(Matrix<double> *matrixC,
                      char *fileName, status *calculationStatus, status *writingStatus, bool *isCrash) {
    *writingStatus = RUN;

    try {
        writeMatrix(matrixC, fileName, calculationStatus, isCrash);
    } catch (std::runtime_error &exception) {
        *isCrash = true;
        std::cerr << exception.what() << '\n';
    }

    *writingStatus = TERMINATE;
}

void Threads::writeMatrix(Matrix<double> *matrix, char *fileName, status *calculationStatus, bool *isCrash) {
    auto output = [&] (std::ostream &out) -> void {
        for (int y = 0; *calculationStatus != TERMINATE || matrix->size() > y; y++) {
            while (y == matrix->size()) {
                if (*isCrash) {
                    return;
                }
            }
            for (int x = 0; x < (*matrix)[0].size(); x++) {
                out << (*matrix)[y][x] << ' ';
            }
            out << '\n';
        }
    };

    if (fileName != nullptr) {
        std::ofstream file(fileName);
        if (!file.is_open()) {
            *isCrash = true;
            throw std::runtime_error("Writing thread error: invalid file");
        }
        output(file);
        file.close();
    } else {
        output(std::cout);
    }
}
