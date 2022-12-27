#ifndef PA_LAB2_CONFIG_H
#define PA_LAB2_CONFIG_H

#include <string>
#include <iostream>


class Config {
public:
    static const int R;
    static const int C;

    // matrix generation
    static const int generation_ceiling;

    // file handler
    static const std::string Data_path;
    static const std::string result;

    //Threads
    static const int min_threads;
    static const int max_threads;
    static const int threads;

    static const int depth_limit;
    static const int size_floor;

    //Logger
    static const std::string log_path;

    static void log_config();
};



#endif //PA_LAB2_CONFIG_H
