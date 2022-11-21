#include "threads.h"

Threads::Threads(int threads, std::vector<char *> filesNames) {
    threadsNum = threads;
    inputFileName[0] = filesNames[0];
    inputFileName[1] = filesNames[1];
    outputFileName = filesNames[2];
}

void Threads::run() {
    bool isCrash = false;

    std::thread readingThread([&] () -> void {
        isCrash = !Threads::reading(&matrixA, &matrixB, inputFileName);
    });
    readingThread.join();

    if (!isCrash) {
        std::thread calculationThread([&] () -> void {
            isCrash = !Threads::calculation(&matrixA, &matrixB, &matrixC, threadsNum);
        });
        calculationThread.join();
    }

    if (!isCrash) {
        std::thread writingThread([&] () -> void {
            isCrash = !Threads::writing(&matrixC, outputFileName);
        });
        writingThread.join();
    }

}

bool Threads::reading(Matrix<double> *matrixA, Matrix<double> *matrixB, char *fileName[2]) {
    matrixA->reserve(1000);
    matrixB->reserve(1000);
    try {
        readMatrix(matrixA, fileName[0]);
        readMatrix(matrixB, fileName[1]);

        if (matrixA->size() != matrixB->size() || (*matrixA)[0].size() != (*matrixB)[0].size()) {
            throw std::runtime_error("Reading thread error: different matrices\' sizes");
        }
    } catch (std::runtime_error &exception) {
        std::cerr << exception.what() << '\n';
        return false;
    }
    return true;
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

bool Threads::calculation(Matrix<double> *matrixA, Matrix<double> *matrixB, Matrix<double> *matrixC, int threadsNum) {
    auto fragmentation = [] (int size, int amount, int index) -> std::pair<int, int> {
        int start, end;
        if (index < size % amount) {
            start = (size / amount + 1) * index;
            end = start + (size / amount + 1);
        } else {
            start = (size / amount + 1) * (size % amount) + (size / amount) * (index - size % amount);
            end = start + (size / amount);
        }
        return std::make_pair(start, end);
    };

    try {
        matrixC->resize(matrixA->size());
        int index = 0;
        std::vector<std::thread> threads(threadsNum);
        for (int i = 0; i < threadsNum; i++) {
            threads[i] = std::thread([&] () -> void {
                auto [start, end] = fragmentation(matrixA->size(), threadsNum, index);
                std::mutex indexMutex;
                indexMutex.lock();
                index++;
                indexMutex.unlock();
                calculateMatrix(matrixA, matrixB, matrixC, start, end);
            });
        }

        for (auto &thread : threads) {
            thread.join();
        }
    } catch (std::runtime_error &exception) {
        std::cerr << exception.what() << '\n';
        return false;
    }
    return true;
}

void Threads::calculateMatrix(Matrix<double> *matrixA, Matrix<double> *matrixB, Matrix<double> *matrixC, int start, int end) {
    std::vector<double> line;
    for (int y = start; y < end; y++) {
        for (int x = 0; x < (*matrixA)[0].size(); x++) {
            line.push_back((*matrixA)[y][x] + (*matrixB)[y][x]);
        }
        (*matrixC)[y].resize(line.size());
        for (int x = 0; x < line.size(); x++) {
            (*matrixC)[y][x] = line[x];
        }
        line.clear();
    }
}

bool Threads::writing(Matrix<double> *matrixC, char *fileName) {
    try {
        writeMatrix(matrixC, fileName);
    } catch (std::runtime_error &exception) {
        std::cerr << exception.what() << '\n';
        return false;
    }
    return true;
}

void Threads::writeMatrix(Matrix<double> *matrix, char *fileName) {
    auto output = [&] (std::ostream &out) -> void {
        for (int y = 0; y < matrix->size(); y++) {
            for (int x = 0; x < (*matrix)[y].size(); x++) {
                out << (*matrix)[y][x] << ' ';
            }
            out << '\n';
        }
    };

    if (fileName != nullptr) {
        std::ofstream file(fileName);
        if (!file.is_open()) {
            throw std::runtime_error("Writing thread error: invalid file");
        }
        output(file);
        file.close();
    } else {
        output(std::cout);
    }
}
