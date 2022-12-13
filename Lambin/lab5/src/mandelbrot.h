#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace Mandelbrot {
    cl_device_id createDevice();
    cl_context createContext(cl_device_id);
    cl_program buildProgram(cl_context, cl_device_id);
    std::string getProgramText();
    cl_kernel createKernel(cl_program);
    cl_command_queue createQueue(cl_context, cl_device_id);
    cl_mem createBuffer(cl_context, size_t);
    void invokeKernel(cl_kernel, cl_command_queue, cl_mem, cl_uint *, float, float, float, int, int, float);
    size_t align(int, size_t);
    void save(const cl_uint *, int, int);
    void reportError(cl_int, const std::string);
};

#endif //MANDELBROT_H
