#include <utility>

#include "../headers/Configurator.h"
#include "../headers/StrassenMatricesMultiplier.h"
#include "../headers/SimpleMatricesMultiplier.h"

void Configurator::run() {
    auto configuration = Configuration::get();

    configure("Input file name: ",
              [](std::string string) { return string; },
              [&configuration](std::string file_name) { configuration->set_input_file_name(std::move(file_name)); });

    configure("Readers count: ",
              [](const std::string &string) { return std::stoi(string); },
              [&configuration](int count) { configuration->set_readers_count(count); });

    configure("Multipliers count: ",
              [](const std::string &string) { return std::stoi(string); },
              [&configuration](int count) { configuration->set_multipliers_count(count); });

    configure("Iterations count: ",
              [](const std::string &string) { return std::stoi(string); },
              [&configuration](int count) { configuration->set_iterations_count(count); });

    configure("Workers count: ",
              [](const std::string &string) {
                  return std::stoi(string);
              },
              [&configuration](int count) { configuration->set_multiplier_workers_count(count); });

    configure("Writers count: ",
              [](const std::string &string) { return std::stoi(string); },
              [&configuration](int count) { configuration->set_writers_count(count); });

    configure("Use strassen algorithm? (yes/y or any other value for no) ",
              [](const std::string &string) { return Configurator::answer_to_bool(string); },
              [&configuration](bool strassen) {
                  MatricesMultiplier *algorithm;

                  if (strassen) {
                      algorithm = new StrassenMatricesMultiplier();
                  } else {
                      algorithm = new SimpleMatricesMultiplier(configuration->get_workers_count());
                  }

                  configuration->set_multiplier(std::shared_ptr<MatricesMultiplier>(algorithm));
              });
}

bool Configurator::answer_to_bool(const std::string &answer) {
    return answer == "yes" || answer == "y";
}
