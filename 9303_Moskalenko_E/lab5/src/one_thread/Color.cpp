#include "Color.h"

Color get_color(int n, int iter_max, int boundary) {
    unsigned int r = (n + boundary * 3) % 255 ;
    unsigned int g = (n * iter_max / boundary) % 255;
    unsigned int b = ((boundary * iter_max / n + 4000) - n) % 255;
    return Color(r,g , b);
};
