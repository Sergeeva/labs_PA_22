#ifndef UNTITLED_CONFIGURATOR_H
#define UNTITLED_CONFIGURATOR_H

#include <iostream>
#include <functional>
#include "Worker.h"
#include "Configuration.h"

class Configurator : public Runnable {

private:

    template<typename P, typename C>
    void configure(const std::string &message, P parser, C consumer) {
        std::cout << message;

        std::string input;

        getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Nothing entered, default value will be set" << '\n';
            return;
        }

        consumer(parser(input));
    }

    static bool answer_to_bool(const std::string &answer);

public:

    void run() final;

};


#endif //UNTITLED_CONFIGURATOR_H
