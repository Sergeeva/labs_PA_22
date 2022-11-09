#include "logger.h"

void Logger::printThreadIdWithText(std::string_view text) {
#if (ENABLE_LOGS)
    std::stringstream ss;
    ss << "ThreadId: " << std::this_thread::get_id() << ". " << text << '\n';
    std::cout << ss.str();
#endif
}

void Logger::printCPlusPlusVersion() {
#if (ENABLE_LOGS)
    std::cout << "C++ version: " << __cplusplus << '\n';
#endif
}
