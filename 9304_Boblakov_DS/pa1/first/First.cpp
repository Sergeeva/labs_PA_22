#include <thread>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>


typedef std::vector<std::vector<int>> Matrix;

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

std::vector<std::vector<int>> generateMatrix(int m_width, int m_height) {
    std::vector<int> init_vec(m_height, 0);
    Matrix gen_matrix(m_width, init_vec);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(-500, 500);
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            gen_matrix[i][j] = distr(gen);
        }
    }
    return gen_matrix;
}

int getSharedMemoryId() {

    int shmID = shmget(2000, 1024, 0666 | IPC_CREAT);

    if (shmID == 1) {
        std::cout << "Error: something went wrong!";
        exit(EXIT_FAILURE);
    }
    return shmID;
}

void * getSharedMemoryPointer(int shmID) {
    void* shmPtr = shmat(shmID, nullptr, 0);
    if (shmPtr == (void *) -1) {
        std::cout << "Error: something went wrong!";
        exit(EXIT_FAILURE);
    }
    return shmPtr;
}

int * setMatrixToShM(int *shmPtr, Matrix &matrix) {
    *shmPtr = matrix.size();
    shmPtr++;

    *shmPtr = matrix[0].size();
    shmPtr++;

    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            *shmPtr = matrix[i][j];
            shmPtr++;
        }
    }
    return shmPtr;
}

int * getMatrixFromShM(int *shmPtr, Matrix &matrix) {
    int height = *shmPtr;
    shmPtr++;

    int width = *shmPtr;
    shmPtr++;

    matrix.resize(height);

    for (int i = 0; i < height; i++) {
        matrix[i].resize(width);
        for (int j = 0; j < width; j++) {
            matrix[i][j] = *shmPtr;
            shmPtr++;
        }
    }

    return shmPtr;
}

void * read(Matrix & m1, Matrix & m2, int width, int height){

    m1 = generateMatrix(width, height);
    m2 = generateMatrix(width, height);

    std::cout<<"\nMatrix filled\n";
    return nullptr;
}

void * write(Matrix & m, std::string & file_name){

    std::ofstream out;
    out.open(file_name, std::fstream::in | std::fstream::out );

    for (int i = 0; i < m.size(); ++i) {
        for (int j = 0; j < m[i].size(); ++j) {
            out << m[i][j]<<" ";
        }
        out <<"\n";
    }
    out.close();
    return nullptr;
}

void read_process() {
    int pid = fork();

    int m_width = 15;
    int m_height = 15;

    switch (pid) {
        case 0:{
            int sharedMemoryId = getSharedMemoryId();
            int *sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

            Matrix m1, m2;
            m1 = generateMatrix(m_width, m_height);
            m2 = generateMatrix(m_width, m_height);

            sharedMemoryPointer = setMatrixToShM(sharedMemoryPointer, m1);
            setMatrixToShM(sharedMemoryPointer, m2);


            std::cout << "1... DONE -- PID: " << getpid() << "\n";

            exit(EXIT_SUCCESS);
        }

        case -1:
            std::cout << "Error: something went wrong";
            exit(EXIT_FAILURE);
        default:
            wait(&pid);
    }
}

void sum_process() {
    int pid = fork();

    switch (pid) {
        case 0:{
            int sharedMemoryId = getSharedMemoryId();
            int *sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

            Matrix m1, m2;

            sharedMemoryPointer = getMatrixFromShM(sharedMemoryPointer, m1);
            getMatrixFromShM(sharedMemoryPointer, m2);

            int height = m1.size();
            int width = m1[0].size();
            Matrix res(height, std::vector<int>(width));

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    res[i][j] = m1[i][j] + m2[i][j];
                }
            }

            sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

            setMatrixToShM(sharedMemoryPointer, res);

            std::cout << "2... DONE -- PID: " << getpid() << "\n";

            exit(EXIT_SUCCESS);
        }

        case -1:
            std::cout << "Error: something went wrong";
            exit(EXIT_FAILURE);
        default:
            wait(&pid);
    }
}

void write_process() {
    int pid = fork();
    std::string file_name = "m3.txt";

    switch (pid) {
        case 0:{
            int sharedMemoryId = getSharedMemoryId();
            int *sharedMemoryPointer = (int*) getSharedMemoryPointer(sharedMemoryId);

            Matrix res;
            getMatrixFromShM(sharedMemoryPointer, res);

            write(res, file_name);
            std::cout << "3... DONE -- PID: " << getpid() << "\n";

            exit(EXIT_SUCCESS);
        }

        case -1:
            std::cout << "Error: something went wrong";
            exit(EXIT_FAILURE);
        default:
            wait(&pid);
    }
}

int main() {
    read_process();
    sum_process();
    write_process();

    return 0;
}