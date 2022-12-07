
uint3 get_color(uint n, uint iter_max, uint boundary) {
    uint r = (n + boundary * 3) % 255 ;
    uint g = (n * iter_max / boundary) % 255;
    uint b = ((boundary * iter_max / n + 4000) - n) % 255;
    return (uint3)(r,g , b);
};

__kernel void mandelbrot( __write_only image2d_t out,
                         uint iterations) {
    int2 pos = (int2)(get_global_id(0), get_global_id(1));
    int2 size = get_image_dim(out);
    const double xMin = -2;
    const double yMax = 1;
    double dx = 3.4;
    double dy = 2;

    float x0 = ((float) pos.x / size.x) * dx + xMin;
    float y0 = ((float) pos.y / size.y) * dy - yMax;
    float x = 0.0;
    float y = 0.0;

    uint iteration = 0;
    float bounded = x * x + y * y;;

    while (iteration < iterations) {
        float x_prev = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = x_prev;
        iteration++;
        bounded = x * x + y * y;
        if (bounded >= 4)
                break;
    }

    uint4 color = (255, 255, 255, 255);

    if (bounded < 4.0) {
        color.xyz = (uint3)(0, 0, 0);
    } else {
        color.xyz = get_color(iteration, iterations, bounded);
    }
    write_imageui(out, pos, color);
}
