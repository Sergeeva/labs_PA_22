#include "threads.h"

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        std::cout << "Usage:"
            << "\tprocesses <input_file_1> <input_file_2>"
            << "\tprocesses <input_file_1> <input_file_2> <output_file>"
            << "\n";
        return -1;
    }

    Threads threads({argv[1], argv[2], argc == 4 ? argv[3] : nullptr});
    threads.run();
    return 0;
}
