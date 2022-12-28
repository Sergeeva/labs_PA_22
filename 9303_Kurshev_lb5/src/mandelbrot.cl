int compute_iterations(const float x0, const float y0, int num_iterations) {
    int i = 0;

    for(float x = x0, y = y0; (x * x + y * y <= 2 * 2) && i < num_iterations; i++) {
        float tmp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = tmp;
    }

    return i;
}

uint color(int total_iterations) {
     return 0x7 * total_iterations % 256 * 0x7;
}

__kernel void mandelbrot(
    float coeff,
    int max_iter,
    int width,
    int height,
    __global uint* result
) {
    int x = get_global_id(0);
    int y = get_global_id(1);

    if (x < width && y < height) {
        float new_x = coeff * (float)((x - width / 2)) / width;
        float new_y = coeff * (float)((y - height / 2)) / height;

        int iterations = compute_iterations(new_x, new_y, max_iter);
        result[y * width + x] = color(iterations);
    }
}
