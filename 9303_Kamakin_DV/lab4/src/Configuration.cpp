#include <utility>
#include <memory>

#include "../headers/Configuration.h"
#include "../headers/SimpleMatricesMultiplier.h"
#include "../headers/StrassenMatricesMultiplier.h"

std::shared_ptr<Configuration> Configuration::instance = nullptr;

void Configuration::set_input_file_name(std::string file_name) {
    this->input_file_name = std::move(file_name);
}

void Configuration::set_iterations_count(int count) {
    this->iterations_count = count;
}

void Configuration::set_multipliers_count(int count) {
    this->multipliers_count = count;
}

void Configuration::set_readers_count(int count) {
    this->readers_count = count;
}

void Configuration::set_multiplier_workers_count(int count) {
    this->workers_count = count;
}

void Configuration::set_writers_count(int count) {
    this->writers_count = count;
}

std::string Configuration::get_input_file_name() const {
    return input_file_name;
}

int Configuration::get_writers_count() const {
    return writers_count;
}

int Configuration::get_readers_count() const {
    return readers_count;
}

int Configuration::get_multipliers_count() const {
    return multipliers_count;
}

int Configuration::get_workers_count() const {
    return workers_count;
}

int Configuration::get_iterations_count() const {
    return iterations_count;
}

std::string Configuration::get_output_file_name() const {
    return output_file_name;
}

std::shared_ptr<Configuration> Configuration::get() {
    if (!instance) {
        instance = std::make_shared<Configuration>(Configuration());
    }
    return instance;
}

const std::shared_ptr<MatricesMultiplier> &Configuration::get_multiplier() const {
    return multiplier;
}

void Configuration::set_multiplier(std::shared_ptr<MatricesMultiplier> multiplier) {
    Configuration::multiplier = std::move(multiplier);
}

const int Configuration::DefaultConfiguration::workers_count = 1;

auto default_multiplier = new StrassenMatricesMultiplier();

const std::string Configuration::DefaultConfiguration::output_file_name = std::string("../resources/output.txt");
const std::string Configuration::DefaultConfiguration::input_file_name = std::string(
        "../resources/input_512_512.txt");
const int Configuration::DefaultConfiguration::writers_count = 1;
const int Configuration::DefaultConfiguration::readers_count = 1;
const std::shared_ptr<MatricesMultiplier> Configuration::DefaultConfiguration::multiplier = std::shared_ptr<MatricesMultiplier>(
        default_multiplier);
const int Configuration::DefaultConfiguration::multipliers_count = 1;
const int Configuration::DefaultConfiguration::iterations_count = 1;
