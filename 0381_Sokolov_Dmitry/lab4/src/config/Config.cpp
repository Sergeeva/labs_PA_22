#include "config.h"

// Matrix dimensions
const int  Config::R = 256;
const int Config::C = 256;

// Defines the maximum number of digits of the matrix values
const int Config::generation_ceiling = 10;

// File output config
const std::string Config::Data_path = "../data/";
const std::string Config::result = "AB";

// Simple parallelization parameters
const int Config::min_threads = 1;
const int Config::max_threads = 70;
const int Config::threads = 7;

// Strassen parameters
const int Config::depth_limit = 1;
const int Config::size_floor = 4;


// Logger parameter
const std::string Config::log_path = "../logs/";

void Config::log_config() {
    std::cout << "Matrix dimensions: [" << Config::R << "x" << Config::C << "],\n";
    std::cout << "filled with integer numbers with [" << Config::generation_ceiling << "] digits.\n";
    std::cout << "Simple parallelization: matrix multiplication split between [" << Config::threads << "] threads;\n";
    std::cout << "Strassen algorithm runs in parallel until reaching depth [" << Config::depth_limit << "] OR \n";
    std::cout << "until matrices reach [" << Config::size_floor << "]" << std::endl;
}
