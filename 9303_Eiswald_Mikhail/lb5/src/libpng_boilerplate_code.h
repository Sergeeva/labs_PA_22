#ifndef LIBPNG_BOILERPLATE_CODE_H
#define LIBPNG_BOILERPLATE_CODE_H

#include <png.h>
#include <pngconf.h>

#define IO_ERROR 1

struct Image {
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_structp png_ptr;
    png_structp write_ptr;
    png_infop info_ptr;
    png_infop info_ptr_before;
    png_infop write_info;
    int number_of_passes;
    png_bytep *row_pointers;

    int write(const char* filename);
};

#endif
