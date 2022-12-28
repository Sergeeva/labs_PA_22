#ifndef SRC_LOGGER_H
#define SRC_LOGGER_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

#include "./Message.h"
#include "../buffers/Treiber_stack.h"


using namespace std::chrono;


bool compare_messages(const Message& first, const Message& second);

class Logger {

    static Logger* instance;

    static MPriority mode;

    static TRStack<Message> file_output;
    static TRStack<Message> console_output;

    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator= (const Logger&) = delete;

public:

    static Logger& get_instance();

    static void toggle_log(MPriority _mode);

    static void trace(const std::string& message, bool to_file = false);

    static void info(const std::string& message, bool to_file = false);

    static void warn(const std::string& message, bool to_file = false);

    static void error(const std::string& message, bool to_file = false);

    static void log(MPriority priority, const std::string& message, bool to_file = false);

    static void log(const Message& message, bool to_file = false);

    static void dump(const std::string& file_path, const std::string& file_name = "log");

    ~Logger() = default;


};


#endif //SRC_LOGGER_H
