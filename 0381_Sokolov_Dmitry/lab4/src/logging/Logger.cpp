#include "Logger.h"

TRStack<Message> Logger::file_output = TRStack<Message>();
TRStack<Message> Logger::console_output = TRStack<Message>();
MPriority Logger::mode = INFO;

Logger &Logger::get_instance() {
    static Logger instance;

    return instance;
}

void Logger::toggle_log(MPriority _mode) {
    mode = _mode;
}

void Logger::trace(const std::string& message, bool to_file) {
    if (mode >= TRACE) {
        log(TRACE, message, to_file);
    }
}

void Logger::info(const std::string& message, bool to_file) {
    if (mode >= INFO) {
        log(INFO, message, to_file);
    }
}

void Logger::warn(const std::string &message, bool to_file) {
    if (mode >= WARN) {
        log(WARN, message, to_file);
    }
}

void Logger::error(const std::string &message, bool to_file) {
    if (mode >= ERROR) {
        log(ERROR, message, to_file);
    }
}

void Logger::log(MPriority priority, const std::string &message, bool to_file) {
    Logger::log(Message(priority, message), to_file);
}

void Logger::log(const Message &message, bool to_file) {
    if (to_file) {
        get_instance().file_output.push(message);
    } else {
        get_instance().console_output.push(message);
    }

}

void Logger::dump(const std::string& file_path, const std::string& file_name) {
    std::vector<Message> file;
    std::vector<Message> console;

    int msgs = console_output.get_size();

    for (int i = 0; i < msgs; i++) {
        console.emplace_back(console_output.pop());
    }

    msgs = file_output.get_size();

    for (int i = 0; i < msgs; i++) {
        file.emplace_back(file_output.pop());
    }

    std::stable_sort(console.begin(), console.end(), compare_messages);
    std::stable_sort(file.begin(), file.end(), compare_messages);

    std::ofstream output;
    std::string path = file_path + file_name + ".txt";
    output.open(path, std::ofstream::app);

    if (!output.is_open()) {
        try {
            std::filesystem::create_directories(file_path);
            output.open(path, std::ofstream::out);
        }
        catch (...)
        {
            throw std::runtime_error("Couldn't open file at {" + path + "}!");
        }
    }

    for (const auto & i : file) {
        output << i;
    }

    output << "-----------------------------------------------------------------\n\n";
    output.close();

    for(const auto & i : console) {
        std::cout << i;
    }
}

bool compare_messages(const Message& first, const Message& second) {
    return first.get_id() < second.get_id();
}
