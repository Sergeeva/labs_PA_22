#ifndef PARALLEL_ALGORITHMS_LAB5_TIME_H
#define PARALLEL_ALGORITHMS_LAB5_TIME_H

#include <chrono>

class Time {
    using clock = std::chrono::time_point<std::chrono::steady_clock>;
public:
    static clock now();

    static long long get_elapsed_time_in_microseconds(clock start);
};

#endif // PARALLEL_ALGORITHMS_LAB5_TIME_H
