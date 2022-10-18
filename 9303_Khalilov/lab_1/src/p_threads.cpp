#include <thread>
#include "utils.cpp"
#include <mutex>

using namespace std;

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
        });
    thread_rand.join();
    timer->stop("Read duration: ");

    MATRIX::Matrix<int> mat_res(mat_1.size().first, mat_1.size().second);

    vector<int> queue_task;

    for (size_t i = 0; i < mat_1.size().first; i++)
    {
        queue_task.push_back(mat_1.size().first - 1 - i);
    }

    timer->start();

    int p = argc == 2 ? stoi(argv[1]) : 4;

    mutex main_mutex;
    vector<thread> threads;

    for (int i = 0; i < p; i++)
    {
        threads.push_back(
            thread(
                [&]()
                {
                    cout << "thread ID[" << this_thread::get_id() << "] - is start work\n";
                    while (!queue_task.empty())
                    {
                        main_mutex.lock();
                        int line = queue_task.back();
                        queue_task.pop_back();
                        main_mutex.unlock();
                        MATRIX::sumVectors(mat_1.getRow(line), mat_2.getRow(line), mat_res.getRow(line));
                    }
                    cout << "thread ID[" <<  this_thread::get_id() << "] - is finished work\n";
                }));
    }

    for (thread &item : threads)
    {
        item.join();
    }
    timer->stop("Matrices sum duration: ");

    timer->start();

    thread thread_write(
        [&]()
        {
            MATRIX::write(mat_res, "result.txt");
        });

    thread_write.join();
    timer->stop("Write duration: ");

    return 0;
}