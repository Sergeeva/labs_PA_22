#ifndef LAB3_SESSION_TIMER_H
#define LAB3_SESSION_TIMER_H


#include <chrono>
#include <utility>
#include <vector>

using namespace std::chrono;

class SessionTimer {
    steady_clock::time_point current;
    bool session_live = false;

    std::vector<std::pair<steady_clock::time_point, steady_clock::time_point>> sessions;

public:
    SessionTimer() = default;

    void start_session();

    void finish_session();

    long long int get_duration(int index);

    long long int last();

};

#endif //LAB3_SESSION_TIMER_H
