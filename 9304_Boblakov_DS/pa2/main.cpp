#include <condition_variable>
#include "utils.h"


std::mutex mutex1;
std::mutex mutex2;

std::condition_variable buf1_is_full, buf1_is_empty;
std::condition_variable buf2_is_full, buf2_is_empty;

std::vector<std::pair<Matrix, Matrix>> buf1;
std::vector<Matrix> buf2;

const int buf_size = 5;
const int m_width = 1000;
const int m_height = 1000;
const int counter_of_threads_triplet =4;

Matrix sum(std::pair<Matrix, Matrix> &matricesForSum) {
    Matrix m1 = matricesForSum.first;
    Matrix m2 = matricesForSum.second;

    Matrix res_m(m1.size(), std::vector<int>(m1[0].size()));

    for (int i = 0; i < m1.size(); i++) {
        for (int j = 0; j < m1[i].size(); j++) {
            res_m[i][j] = m1[i][j] + m2[i][j];
        }
    }

    return res_m;
}

void consume_m2() {

    std::unique_lock<std::mutex> ul(mutex2);

    buf2_is_empty.wait(ul, []() {
        return buf2.size() > 0;
    });

    Matrix res_m = buf2.back();
    buf2.pop_back();

    printMatrix(res_m);
    buf2_is_full.notify_one();
    ul.unlock();


}

void produce_m2(std::pair<Matrix, Matrix> &matricesForSum) {
    Matrix res_m = sum(matricesForSum);

    std::unique_lock<std::mutex> ul(mutex2);

    buf2_is_full.wait(ul, []() {
        return buf2.size() != buf_size;
    });

    buf2.push_back(res_m);
    buf2_is_empty.notify_one();
    ul.unlock();


}

void consume_m1() {

    std::unique_lock<std::mutex> ul(mutex1);

    buf1_is_empty.wait(ul, []() {
        return buf1.size() > 0;
    });

    std::pair<Matrix, Matrix> pair_of_matrix = buf1.back();
    buf1.pop_back();

    ul.unlock();

    produce_m2(pair_of_matrix);

    buf1_is_full.notify_one();

}


void produce_m1() {

    Matrix m1 = generateMatrix(m_width, m_height);
    Matrix m2 = generateMatrix(m_width, m_height);
    std::pair<Matrix, Matrix> pair_of_matrix;
    pair_of_matrix.first = m1;
    pair_of_matrix.second = m2;

    std::unique_lock<std::mutex> ul(mutex1);

    buf1_is_full.wait(ul, []() {
        return buf1.size() != buf_size;
    });

    buf1.push_back(pair_of_matrix);
    ul.unlock();

    buf1_is_empty.notify_one();
}


int main() {
    buf1.reserve(buf_size);
    buf2.reserve(buf_size);
    std::vector<std::thread> threads;
    threads.reserve(counter_of_threads_triplet * 3);
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < counter_of_threads_triplet; i++) {

        std::thread producerThread(produce_m1);
        threads.push_back(std::move(producerThread));

        std::thread originConsumerThread(consume_m1);
        threads.push_back(std::move(originConsumerThread));

        std::thread resultConsumerThread(consume_m2);
        threads.push_back(std::move(resultConsumerThread));
    }

    for (auto &thread: threads) {
        thread.join();
    }
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Elapsed time: " << (double) duration.count() / 1000000 << "\n";

    return 0;
}