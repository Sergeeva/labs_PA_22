#ifndef MANDELBROT_SAVE_CL_H
#define MANDELBROT_SAVE_CL_H

#include <OpenCL/cl.h>

#define CL_SAVE_EXECUTE(...)    check_code((__VA_ARGS__), __FILE__, __LINE__);
#define CL_CHECK_CODE(err) check_code(err, __FILE__, __LINE__);

char const *map_to_description(cl_int const code);

cl_int check_code(cl_int code, const char *file, int line);

#endif

