#include "first_task/ProcessHandler.h"
#include "second_task/ThreadHandler.h"
#include "third_task/MultiThread.h"

void task_1() {
    std::cout << "[Task 1: processes]\n\n";
    ProcessHandler proc = ProcessHandler();
    proc.run();
}

void task_2() {
    std::cout << "[Task 2: 3 threads]\n\n";
    ThreadHandler thrd = ThreadHandler();
    thrd.execute(true);
}

void task_3() {
    std::cout << "[Task 3: P - threads]\n\n";
    MultiThread mlth = MultiThread();
    mlth.log_mode(false);

    int min_threads = PThreadsConfig::min_threads;
    int max_threads = PThreadsConfig::max_threads;

    for (int i = min_threads; i <= max_threads; i+= (i < 10 ? 1 : 5)) {
        mlth.set_threads(i);
        long long duration = mlth.timed_execute(Config::remake);

        std::cout << "[Threads: " << std::to_string(i) << " / Time: " << std::to_string(duration) << "]\n";
    }

    std::cout << "";

}

int main() {

    //task_1();
    //taske_2();
    task_3();

    return 0;
}