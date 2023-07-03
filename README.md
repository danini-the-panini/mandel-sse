# Mandelbrot SSE

Based off: https://github.com/skeeto/mandel-simd

## How do (instruction for ubuntu/wsl)

1. clone

    `git clone git@github.com:danini-the-panini/mandel-sse`

2. make sure you have build stuffs

    `sudo apt install build-essential`

3. make sure you have OpenMP

    `sudo apt install libomp-dev`

4. build

    `make`

5. run

    `./mandel > out.ppm`

    or

    `./mandel_simd > out.ppm`

6. use `feh` to open `ppm` files

    `sudo apt install feh`

    `./mandel_simd | feh -`
