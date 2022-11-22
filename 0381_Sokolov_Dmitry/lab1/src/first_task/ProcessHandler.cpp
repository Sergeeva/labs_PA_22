#include "ProcessHandler.h"

ProcessHandler::ProcessHandler() = default;

void ProcessHandler::process(const std::function<void()> &func) {
    auto pid = fork();

    switch (pid) {
        case 0:

            std::cout << "\n[Process #";
            std:: cout << getpid();
            std::cout << "]" <<std::endl;

            func();

            exit(0);

        case -1:
            std::cout << "Got error while forking process";
            break;

        default:
            wait(&pid);
    }
}

void ProcessHandler::input() {
    Matrix A = data.provide_first(this->renew);
    Matrix B = data.provide_second(this->renew);

    std::cout << "Matrices loaded." << std::endl;

    auto after_A = storage.write_to_shared(A);
    storage.write_to_shared(B, after_A);

    std::cout << "Matrices written to memory" << std::endl;
}

void ProcessHandler::sum() {

    auto A_ptr = storage.read_from_shared();
    auto const &A = A_ptr.first;

    auto B_ptr = storage.read_from_shared(A_ptr.second);
    auto const &B = B_ptr.first;

    std::cout << "matrices loaded from memory" << std::endl;

    auto result = A + B;
    storage.write_to_shared(result);

    std::cout << "Matrices summed" << std::endl;
}

void ProcessHandler::output() {
    auto result = storage.read_from_shared();
    auto const &summed = result.first;

    this->data.log(summed, Config::first_plus_second);

    std::cout << "Result written to file" << std::endl;
}

void ProcessHandler::run(bool remake) {

    this->renew = remake;

    this->process(this->get_func(0));
    this->process(this->get_func(1));
    this->process(this->get_func(2));

    this->renew = Config::remake;

}

std::function<void(void)> ProcessHandler::get_func(int id) {
    switch (id) {
        case 0:
            return [this] { input(); };

        case 1:
            return [this] { sum(); };

        default:
            return [this] {output(); };

    }
}
