#include <sstream>

#include "ThreadLab.h"



int main(int argc, char* argv[])
{
    if (argc == 1 || argc > 2)
        return EXIT_FAILURE;

    std::istringstream iss(argv[1]);

    uint32_t count;

    if (!(iss >> count))
        return EXIT_FAILURE;

    startLab(count);

    return EXIT_SUCCESS;
}