int computeIterations(const float x0, const float y0, int iterations) {
    int n = 0;
    for (float x = x0, y = y0; (x * x + y * y <= 2 * 2) && (n < iterations); n++) {
        float temp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = temp;
    }
    return n;
}

uint color(int totalIterations, float maxIterations) {
    float minIterations = maxIterations - 50;
    int position = 0xff * max(0.f, totalIterations - minIterations) / (maxIterations - minIterations);
    return position | (position << 8) | (position << 16) | (0xff << 24);
}

__kernel void draw(float xPixel,
                   float yPixel,
                   int width,
                   int height,
                   float scale,
                   float maxIterations,
                   int stepResult,
                   volatile __global uint *result) {
    int xWorkItem = get_global_id(0);
    int yWorkItem = get_global_id(1);

    if (xWorkItem < width && yWorkItem < height) {
        float x = xPixel + scale * (float) (xWorkItem - width / 2) / width;
        float y = yPixel + scale * (float) (yWorkItem - height / 2) / width;

        int totalIterations = computeIterations(x, y, (int) maxIterations);
        result[yWorkItem * stepResult + xWorkItem] = color(totalIterations, maxIterations);
    }
}