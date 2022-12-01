#include "config.h"

// Matrix dimensions
const int  Config::R = 4;
const int Config::C = 4;

// Defines the maximum number of digits of the matrix values
const int Config::generation_ceiling = 101;

// File output config
const std::string Config::Data_path = "../data/";
const std::string Config::summed = "A+B";

// Multiple-thread sum parameters
const int Config::min_threads = 1;
const int Config::max_threads = 40;
const int Config::summator_threads = 4;

// Number of iterations of performing the operations on matrices
const int Config::iterations = 5;

// Queue size limit
const int Config::max_size = 10;
