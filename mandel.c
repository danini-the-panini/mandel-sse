#include <stdio.h>

int main() {

  // Image

  const int width = 1280;
  const int height = 720;

  const int iterations = 100000;
  const float zoom = 150.0f;

  // Render

  printf("P3\n%i %i\n255\n", width, height);

  unsigned char buffer[width*height*3];

  #pragma omp parallel for schedule(dynamic, 1)
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      float zx = 0.0f;
      float zy = 0.0f;
      float cX = (x - width / 2.0f) / zoom;
      float cY = (y - height / 2.0f) / zoom;

      int iter = iterations;

      while (zx * zx + zy * zy < 4.0f && iter > 0) {
        float tmp = zx * zx - zy * zy + cX;
        zy = 2.0f * zx * zy + cY;
        zx = tmp;
        --iter;
      }

      int abc = iter | (iter << 8);

      unsigned char* src = (unsigned char*)&abc;

      int i = y*width+x;
      buffer[i*3+0] = src[2];
      buffer[i*3+1] = src[0];
      buffer[i*3+2] = src[1];
    }
  }

  for (int y = height-1; y >= 0; --y) {
    for (int x = 0; x < width; ++x) {
      int i = y*width+x;
      printf("%i %i %i\n",
        buffer[i*3+0],
        buffer[i*3+1],
        buffer[i*3+2]);
    }
  }
}
