#ifndef PROJECT_DATAHANDLER_H
#define PROJECT_DATAHANDLER_H

#include <utility>

#include "services/file_handler.h"
#include "services/Generator.h"

//#include "../configuration/config.h"

class DataHandler {
    FileHandler file = FileHandler(Config::Data_path);
    Generator gen = Generator(Config::generation_ceiling);

    bool silent = false;

public:
    DataHandler() = default;

    explicit DataHandler(FileHandler file);
    explicit DataHandler(Generator gen);
    explicit DataHandler(FileHandler file, Generator gen);
    explicit DataHandler(bool silent_mode);

    void create_data(const std::string& name);

    Matrix provide_first(bool remake);
    Matrix provide_second(bool remake);

    void silence();
    void loud();

    void log(const Matrix& matrix, const std::string& file);
};


#endif //PROJECT_DATAHANDLER_H
