#ifndef UNTITLED_CONFIGURATION_H
#define UNTITLED_CONFIGURATION_H

#include <string>
#include <memory>
#include <functional>
#include "MatricesMultiplier.h"

class Configuration {

    static std::shared_ptr<Configuration> instance;
    std::shared_ptr<MatricesMultiplier> multiplier;
    std::string input_file_name;
    std::string output_file_name;
    int readers_count;
    int multipliers_count;
    int workers_count;
    int iterations_count;
    int writers_count;

public:

    class DefaultConfiguration {

    public:
        static const std::string input_file_name;
        static const std::string output_file_name;
        static const int readers_count;
        static const std::shared_ptr<MatricesMultiplier> multiplier;
        static const int multipliers_count;
        static const int workers_count;
        static const int iterations_count;
        static const int writers_count;
    };

    static std::shared_ptr<Configuration> get();

    void set_input_file_name(std::string file_name);

    void set_readers_count(int count);

    void set_multipliers_count(int count);

    void set_multiplier_workers_count(int count);

    void set_iterations_count(int count);

    void set_writers_count(int count);

    void set_multiplier(std::shared_ptr<MatricesMultiplier> multiplier);

    const std::shared_ptr<MatricesMultiplier> &get_multiplier() const;

    std::string get_input_file_name() const;

    std::string get_output_file_name() const;

    int get_readers_count() const;

    int get_multipliers_count() const;

    int get_writers_count() const;

    int get_workers_count() const;

    int get_iterations_count() const;

    Configuration() : input_file_name(DefaultConfiguration::input_file_name),
                      multiplier(DefaultConfiguration::multiplier),
                      output_file_name(DefaultConfiguration::output_file_name),
                      readers_count(DefaultConfiguration::readers_count),
                      multipliers_count(DefaultConfiguration::multipliers_count),
                      workers_count(DefaultConfiguration::workers_count),
                      iterations_count(DefaultConfiguration::iterations_count),
                      writers_count(DefaultConfiguration::writers_count) {}


};


#endif //UNTITLED_CONFIGURATION_H
