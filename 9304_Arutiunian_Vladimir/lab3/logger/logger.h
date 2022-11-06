#ifndef LABS_PA_22_LAB3_LOGGER_H
#define LABS_PA_22_LAB3_LOGGER_H

#include <string>
#include <iostream>
#include <sstream>
#include <thread>

#define ENABLE_LOGS true

class Logger {
public:
    static void printThreadIdWithText(std::string_view text);

    static void printCPlusPlusVersion();
};


#endif //LABS_PA_22_LAB3_LOGGER_H
