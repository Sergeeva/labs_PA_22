#include "DataHandler.h"

DataHandler::DataHandler(FileHandler _file): file(std::move(_file)) {}

DataHandler::DataHandler(Generator _gen): gen(_gen){}

DataHandler::DataHandler(bool silent_mode) : silent(silent_mode) {}

DataHandler::DataHandler(FileHandler _file, Generator _gen): file(std::move(_file)), gen(_gen) {}

void DataHandler::log(const Matrix &matrix, const std::string& _file) {
    this->file.write(matrix, _file);
}

Matrix DataHandler::provide_first(bool remake) {
    if (!this->file.check_file(Config::first) || remake) {
        this->create_data(Config::first);

    }

    return this->file.read(Config::first);

}

Matrix DataHandler::provide_second(bool remake) {
    if (!this->file.check_file(Config::second) || remake) {
        this->create_data(Config::second);
    }

    return this->file.read(Config::second);

}

void DataHandler::create_data(const std::string& name) {
    if (name == Config::first) {
        this->file.write(Matrix(this->gen.generate(Config::R, Config::C, Config::first_seed)), Config::first);

        if (!this->silent) {
            std::cout << "First matrix generated" << std::endl;
        }

        return;
    }

    this->file.write(Matrix(this->gen.generate(Config::R, Config::C, Config::second_seed)), Config::second);

    if (!this->silent) {
        std::cout << "Second matrix generated" << std::endl;
    }

}

void DataHandler::silence() {
    this->silent = true;
}

void DataHandler::loud() {
    this->silent = false;
}
