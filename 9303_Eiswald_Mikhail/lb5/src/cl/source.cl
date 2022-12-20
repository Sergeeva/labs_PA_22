__constant char red[] = {66, 25, 9, 4, 0, 12, 24, 57, 134, 211, 241, 248, 255, 204, 153, 106};
__constant char green[] = {30, 7, 1, 4, 7, 44, 82, 125, 181, 236, 233, 201, 170, 128, 87, 52};
__constant char blue[] = {15, 26, 47, 73, 100, 138, 177, 209, 229, 248, 191, 95, 0, 0, 0, 3};

uint color__(int total_iterations) {
     return 0xf * total_iterations % 256 * 0xf;
}

__kernel void mandelbrot(int width, int height, int iterations, __global uchar* result) {
    int2 pos = (int2)(get_global_id(0), get_global_id(1));
    float x = 0, y = 0;
    float x0 = ((float) pos.x / width) * 3.25 - 2.5;
    float y0 = ((float) pos.y / height) * 2.0 - 1.0;
    float absolute = 0.0;
    int i = 0;
    while (i < iterations && absolute <= 2 * 2) {
        float x_prev = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = x_prev;

        absolute = x * x + y * y;
        i++;
    }
    uchar3 color;
    if (absolute < 2 * 2) {
        color.xyz = (uchar3)(0, 0, 0);
    } else {
        color.x = red[i % sizeof(red)];
        color.y = green[i % sizeof(green)];
        color.z = blue[i % sizeof(blue)];
    }
    result[4 * (pos.y * width + pos.x)] = color.x;
    result[4 * (pos.y * width + pos.x) + 1] = color.y;
    result[4 * (pos.y * width + pos.x) + 2] = color.z;
    result[4 * (pos.y * width + pos.x) + 3] = 255;
}
