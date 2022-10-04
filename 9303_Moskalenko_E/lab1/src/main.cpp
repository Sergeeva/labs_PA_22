#include <iostream>
#include "./processes.h"
#include "./threads.h"

using namespace std;

enum TASK {
    PROCESSES = 0,
    ONE_THREAD_ADDITION = 1,
    MULTI_THREAD_ADDITION = 2
};

int main() {
    int task;
    cin >> task;
    switch (task) {
        case PROCESSES:
            processesFunc();
            break;
        case ONE_THREAD_ADDITION:
            threads(&oneThreadAddition);
            break;
        case MULTI_THREAD_ADDITION:
            threads(&multiThreadsAddition);
            break;
        default:
            return 0;
    }
    return 0;
}