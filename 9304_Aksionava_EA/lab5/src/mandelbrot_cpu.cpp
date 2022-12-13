#include "mandelbrot_cpu.hpp"

void mandelbrot_cpu(
        unsigned int* results,
        unsigned int height,
        unsigned int width,
        int iterations
) {

    clock_t start_time = clock();

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            float x0 = COEFFICIENT * (((float)i - (float)width/2) / (float)width);
            float y0 = COEFFICIENT * (((float)j - (float)height/2) / (float)height);
            float x = x0;
            float y = y0;

            int iteration = 0;

            while ((x * x + y * y) <= 4.0 && iteration < iterations) {
                float x_next = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = x_next;
                iteration++;
            }

            results[j * width + i] = 0xf * (iteration % 256) * 0xf;
        }
    }

    printf("Time taken: %.6f\n", (double)(clock() - start_time) / CLOCKS_PER_SEC);
}

void run_cpu() {
    std::vector<unsigned int> result(WIDTH * HEIGHT);
    mandelbrot_cpu(result.data());
    save_result(result.data(), RESULT_CPU_FILE_PATH);
}



