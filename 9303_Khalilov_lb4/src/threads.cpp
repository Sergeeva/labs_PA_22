#include "./utils.cpp"
#include "./strassen.cpp"
#include <thread>
#include <ctime>
#include <math.h> /* pow */

using namespace std;

int main(int argc, const char **argv)
{
    srand(time(0));

    int power = 7, n = pow(2, power), m = pow(2, power);

    auto A = MATRIX::generate(n, m);
    auto B = MATRIX::generate(n, m);

    cout << "matrices size = " << n << "x" << m << "\n";

    clock_t timer = clock();

    MATRIX::Matrix<int> C1 = MATRIX::parallelSimpleMultiply(A, B);

    clock_t duration = clock() - timer;
    cout << "Simple Parallel Multiply:   ";
    cout << duration / (double)CLOCKS_PER_SEC << "s" << endl;

    MATRIX::Matrix<int> C2;

    timer = clock();

    STRASSEN::multiply(A, B, C2, n);

    // cout << "\n"
    //      << A << "\n"
    //      << B << "\n"
    //      << C1 << "\n"
    //      << C2 << "\n";

    duration = clock() - timer;
    cout << "Strassen Parallel Multiply: ";
    cout << duration / (double)CLOCKS_PER_SEC << "s" << endl;

    return 0;
}