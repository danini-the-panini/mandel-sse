CFLAGS = -std=c99 -Wall -Wextra -Ofast -fopenmp

all: mandel mandel_simd

mandel : mandel.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

mandel_simd : mandel_simd.c
	$(CC) $(CFLAGS) $(LDFLAGS) -msse2 -o $@ $^ $(LDLIBS)

clean :
	$(RM) mandel mandel_simd

.PHONY : clean
