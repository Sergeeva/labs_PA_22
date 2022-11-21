#include "ThreadHandler.h"

#include <memory>

void ThreadHandler::log_thread() {
    std::cout << "\n[Thread # ";
    std::cout << std::this_thread::get_id();
    std::cout << "]" << std::endl;
}

void ThreadHandler::input(Matrix &A, Matrix &B, bool remake) {

    this->log_thread();

    A = this->data.provide_first(remake);
    B = this->data.provide_second(remake);

    std::cout << "Matrices loaded." << std::endl;
}

void ThreadHandler::sum(Matrix &A, Matrix &B) {

    this->log_thread();

    Matrix summed = A + B;

    this->promised.set_value(summed);

    std::cout << "Matrices summed" << std::endl;
}

void ThreadHandler::output(Matrix &matrix) {
    this->log_thread();

    this->data.log(matrix, Config::first_plus_second);

    std::cout << "Result written to file" << std::endl;
}

void ThreadHandler::queue() {
    std::promise<Matrix> outcome;
    this->promised = std::move(outcome);
}

Matrix ThreadHandler::get_queued() {
    return this->promised.get_future().get();
}

void ThreadHandler::thread_input(Matrix& A, Matrix&  B, bool remake) {
    this->init = std::move(std::thread(std::bind(&ThreadHandler::input, this, std::ref(A), std::ref(B),
                                                          remake)));
}

void ThreadHandler::thread_sum(Matrix& A, Matrix & B) {
    this->queue();

    this->summator = std::move(std::thread(std::bind(&ThreadHandler::sum, this, std::ref(A), std::ref(B))));

}

void ThreadHandler::thread_output() {
    auto result = this->get_queued();
    this->print = std::move(std::thread(std::bind(&ThreadHandler::output, this, result)));
}

void ThreadHandler::execute(bool remake) {
    auto A = Matrix();
    auto B = Matrix();

    this->thread_input(A, B, remake);
    this->init.join();

    this->thread_sum(std::ref(A), std::ref(B));

    this->thread_output();

    this->summator.join();
    this->print.join();

}
