#include <filesystem>
#include "../headers/Starter.h"
#include "../headers/Configurator.h"

int main() {
//    Configurator().run();
    Starter(Configuration::get()).run();
    return 0;
}
