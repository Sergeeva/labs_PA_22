float nextX(float x, float y, float x0) {
    return x * x - y * y + x0;
}

float nextY(float x, float y, float y0) {
    return 2 * x * y + y0;
}

float cond(float x, float y) {
    return x * x + y * y;
}

int computeIterations(const float x0, const float y0, int iterations) {
    int curIteration = 0;

    for(float x = x0, y = y0; (cond(x, y) <= 4) && curIteration < iterations; curIteration++) {
        float temp = nextX(x, y, x0);
        y = nextY(x, y, y0);
        x = temp;
    }

    return curIteration;
}

uint color(int totalIterations) {
    return 0xf4 * totalIterations % 256 * 0xf4;
}

__kernel void mandelbrot(float coefficient, int height, int width, int maxIterations, __global uint* result) {
    int itemX = get_global_id(0);
    int itemY = get_global_id(1);

    if (itemX < width && itemY < height) {
        float x = coefficient * (float)((itemX - width/2)) / width;
        float y = coefficient * (float)((itemY - height/2)) / height;

        int totalIterations = computeIterations(x, y, maxIterations);
        result[itemY * width + itemX] = color(totalIterations);
    }
}
