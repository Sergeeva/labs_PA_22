#include "Time.h"

Time::clock Time::now() {
    return std::chrono::steady_clock::now();
}

long long Time::get_elapsed_time_in_microseconds(const Time::clock start) {
    auto end = now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
