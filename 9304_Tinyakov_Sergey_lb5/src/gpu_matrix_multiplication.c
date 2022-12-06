#include "gpu_matrix_multiplication.h"
#include <stdio.h>
#include <CL/cl.h>

#define GROUP_BLOCK_SIZE (8)

cl_context context;
cl_program program;
cl_kernel kernel;
cl_mem buffer_a, buffer_b, buffer_c;
cl_command_queue cq;
cl_device_id devices[1];
size_t work_size_g[] = {GROUP_BLOCK_SIZE, GROUP_BLOCK_SIZE}, work_size_l[] = {GROUP_BLOCK_SIZE, GROUP_BLOCK_SIZE};

char* __read_kernel_source(char* filename, int* size){
    FILE *f = fopen(filename, "r");
    char *res = NULL;
    if(!f){
        return res;
    }
    if(fseek(f, 0L, SEEK_END)){
        goto read_end;
    }
    size_t source_size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    res = malloc(source_size * sizeof(char));
    if(!res){
        goto read_end;
    }
    fread(res, 1, source_size, f);
    *size = source_size;
    res[source_size - 1] = '\0';
read_end:
    fclose(f);
    return res;
}

void __clear_buffers(){
    if(buffer_a) clReleaseMemObject(buffer_a);
    if(buffer_b) clReleaseMemObject(buffer_b);
    if(buffer_c) clReleaseMemObject(buffer_c);
}

int compile_kernel(){
    int ret = 0;
    char *filename = "src/matrix_mul_kernel.cl";
    char info[1000] = {0};
    cl_int CL_err = CL_SUCCESS;
    cl_uint numPlatforms = 0;
    cl_platform_id platforms[1];

    CL_err = clGetPlatformIDs(1, platforms, &numPlatforms);
    if(CL_err != CL_SUCCESS){
        puts("Cannot get platforms");
        return 1;
    }
    clGetPlatformInfo(platforms[0], CL_PLATFORM_NAME, 1000, (void*)info, NULL);
    printf("Using platform %s\n", info);
    memset(info, '\0', 1000);
    
    cl_uint num_devices = 0;
    //cl_device_id devices[1];
    CL_err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 1, devices, &num_devices);
    if(CL_err != CL_SUCCESS){
        puts("Cannot get devices");
        return 1;
    }
    clGetDeviceInfo(devices[0], CL_DEVICE_NAME, 1000, (void*)info, NULL);
    printf("Using device %s\n", info);

    context = clCreateContext(NULL, num_devices, devices, NULL, NULL, &CL_err);
    if(CL_err != CL_SUCCESS){
        puts("Cannot create contex");
        return 1;
    }
    int source_size = 0;
    char *source = __read_kernel_source(filename, &source_size);
    if(source == NULL){
        puts("Cannot read source");
        return 1;
    }
    program = clCreateProgramWithSource(context, 1, &source, NULL, &CL_err);
    if(CL_err != CL_SUCCESS){
        puts("Cannot create program");
        ret = 1;
        return 1;
    }
    CL_err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if(CL_err != CL_SUCCESS){
        puts("Cannot buidl program");
        if(CL_err == CL_BUILD_PROGRAM_FAILURE){
            // Copied from stackoverflow
            // https://stackoverflow.com/questions/9464190/error-code-11-what-are-all-possible-reasons-of-getting-error-cl-build-prog
            puts("Build failed");
            size_t log_size;
            clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

            // Allocate memory for the log
            char *log = (char *) malloc(log_size);

            // Get the log
            clGetProgramBuildInfo(program, devices[0], CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

            // Print the log
            printf("%s\n", log);
            printf("Source code: %s\n", source);
        }
        return 1;
    }
    free(source);
    kernel = clCreateKernel(program, "matrix_mul", &CL_err);
    if(CL_err != CL_SUCCESS){
        puts("Cannot create kernel");
        return 1;
    }
    
    if(CL_err != CL_SUCCESS){
        puts("Cannot set kernel args");
        return 1;
    }
    cq = clCreateCommandQueue(context, devices[0], NULL, &CL_err);
    if(CL_err != CL_SUCCESS){
        puts("Cannot create command queue");
        return 1;
    }
    return 0;
}

int set_kernel_args(Matrix *matrix1, Matrix *matrix2, Matrix *result){
    cl_int CL_err = CL_SUCCESS;
    __clear_buffers();
    buffer_a = clCreateBuffer(context, CL_MEM_READ_WRITE, matrix1->row_size * matrix1->col_size * sizeof(int), NULL, &CL_err);
    if(CL_err != CL_SUCCESS){
        puts("Cannot create buffer A");
        return 1;
    }
    buffer_b = clCreateBuffer(context, CL_MEM_READ_WRITE, matrix2->row_size * matrix2->col_size * sizeof(int), NULL, &CL_err);
    if(CL_err != CL_SUCCESS){
        puts("Cannot create buffer B");
        return 1;
    }
    buffer_c = clCreateBuffer(context, CL_MEM_READ_WRITE, result->row_size * result->col_size * sizeof(int), NULL, &CL_err);
    if(CL_err != CL_SUCCESS){
        puts("Cannot create buffer C");
        return 1;
    }
    CL_err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_a);
    CL_err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_b);
    CL_err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_c);
    CL_err = clSetKernelArg(kernel, 3, sizeof(unsigned long), &matrix1->row_size);
    CL_err = clSetKernelArg(kernel, 4, sizeof(unsigned long), &matrix1->col_size);
    if(CL_err != CL_SUCCESS){
        puts("Cannot set args");
        return 1;
    }
    work_size_g[0] = matrix1->row_size;
    work_size_g[1] = matrix1->col_size;
    return 0;
}

int write_kernel_data(Matrix *matrix1, Matrix *matrix2){
    cl_int CL_err = CL_SUCCESS;
    cl_event write_events[2];
    CL_err = clEnqueueWriteBuffer(cq, buffer_a, CL_FALSE, 0, matrix1->row_size * matrix1->col_size * sizeof(int), matrix1->data, 0, NULL, &write_events[0]);
    if(CL_err != CL_SUCCESS){
        puts("Cannot write to buffer A");
        return 1;
    }
    CL_err = clEnqueueWriteBuffer(cq, buffer_b, CL_FALSE, 0, matrix2->row_size * matrix2->col_size * sizeof(int), matrix2->data, 0, NULL, &write_events[1]);
    if(CL_err != CL_SUCCESS){
        puts("Cannot write to buffer B");
        return 1;
    }
    CL_err = clWaitForEvents(2, write_events);
    if(CL_err != CL_SUCCESS){
        puts("Cannot wait for write events");
        return 1;
    }
}

int run_kernel(){
    cl_int CL_err = CL_SUCCESS;
    int a[64], b[64], c[64] = {0};
    for(int i = 0; i < 64; i++){
        a[i] = b[i] = i;
    }
    cl_event kernel_event;
    CL_err = clEnqueueNDRangeKernel(cq, kernel, 2, NULL, work_size_g, work_size_l, 0, NULL, &kernel_event);
    if(CL_err != CL_SUCCESS){
        puts("Cannot run kernel");
        return 1;
    }
    CL_err = clWaitForEvents(1, &kernel_event);
    if(CL_err != CL_SUCCESS){
        puts("Cannot wait for kernel event");
        return 1;
    }
    return 0;
}

void clear_kernel(){
    __clear_buffers();
    if(cq) clReleaseCommandQueue(cq);
    if(kernel) clReleaseKernel(kernel);
    if(program) clReleaseProgram(program);
    if(context) clReleaseContext(context);
}

int read_kernel_data(Matrix *result){
    cl_int CL_err = CL_SUCCESS;
    cl_event read_event;
    CL_err = clEnqueueReadBuffer(cq, buffer_c, CL_TRUE, 0, result->row_size * result->col_size * sizeof(int), result->data, 0, NULL, &read_event);
    if(CL_err != CL_SUCCESS){
        puts("Cannot read from buffer");
        return 1;
    }
    CL_err = clWaitForEvents(1, &read_event);
    if(CL_err != CL_SUCCESS){
        puts("Cannot wait for read event");
        return 1;
    }
    return 0;
}
