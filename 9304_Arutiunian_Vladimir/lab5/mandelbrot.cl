bool is_not_tend_to_infinity(const float x, const float y) {
    return x * x + y * y <= 4;
}

bool is_not_last_iteration(const float x, const float y, const int iteration, const int iterations_limit) {
    return is_not_tend_to_infinity(x, y) && iteration < iterations_limit;
}

float get_next_x(const float x, const float y, const float center_x) {
    return x * x - y * y + center_x;
}

float get_next_y(const float x, const float y, const float center_y) {
    return 2 * x * y + center_y;
}

int compute_iterations(const float center_x, const float center_y, int iterations_limit) {
    int iteration = 0;
    float x = center_x;
    float y = center_y;

    while (is_not_last_iteration(x, y, iteration, iterations_limit)) {
        float x_temp = get_next_x(x, y, center_x);
        y = get_next_y(x, y, center_y);
        x = x_temp;

        ++iteration;
    }

    return iteration;
}

uint to_gray_color(int iterations, int iterations_limit) {
    int max_color_value = 0xFF;
    int pos = max_color_value * iterations / iterations_limit;

    return pos | (pos << 8) | (pos << 16) | (max_color_value << 24);
}

__kernel void compute_mandelbrot(
    int height,
    int width,
    float ratio,
    int iterations_limit,
    __global uint* result_buffer
) {
    int local_x = get_global_id(0);
    int local_y = get_global_id(1);

    if (local_x < width && local_y < height) {
        float x = ratio * (float) (local_x - width / 2) / width;
        float y = ratio * (float) (local_y - height / 2) / width;

        int iteration = compute_iterations(x, y, iterations_limit);
        int pixel_index = local_y * width + local_x;
        result_buffer[pixel_index] = to_gray_color(iteration, iterations_limit);
    }
}
