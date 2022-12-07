#ifndef MANDELBROT_SVPNG_H
#define MANDELBROT_SVPNG_H

#ifndef SVPNG_LINKAGE
#define SVPNG_LINKAGE
#endif

#ifndef SVPNG_OUTPUT

#include <stdio.h>

#define SVPNG_OUTPUT FILE* fp
#endif

#ifndef SVPNG_PUT
#define SVPNG_PUT(u) fputc(u, fp)
#endif


/*!
    \brief Save a RGB/RGBA image in PNG format.
    \param SVPNG_OUTPUT Output stream (by default using file descriptor).
    \param w Width of the image. (<16383)
    \param h Height of the image.
    \param img Image pixel data in 24-bit RGB or 32-bit RGBA format.
    \param alpha Whether the image contains alpha channel.
*/
SVPNG_LINKAGE void svpng(SVPNG_OUTPUT, unsigned w, unsigned h, const unsigned char *img, int alpha);

#endif
