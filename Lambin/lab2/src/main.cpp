#include "threads.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./lab2 <buffer_capacity> <output_file>\n";
        return 0;
    }

    int bufferCapacity = 0;
    for (int i = 0; argv[1][i] != '\0'; i++) {
        bufferCapacity = bufferCapacity * 10 + (argv[1][i] - '0');
    }

    Threads threads(bufferCapacity, argv[2]);
    threads.run();
    return 0;
}
