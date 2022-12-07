#include "one_thread_mandelbrot.h"
#include "../svpng/svpng.h"
#include "Color.h"

std::chrono::milliseconds one_thread_algorithm(unsigned char *results,
                                               size_t height,
                                               size_t width,
                                               size_t iterations) {
    auto start = std::chrono::steady_clock::now();
    const double xMin = -2;
    const double yMax = 1;
    double dx = 3.4;
    double dy = 2;

    for (int j = 0; j < height; j++) {
        float y0 =  (j * dy) / height - yMax;
        for (int i = 0; i < width; i++) {

          float x0 = (i * dx) / width + xMin;
            float x = 0.0;
            float y = 0.0;

            int iteration = 0;
            float bounded = x * x + y * y;

            while (iteration < iterations) {
                float x_prev = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = x_prev;
                bounded = x * x + y * y;
                iteration++;
                if (bounded >= 4.0)
                    break;
            }

            Color color{};

            if (bounded >= 4.0) {
                 color = get_color(iteration, iterations, bounded);
            }

            color.set(results, i, j, width);
        }
    }

    auto finish = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
}

std::chrono::milliseconds one_thread_mandelbrot(std::string &output_filename,
                                                size_t image_height, size_t image_width,
                                                size_t iterations) {
    auto image = new unsigned char[image_width * image_height * 4];

    auto duration = one_thread_algorithm(image, image_height, image_width, iterations);

    auto file = fopen(output_filename.c_str(),  "wb");
    svpng(file, image_width, image_height, image, 1);
    fclose(file);

    delete[] image;

    return duration;
}
