#include "threads.h"

int main(int argc, char *argv[]) {
    auto stringToNumber = [] (char *string) -> int {
        int number = 0;
        for (int i = 0; string[i] != '\0'; i++) {
            number = number * 10 + (string[i] - '0');
        }
        return number;
    };

    if (argc != 4) {
        std::cout << "Usage: ./lab4 <matrix_size> <multiplication_output_file> <strassen_output_file>\n";
        return 0;
    }

    int matrixSize = stringToNumber(argv[1]);

    Threads threads(matrixSize, argv[2], argv[3]);
    threads.run();
    return 0;
}
