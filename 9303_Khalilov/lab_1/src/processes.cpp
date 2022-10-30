#include <sys/wait.h>
#include "utils.cpp"

int main(int argc, const char **argv)
{

    srand(time(0));

    MATRIX::Timer *timer = new MATRIX::Timer();

    timer->start();

    MATRIX::Matrix<int> mat_1 = MATRIX::read<int>("matrix_1.txt");
    MATRIX::Matrix<int> mat_2 = MATRIX::read<int>("matrix_2.txt");

    timer->stop("Read duration: ");

    pid_t sum_pid = fork();
    if (sum_pid == 0)
    {
        pid_t write_pid = fork();

        timer->start();

        MATRIX::Matrix<int> sum = MATRIX::sum(mat_1, mat_2);
        timer->stop("Matrices sum duration: ");

        if (write_pid == 0)
        {
            timer->start();
            MATRIX::write(sum, "result.txt");
            timer->stop("Write duration: ");
            exit(0);
        }
        else if (write_pid > 0)
        {
            exit(-1);
        }
        else
        {
            wait(&write_pid);
        }
    }
    else if (sum_pid > 0)
    {
        wait(&sum_pid);
    }
    else
    {
        exit(-1);
    }

    return 0;
}