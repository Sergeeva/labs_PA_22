#include "mandelbrot_cpu.hpp"

void mandelbrot_cpu(unsigned int* results, unsigned int height, unsigned int width, int iterations, int coeff) {

    clock_t start_time = clock();

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            float x0 = coeff * (((float)i - (float)width/2) / (float)width);
            float y0 = coeff * (((float)j - (float)height/2) / (float)height);
            float x = x0;
            float y = y0;

            int cur_i = 0;

            for(; (x * x + y * y <= 2 * 2) && cur_i < iterations; cur_i++) {
                float tmp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = tmp;
            }


            results[j * width + i] = 0x7 * (cur_i % 256) * 0x7;
        }
    }


    std::cout << "Time 2 - " << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s" << std::endl;
}

void save_cpu_result(const unsigned int* result, const std::string& name, int width, int height) {
    std::ofstream file(name, std::ios::binary);
    file << "P6\n" << width << " " << height << "\n255\n";
    for (int y = 0; y < height; ++y) {
        const unsigned int* line = result + width * y;
        for (int x = 0; x < width; ++x) {
            file.write((const char *) (line + x), 3);
        }
    }
    file.close();
}


void start_cpu(int width, int height, int coeff, int iterations) {
    std::vector<unsigned int> result(width * height);
    mandelbrot_cpu(result.data(), height, width, iterations, coeff);
    save_cpu_result(result.data(), "./result2.ppm", width, height);
}