#include "file_handler.h"

#include <utility>

FileHandler::FileHandler(std::string data) : file_path(std::move(data)) {}

std::string FileHandler::to_file(const std::string &name) {
    return this->file_path + name + ".txt";
}

bool FileHandler::check_file(const std::string &name) {

    return std::filesystem::exists(this->to_file(name));

}

void FileHandler::write(const Matrix &matrix, const std::string &name) {

    std::ofstream output;
    output.open(this->to_file(name), std::ofstream::out | std::ofstream::trunc);

    if (!output.is_open()) {
        throw std::ofstream::failure("Couldn't open file at {" + this->to_file(name) + "}!");
    }

    output << matrix;
    output.close();
}

Matrix FileHandler::read(const std::string &name) {

    std::ifstream input;
    input.open(this->to_file(name), std::ifstream::in);

    if (!input.is_open()) {
        throw std::ofstream::failure("Couldn't open file at {" + this->to_file(name) + "}!");
    }

    Matrix matrix = Matrix();
    input >> matrix;

    input.close();

    return matrix;
}

