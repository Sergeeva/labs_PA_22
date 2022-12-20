#ifndef PA4_UTILS_H
#define PA4_UTILS_H

#include <future>
#include <thread>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>

typedef std::vector<std::vector<int>> Matrix;
typedef std::pair<Matrix, Matrix> pMatrix;

void printMatrix(Matrix matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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

std::vector<std::vector<int>> generateZeroM(int m_width, int m_height) {
    std::vector<int> init_vec(m_height, 0);
    Matrix gen_matrix(m_width, init_vec);
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            gen_matrix[i][j] = 0;
        }
    }
    return gen_matrix;
}


struct Node {

    const Matrix data;
    std::shared_ptr<Node> next = nullptr;

    explicit Node(Matrix data) : data(std::move(data)) {}

};

class CustomStack {

    std::shared_ptr<Node> head{};

public:

    void push(const Matrix &data) {
        auto newNode = std::make_shared<Node>(data);

        std::shared_ptr<Node> tmp{};
        do {
            tmp = head;
            newNode->next = tmp;
        }

        while (!std::atomic_compare_exchange_weak(&head, &tmp, newNode));
    }

    Matrix pop() {
        std::shared_ptr<Node> tmp{};

        do{
            tmp = head;
        }
        while (!tmp || !std::atomic_compare_exchange_weak(&head, &tmp, tmp->next));

        return tmp->data;
    }
};

#endif //PA4_UTILS_H
