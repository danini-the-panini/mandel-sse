#include <xmmintrin.h>
#include <stdio.h>

int main() {

  // Image

  const int width = 1280;
  const int height = 720;
  const __m128 half_width = _mm_set1_ps(width / 2.0f);
  const __m128 half_height = _mm_set1_ps(height / 2.0f);

  const int iterations = 92682;
  const __m128 zoom = _mm_set1_ps(150.0f);

  const __m128 threshold = _mm_set1_ps(4);
  const __m128i one = _mm_set1_epi32(1);

  // Render

  printf("P3\n%i %i\n255\n", width, height);

  unsigned char buffer[width*height*3];

  #pragma omp parallel for schedule(static, 1)
  for (int y = height-1; y >= 0; --y) {
    for (int x = 0; x < width; x += 4) {
      __m128 mx = _mm_set_ps(x + 3, x + 2, x + 1, x + 0);
      __m128 my = _mm_set1_ps(y);

      __m128 zx = _mm_set1_ps(0.0f);
      __m128 zy = _mm_set1_ps(0.0f);
      __m128 cX = _mm_div_ps(_mm_sub_ps(mx, half_width), zoom);
      __m128 cY = _mm_div_ps(_mm_sub_ps(my, half_height), zoom);

      int iter = iterations;
      __m128i m_iter = _mm_set1_epi32(iter);

      __m128 zx2 = _mm_mul_ps(zx, zx);
      __m128 zy2 = _mm_mul_ps(zy, zy);
      while (iter > 0) {
        // tmp = zx * zx - zy * zy + cX
        __m128 tmp = _mm_add_ps(_mm_sub_ps(zx2, zy2), cX);
        __m128 zy1 = _mm_mul_ps(zx, zy);
        zy = _mm_add_ps(_mm_add_ps(zy1, zy1), cY);
        zx = tmp;

        // zx * zx + zy * zy < 4.0f
        zx2 = _mm_mul_ps(zx, zx);
        zy2 = _mm_mul_ps(zy, zy);
        __m128 mask = _mm_cmplt_ps(_mm_add_ps(zx2, zy2), threshold);
        m_iter = _mm_sub_epi32(m_iter, _mm_and_si128(_mm_castps_si128(mask), one));

        if (_mm_movemask_ps(mask) == 0) break;

        --iter;
      }

      __m128i m_abc = _mm_or_si128(m_iter, _mm_slli_epi32(m_iter, 8));

      unsigned char *abc = (unsigned char *)&m_abc;

      int i = y*width+x;
      for (int b = 0; b < 4; b++) {
        buffer[(i+b)*3+0] = abc[b*4+2];
        buffer[(i+b)*3+1] = abc[b*4+0];
        buffer[(i+b)*3+2] = abc[b*4+1];
      }
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
