#include <thread>
#include "utils.cpp"


int main(int argc, const char **argv)
{


    MATRIX::Matrix<int> mat_1, mat_2;
    
    MATRIX::Timer *timer = new MATRIX::Timer();

    timer->start();
    thread thread_rand(
        [&]()
        {
            mat_1 = MATRIX::read<int>("matrix_1.txt");
            mat_2 = MATRIX::read<int>("matrix_2.txt"); 
        }
    );
    thread_rand.join();
    timer->stop("Read duration: ");

    
    MATRIX::Matrix<int> sum;    
    timer->start();

    thread thread_sum(
        [&](){
            sum = MATRIX::sum(mat_1, mat_2);
        }
    );
    
    thread_sum.join();
    timer->stop("Matrices sum duration: ");

    timer->start();
    
    thread thread_write(
        [&](){
            MATRIX::write(sum, "result.txt");
        }
    );
    
    thread_write.join();
    timer->stop("Write duration: ");
    
    return 0;
}