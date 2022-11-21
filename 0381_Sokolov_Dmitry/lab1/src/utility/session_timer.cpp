#include "session_timer.h"

void SessionTimer::start_session() {

    if (session_live) {
        this->finish_session();

    }

    this->session_live = true;
    this->current = steady_clock::now();
}

void SessionTimer::finish_session() {
    steady_clock::time_point end = steady_clock::now();

    this->sessions.emplace_back(std::make_pair(this->current, end));
    session_live = false;
}

long long int SessionTimer::get_duration(int index) {
    return duration_cast<std::chrono::microseconds>(this->sessions[index].second - this->sessions[index].first).count();
}

long long int SessionTimer::last() {
    return duration_cast<std::chrono::microseconds>(this->sessions.back().second - this->sessions.back().first).count();
}


