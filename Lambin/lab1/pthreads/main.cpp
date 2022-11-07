#include "threads.h"

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        std::cout << "Usage:"
            << "\tprocesses <threads_amount> <input_file_1> <input_file_2>"
            << "\tprocesses <threads_amount> <input_file_1> <input_file_2> <output_file>"
            << "\n";
        return -1;
    }

    int threadsNum = 0;
    for (int i = 0; argv[1][i] != '\0'; i++) {
        threadsNum = threadsNum * 10 + (argv[1][i] - '0');
    }

    Threads threads(threadsNum, {argv[2], argv[3], argc == 5 ? argv[4] : nullptr});
    threads.run();
    return 0;
}
