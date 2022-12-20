#include <cmath>

#include "mandelbrot_cpu.h"

void mandelbrotCPU(float* results,
		   unsigned int width, unsigned int height,
		   float fromX, float fromY,
		   float sizeX, float sizeY,
		   unsigned int iters, bool smoothing){
  
  const float threshold = 2.0f;
  const float threshold2 = threshold * threshold;

  for (int j = 0; j < height; ++j){
    for (int i = 0; i < width; ++i){
      
      float x0 = fromX + (i + 0.5f) * sizeX / width;
      float y0 = fromY + (j + 0.5f) * sizeY / height;
      float x = x0;
      float y = y0;
      
      int iter = 0;
      for (; iter < iters; ++iter) {
	float xPrev = x;
	x = x * x - y * y + x0;
	y = 2.0f * xPrev * y + y0;

	if ((x * x + y * y) > threshold2){
	  break;
	}
      }

      float result = iter;

      if (smoothing && iter != iters){
	result = result - logf(logf(sqrtf(x * x + y * y)) / logf(threshold)) / logf(2.0f);
      }
      
      result = 1.0f * result / iters;
      results[j * width + i] = result;
      
    }//end for i
  }//end for j

}



