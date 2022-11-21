#include <sys/wait.h>
#include "utils.cpp"

int main(int argc, const char **argv)
{

    srand(time(0));

    int n = 5;
    int m = 5;

    if( argc == 3 ) {
        n = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    }

    MATRIX::Matrix<int> matrix_1 = MATRIX::generate(n, m);
    MATRIX::write(matrix_1, "matrix_1.txt");

    MATRIX::Matrix<int> matrix_2 = MATRIX::generate(n, m);
    MATRIX::write(matrix_2, "matrix_2.txt");


    return 0;
}