#include "threads.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: ./lab3 <matrix_amount> <output_file>\n";
        return 0;
    }

    int matrixAmount = 0;
    for (int i = 0; argv[1][i] != '\0'; i++) {
        matrixAmount = matrixAmount * 10 + (argv[1][i] - '0');
    }

    Threads threads(matrixAmount, argv[2]);
    threads.run();
    return 0;
}
