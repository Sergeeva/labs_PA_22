#pragma once

#include <unistd.h>
#include <string>
#include <iostream>
#include <thread>

namespace ForkPids {
    const pid_t child = 0;
    const pid_t error = -1;
}

namespace Filenames {
    const auto inputFilename = "input.txt";
    const auto outputFilename = "output.txt";
}

void log(const std::string &text);
