#include "MandelbrotCPU.h"

MandelbrotCPU::MandelbrotCPU(int width, int height, unsigned int iterations, float scale, float offsetX, float offsetY): width_(width), height_(height), iterations_(iterations), scale_(scale), offsetX_(offsetX), offsetY_(offsetY){
    result_.resize(width*height);
}

void MandelbrotCPU::vectorToPPM(std::string filename) {
    std::ofstream file(filename, std::ios::binary);
    file << "P3\n" << width_ << ' ' << height_ << ' ' << 255 << std::endl;
    for (unsigned int i = 0; i < width_ * height_; i++){
        file << (255 - result_[i]) << ' ' << (255 - result_[i]) << ' ' << (255 - (result_[i] * 15) % 256) << std::endl;
    }
    file.close();
}

void MandelbrotCPU::mandelbrot(){
    auto start = std::chrono::high_resolution_clock::now();
    for (float j; j < height_; j++)
        for (float i = 0; i < width_; i++){
            float x0 = offsetX_ + scale_ * (float)(i - width_ / 2) / width_;
            float y0 = offsetY_ + scale_ * (float)(j - height_ / 2) / height_;
            float x = x0;
            float y = y0;
            unsigned int iteration = 0;
            while ((iteration != iterations_) && (x * x + y * y <= 4.0f)) {
                float xtemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xtemp;
                iteration++;
            }
            result_[j * width_ + i] = iteration % 256;
        }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Duration (CPU) - " << duration.count() << "ms" << std::endl;
    vectorToPPM("./CPU.ppm");
}