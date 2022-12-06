#ifndef MANDELBROT_OPENCL_MANDELBROT_H
#define MANDELBROT_OPENCL_MANDELBROT_H

#include <string>
#include <OpenCL/CL.h>

std::chrono::microseconds opencl_mandelbrot(unsigned char *image,
                                            unsigned int image_height, unsigned int image_width,
                                            unsigned int iterations);

cl_uint get_platforms_count();

std::vector<cl_platform_id> get_platforms(cl_uint platforms_count);

std::vector<unsigned char> get_device_name(cl_device_id device);

cl_uint get_devices_count(cl_platform_id platform);

std::vector<cl_device_id> get_devices(cl_platform_id platform, cl_uint devices_count);

cl_device_id get_first_device(cl_platform_id platform);

cl_device_id get_first_device();

cl_program build(const cl_context context, const cl_device_id device, const std::string &source);

std::string get_device_type(cl_device_id device);

cl_mem createImage(cl_context context, size_t height, size_t width);

#endif
