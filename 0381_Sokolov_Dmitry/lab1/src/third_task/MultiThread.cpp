#include "MultiThread.h"

void MultiThread::input(Matrix &A, Matrix &B, bool remake) {

    this->log_thread();

    A = this->data.provide_first(remake);
    B = this->data.provide_second(remake);

    if (logs) {
        std::cout << "Matrices loaded." << std::endl;
    }
}

void MultiThread::thread_input(Matrix& A, Matrix&  B, bool remake) {
    this->init = std::move(std::thread(std::bind(&MultiThread::input, this, std::ref(A), std::ref(B),
                                                 remake)));
}

void MultiThread::queue() {
    std::promise<Matrix> outcome;
    this->promised = std::move(outcome);
}

std::future<Matrix> MultiThread::get_queued() {
    return this->promised.get_future();
}

//void MultiThread::partial_sum(const Matrix &first, const Matrix &second, int start, int length, Matrix& result) {
//    int size = result.get_rows() * result.get_columns();
//
//    for (int i = start; i < start + length && i < size; ++i) {
//        int y = i / result.get_columns();
//        int x = i % result.get_columns();
//
//        result[y][x] = first.get(y, x) + second.get(y, x);
//    }
//
//}

void MultiThread::sum(Matrix &A, Matrix &B) {

    if (this->thread_number < PThreadsConfig::min_threads) {
        throw std::runtime_error("Too little threads to perform calculation: " + std::to_string(this->thread_number));
    }

    if (this->thread_number > PThreadsConfig::max_threads) {
        throw std::runtime_error("Thread limit exceeded! Number of threads: " + std::to_string(this->thread_number));
    }

    if (!A.check_dimensions(B)) {
        throw std::runtime_error("Matrix dimensions do not match!");
    }

    Matrix result = Matrix(A.get_rows(), A.get_columns());

    int size = result.get_rows() * result.get_columns();
    int per_thread = (size / this->thread_number) + (size % this->thread_number > 0 ? 1 : 0);

    std::vector<std::thread> execution;

    this->timer.start_session();

    for (int i = 0; i < size; i += per_thread) {
        std::thread thread(std::bind(&Matrix::partial_sum, std::ref(A), std::ref(B), i, per_thread,
                                     std::ref(result)));
        execution.emplace_back(std::move(thread));
    }

    for (auto &thread: execution) {
        thread.join();
    }

    this->queue();
    this->promised.set_value(result);

    this->timer.finish_session();

    if (logs) {
        std::cout << "Matrices summed" << std::endl;
    }

}

void MultiThread::output(Matrix &matrix) {
    this->log_thread();

    this->data.log(matrix, Config::first_plus_second);

    if (logs) {
        std::cout << "Result written to file" << std::endl;
    }
}

void MultiThread::thread_output() {
    auto result = this->get_queued().get();
    this->out = std::move(std::thread(std::bind(&MultiThread::output, this, result)));
}

void MultiThread::log_thread() {
    if (this->logs) {
        std::cout << "\n[Thread # ";
        std::cout << std::this_thread::get_id();
        std::cout << "]" << std::endl;
    }
}

void MultiThread::set_threads(int thread_number) {
    this->thread_number = thread_number;
}

void MultiThread::execute(bool remake) {
    auto A = Matrix();
    auto B = Matrix();

    this->thread_input(A, B, remake);
    this->init.join();

    this->sum(std::ref(A), std::ref(B));

    this->thread_output();

    this->out.join();
}

long long int MultiThread::timed_execute(bool remake) {
    this->execute(remake);
    return this->timer.last();
}

void MultiThread::log_mode(bool write_logs) {
    this->logs = write_logs;

    if (this->logs) {
        this->data.loud();
    }

    else {
        this->data.silence();
    }

}
