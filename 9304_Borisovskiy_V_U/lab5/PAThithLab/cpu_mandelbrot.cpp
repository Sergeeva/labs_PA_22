#include "cpu_mandelbrot.h"


int compute_iterations(const float center_x, const float center_y, int iterations_limit) {
    int n = 0;

    for(float x = center_x, y = center_y; (x * x + y * y <= 2 * 2) && n < iterations_limit; n++) {
        float xtemp = x * x - y * y + center_x;
        y = 2 * x * y + center_y;
        x = xtemp;
    }

    return n;
}

unsigned int to_gray_color(int iterations, int iterations_limit) {
    int max_color_value = 0xFF;
    int pos = max_color_value * iterations / iterations_limit;

    return pos | (pos << 8) | (pos << 16) | (max_color_value << 24);
}

std::vector<unsigned int> compute_cpu_mandelbrot(
        int height,
        int width,
        float ratio,
        int iterations_limit
) {
    std::vector<unsigned int> pixels_buffer(height * width);

    for (int cur_y = 0; cur_y < height; ++cur_y) {
        for (int cur_x = 0; cur_x < width; ++cur_x) {

            if (cur_x < width && cur_y < height) {
                float x = ratio * (float) (cur_x - width / 2) / width;
                float y = ratio * (float) (cur_y - height / 2) / width;

                int iteration = compute_iterations(x, y, iterations_limit);
                int pixel_index = cur_y * width + cur_x;
                pixels_buffer[pixel_index] = to_gray_color(iteration, iterations_limit);
            }
        }
    }

    return pixels_buffer;
}
