#pragma once

#include <vector>

std::vector<unsigned int> mandelbrotCpu(float coefficient, int height, int width, int iterations) {
    std::vector<unsigned int> results;
    results.resize(height * width);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            float x0 = coefficient * (((float) i - (float) width / 2) / (float) width);
            float y0 = coefficient * (((float) j - (float) height / 2) / (float) height);
            float x = x0;
            float y = y0;

            int iter = 0;

            while (x * x + y * y <= 4.0 && iter < iterations) {
                float x_next = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = x_next;
                iter++;
            }

            results[j * width + i] = 0xf4 * iter % 256 * 0xf4;
        }
    }

    return results;
}