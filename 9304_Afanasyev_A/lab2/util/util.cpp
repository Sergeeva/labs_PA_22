#include "util.h"

void log(const std::string &text) {
    std::cout << "Pid: " << getpid() << ' ' << "ThreadId: " << std::this_thread::get_id() << ' ' << text << std::endl;
}
