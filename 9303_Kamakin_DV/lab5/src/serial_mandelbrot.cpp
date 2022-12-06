#include "serial_mandelbrot.h"

class ColorRGBA {
    unsigned int R{};
    unsigned int G{};
    unsigned int B{};
    unsigned int A{255};

public:
    ColorRGBA(unsigned int r, unsigned int g, unsigned int b) : R(r), G(g), B(b) {}

    ColorRGBA() = default;

    void set(unsigned char *results, int x, int y, unsigned int width) const {
        auto coord = (y * width + x) * 4;

        results[coord] = R;
        results[coord + 1] = G;
        results[coord + 2] = B;
        results[coord + 3] = A;
    }

};

ColorRGBA get_color(int steps) {
    steps = steps % 16;

    switch (steps) {
        case 0:
            return {66, 30, 15};
        case 1:
            return {25, 7, 26};
        case 2:
            return {9, 1, 47};
        case 3:
            return {4, 4, 73};
        case 4:
            return {0, 7, 100};
        case 5:
            return {12, 44, 138};
        case 6:
            return {24, 82, 177};
        case 7:
            return {57, 125, 209};
        case 8:
            return {134, 181, 229};
        case 9:
            return {211, 236, 248};
        case 10:
            return {241, 233, 191};
        case 11:
            return {248, 201, 95};
        case 12:
            return {255, 170, 0};
        case 13:
            return {204, 128, 0};
        case 14:
            return {153, 87, 0};
        case 15:
            return {106, 52, 3};
        default:
            return {0, 0, 0};
    }
}

std::chrono::microseconds serial_mandelbrot(unsigned char *results,
                                            unsigned int height,
                                            unsigned int width,
                                            unsigned int iterations) {
    auto start = std::chrono::steady_clock::now();

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            float x0 = ((float) i / width) * 3.25 - 2.0;
            float y0 = ((float) j / height) * 2.0 - 1.0;

            float x = 0.0;
            float y = 0.0;

            int iteration = 0;
            float bounded = 0;

            while (iteration < iterations && bounded < 4.0) {
                float x_prev = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = x_prev;

                bounded = x * x + y * y;
                iteration++;
            }

            ColorRGBA color{};

            if (bounded >= 4.0) {
                color = get_color(iteration);
            }

            color.set(results, i, j, width);
        }
    }

    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}
