#include "config.h"

const int  Config::R = 25;
const int Config::C = 25;

const bool Config::remake = true;
const int Config::first_seed = 800;
const int Config::second_seed = 127;
const int Config::generation_ceiling = 101;

const std::string Config::Data_path = "../../data/";
const std::string Config::first = "A";
const std::string Config::second = "B";
const std::string Config::first_plus_second = "A + B";

//----------------------------------------------------------------------------------------------------------------------

const int MemoryConfig::memory_size = 1024;
const int MemoryConfig::memory_key = 1;
const int MemoryConfig::flag = 0644 | IPC_CREAT;

//----------------------------------------------------------------------------------------------------------------------

const int PThreadsConfig::min_threads = 1;
const int PThreadsConfig::max_threads = 40;