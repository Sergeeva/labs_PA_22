#include "mandelbrot.hpp"
#include "mandelbrot_cpu.hpp"

#define WIDTH 1920
#define HEIGHT 1080
#define ITERATIONS 256
#define COEFFICIENT 4
#define RESULT_FILE_PATH "mandelbrot.ppm"
#define RESULT_CPU_FILE_PATH "mandelbrot_cpu.ppm"

void save(const cl_uint *result, const std::string &name, int height, int width) {
    std::ofstream file(name, std::ios::binary);
    file << "P6\n" << width << " " << height << "\n255\n";
    for (int y = 0; y < height; ++y) {
        const unsigned int *line = result + width * y;
        for (int x = 0; x < width; ++x) {
            file.write((const char *) (line + x), 3);
        }
    }
}

int main() {
    clock_t startTime = clock();
    std::vector<cl_uint> result = mandelbrot(COEFFICIENT, HEIGHT, WIDTH, ITERATIONS);
    printf("Time taken: %.6f\n", (double) (clock() - startTime) / CLOCKS_PER_SEC);
    save(result.data(), RESULT_FILE_PATH, HEIGHT, WIDTH);

    startTime = clock();
    std::vector<unsigned int> resultCpu = mandelbrotCpu(COEFFICIENT, HEIGHT, WIDTH, ITERATIONS);
    printf("Time taken: %.6f\n", (double) (clock() - startTime) / CLOCKS_PER_SEC);
    save(resultCpu.data(), RESULT_CPU_FILE_PATH, HEIGHT, WIDTH);

    return 0;
}
