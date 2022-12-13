#include <stdexcept>
#include <string>
#include "save_cl.h"

cl_int check_code(cl_int code, const char *file, int line) {
    if (code != CL_SUCCESS) {
        throw std::runtime_error(
                "ERROR OCCURRED! FILE:: " +
                std::string(file) +
                ", LINE: " +
                std::to_string(line) +
                ", CODE: " +
                std::to_string(code));
    }
    return code;
}
