uint getIteration(float x0, float y0, uint iterations){
    uint iteration = 0;
    float x = x0;
    float y = y0;
    while ((iteration != iterations) && (x * x + y * y <= 4.0f)) {
        float xtemp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        iteration++;
    }
    return iteration;
}

__kernel void get_point(float offsetX, float offsetY, float scale, uint iterations, int width, int height, __global uint* result){
    int i = get_global_id(0);
    int j = get_global_id(1);
    if (i < width && j < height){
        float x0 = offsetX + scale * (float)(i - width / 2) / width;
        float y0 = offsetY + scale * (float)(j - height / 2) / height;
        uint iteration = getIteration(x0, y0, iterations);
        result[j * width + i] = iteration % 256;
    }
}