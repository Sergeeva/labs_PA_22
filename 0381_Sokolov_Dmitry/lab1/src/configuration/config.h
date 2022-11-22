#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <string>
#include <sys/shm.h>


class Config {
public:
    static const int R;
    static const int C;

    // matrix generation
    static const bool remake;
    static const int first_seed;
    static const int second_seed;
    static const int generation_ceiling;

    // file handler
    static const std::string Data_path;
    static const std::string first;
    static const std::string second;
    static const std::string first_plus_second;

};

class MemoryConfig {
public:
    // shared memory
    static const int memory_size;
    static const int memory_key;
    static const int flag;
};

class PThreadsConfig {
public:
    static const int min_threads;
    static const int max_threads;
};

#endif //PROJECT_CONFIG_H
