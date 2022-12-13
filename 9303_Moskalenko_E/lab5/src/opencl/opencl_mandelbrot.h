#ifndef MANDELBROT_OPENCL_MANDELBROT_H
#define MANDELBROT_OPENCL_MANDELBROT_H

#include <string>
#include <OpenCL/CL.h>

std::chrono::milliseconds opencl_mandelbrot(std::string &output_filename,
                                            size_t image_height, size_t image_width,
                                            size_t iterations);

cl_program build(const cl_context context, const cl_device_id device, const std::string &source);

std::string get_device_type(cl_device_id device);

cl_uint get_platforms_count();

cl_platform_id get_platform(cl_uint platform_id);

std::vector<unsigned char> get_device_name(cl_device_id device);

cl_device_id get_first_device();

cl_mem create_image(cl_context context, size_t height, size_t width);

#endif
