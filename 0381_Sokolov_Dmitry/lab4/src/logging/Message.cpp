#pragma once
#include "Message.h"

int message_id() {
    return msg_id++;
}

Message::Message(MPriority _priority, std::string _msg): msg(std::move(_msg)), id(message_id()){
    set_priority(_priority);
}

std::string Message::get_message() const {
    return msg;
}

int Message::get_id() const {
    return id;
}

std::string Message::get_priority() const {
    return priority;
}

std::ostream &operator<<(std::ostream &out, const Message &msg) {
    out << std::string("[");
    out << msg.get_priority();
    out << std::string("] ");

    out << msg.get_message();

    return out;
}

void Message::set_priority(MPriority _priority) {

    switch (_priority) {
        case TRACE:
            priority = "TRACE";
            break;

        case WARN:
            priority = "WARNING";
            break;

        case ERROR:
            priority = "ERROR!";
            break;

        default:
            priority = "INFO";
            break;
    }
}

Message Message::operation_complete(int id, const std::string& operation, long long int duration) {
    std::string msg;

    if (duration >= 0) {
        msg += std::to_string(id);
        msg += " set of matrices ";
        msg += "{" + operation + "} ";
        msg += "- done within: ";
        msg += std::to_string(duration) + " mcs\n";
    }
    else {
        msg += std::to_string(id);
        msg += " set of matrices ";
        msg += "{" + operation + "} ";
        msg += "- done\n";
    }

    return {TRACE, msg};
}
