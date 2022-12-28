#include "opencl_mandelbrot.h"
#include "sequential_mandelbrot.h"

#include <iostream>
#include "time/Time.h"

void save_image_in_portable_pixmap_format(
        int height,
        int width,
        const unsigned int* buffer,
        const std::string& output_ppm_filename
) {
    std::ofstream file(output_ppm_filename, std::ios::binary);
    const auto ppm_file_type_identifier = "P6";
    const auto ppm_max_color_value = 255;
    const auto whitespace_delimiter = '\n';

    file << ppm_file_type_identifier << whitespace_delimiter
         << width << whitespace_delimiter
         << height << whitespace_delimiter
         << ppm_max_color_value << whitespace_delimiter;

    for (unsigned int y = 0; y < height; ++y) {
        const unsigned int* row = buffer + width * y;

        for (unsigned int x = 0; x < width; ++x) {
            file.write((const char*) (row + x), 3);
        }

    }
}

void printTimeWithText(
        long long time,
        const std::string& text
) {
    std::cout << text << ": " << time << " microseconds\n";
}

int main() {
    float ratio = 5;
    const size_t width = 1920;
    const size_t height = 1280;
    int iterations_limit = 60;
    const auto output_sequential_ppm_filename = "output_sequential_mandelbrot.ppm";
    const auto output_opencl_ppm_filename = "output_opencl_mandelbrot.ppm";

    auto start = Time::now();
    auto sequential_buffer = compute_sequential_mandelbrot(height, width, ratio, iterations_limit);
    save_image_in_portable_pixmap_format(height, width, sequential_buffer.data(), output_sequential_ppm_filename);
    printTimeWithText(Time::get_elapsed_time_in_microseconds(start), "Sequential (CPU)");

    start = Time::now();
    auto opencl_buffer = compute_opencl_mandelbrot(height, width, ratio, iterations_limit);
    save_image_in_portable_pixmap_format(height, width, opencl_buffer.data(), output_opencl_ppm_filename);
    printTimeWithText(Time::get_elapsed_time_in_microseconds(start), "OpenCL (GPU)");


    return 0;
}
