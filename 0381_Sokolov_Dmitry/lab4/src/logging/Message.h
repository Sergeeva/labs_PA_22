#ifndef SRC_MESSAGE_H
#define SRC_MESSAGE_H

#include <string>
#include <utility>
#include <atomic>


enum MPriority {
    INFO, TRACE, WARN, ERROR
};


static std::atomic<int> msg_id{0};

int message_id();


class Message {
    int id = 0;

    std::string priority = "INFO";

    std::string msg;

    void set_priority(MPriority _priority);

public:
    Message() = default;

    Message(MPriority _priority, std::string _msg);

    Message(const Message& other) = default;

    Message& operator=(const Message& other) = default;

    std::string get_message() const;

    std::string get_priority() const;

    int get_id() const;

    friend std::ostream& operator<<(std::ostream& out, const Message& msg);

    static Message operation_complete(int id, const std::string& operation, long long int duration = -1);
};


#endif //SRC_MESSAGE_H
