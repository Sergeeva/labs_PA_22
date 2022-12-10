float next_x(float x, float y, float x0) {
    return x * x - y * y + x0;
}

float next_y(float x, float y, float y0) {
    return 2 * x * y + y0;
}

float cond(float x, float y) {
    return x * x + y * y;
}

int compute_iterations(const float x0, const float y0, int iterations) {
    int cur_iteration = 0;

    for(float x = x0, y = y0; (cond(x, y) <= 4) && cur_iteration < iterations; cur_iteration++) {
        float temp = next_x(x, y, x0);
        y = next_y(x, y, y0);
        x = temp;
    }

    return cur_iteration;
}

uint color(int total_iterations) {
     return 0xf * total_iterations % 256 * 0xf;
}

__kernel void mandelbrot(
    float coefficient,
    int max_iterations,
    int width,
    int height,
    __global uint* result
) {
    int item_x = get_global_id(0);
    int item_y = get_global_id(1);

    if (item_x < width && item_y < height) {
        float x = coefficient * (float)((item_x - width/2)) / width;
        float y = coefficient * (float)((item_y - height/2)) / height;

        int total_iterations = compute_iterations(x, y, max_iterations);
        result[item_y * width + item_x] = color(total_iterations);
    }
}
