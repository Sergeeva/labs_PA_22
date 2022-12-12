#include "config.h"

// Matrix dimensions
const int  Config::R = 7;
const int Config::C = 5;

// Defines the maximum number of digits of the matrix values
const int Config::generation_ceiling = 10;

// File output config
const std::string Config::Data_path = "../data/";
const std::string Config::summed = "A+B";

// Multiple-thread arithmetic parameters
const int Config::min_threads = 1;
const int Config::max_threads = 60;
const int Config::execution_threads = 50;
const int Config::recursion_limit = 200;

// Number of operations being performed on matrices
const int Config::threads = 3;
const int Config::iterations = 50;

// Buffer configuration
const int Config::max_size = 300;


void Config::log_config() {
    std::cout << "current configuration:" << std::endl;
    std::cout << "Matrices [" + std::to_string(Config::R) + " X " + std::to_string(Config::C) + "], ";
    std::cout << "which are randomly generated, " << std::endl;
    std::cout << "Are being summed by " + std::to_string(Config::threads) + " threads, ";
    std::cout << "each iterating " + std::to_string(Config::iterations) + " times." << std::endl;
    std::cout << "Each sum is paralleled into " + std::to_string(Config::execution_threads) + " threads." << std::endl;
    std::cout << "(Blocked queue buffer size limit: " + std::to_string(Config::max_size) + " elements)" << std::endl;
    std::cout << std::endl;
}
