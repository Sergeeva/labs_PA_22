#ifndef PROJECT_FILE_HANDLER_H
#define PROJECT_FILE_HANDLER_H

#include <string>
#include <fstream>
#include <filesystem>

#include "../../matrices/Matrix.h"

class FileHandler {
    const std::string file_path = "../../data/";

protected:
    std::string to_file(const std::string &name);

public:
    FileHandler() = default;

    FileHandler(std::string data);

    bool check_file(const std::string &name);

    void write(const Matrix &matrix, const std::string &name);

    Matrix read(const std::string &name);

};


#endif //PROJECT_FILE_HANDLER_H
