#include "semafore.h"


Semafore::Semafore(int count){
    this->count = count;
}


void Semafore::up(){
    std::unique_lock<std::mutex> lock(mutex);
    count++;
    if (count == 0) {
        cond_var.notify_one();
    }
}


void Semafore::down(){
    std::unique_lock<std::mutex> lock(mutex);
    count--;
    if (count <  0) {
        cond_var.wait(lock);
    }
}
