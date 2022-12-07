#ifndef MANDELBROT_COLOR_H
#define MANDELBROT_COLOR_H

class Color {
    unsigned int red{};
    unsigned int green{};
    unsigned int blue{};
    unsigned int alpha{255};

public:
    Color(unsigned int r, unsigned int g, unsigned int b) : red(r), green(g), blue(b) {}
    Color() = default;
    void set(unsigned char *results, int x, int y, unsigned int width) const {
        auto coord = (y * width + x) * 4;
        results[coord] = red;
        results[coord + 1] = green;
        results[coord + 2] = blue;
        results[coord + 3] = alpha;
    }

};

Color get_color(int steps, int max, int b);

#endif //MANDELBROT_COLOR_H
