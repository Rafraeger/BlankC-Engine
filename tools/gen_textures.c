#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb/stb_image_write.h"
#include <stdlib.h>

void write_checkerboard(const char* path, int size) {
    unsigned char* pixels = malloc(size * size * 3);
    int block = size / 8;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int bx = (x / block) % 2;
            int by = (y / block) % 2;
            int idx = (y * size + x) * 3;
            if ((bx + by) % 2 == 0) {
                pixels[idx] = 180;
                pixels[idx+1] = 120;
                pixels[idx+2] = 80;
            } else {
                pixels[idx] = 100;
                pixels[idx+1] = 140;
                pixels[idx+2] = 180;
            }
        }
    }
    stbi_write_png(path, size, size, 3, pixels, size * 3);
    free(pixels);
}

void write_color(const char* path, int size, int r, int g, int b) {
    unsigned char* pixels = malloc(size * size * 3);
    for (int i = 0; i < size * size * 3; i += 3) {
        pixels[i] = r;
        pixels[i+1] = g;
        pixels[i+2] = b;
    }
    stbi_write_png(path, size, size, 3, pixels, size * 3);
    free(pixels);
}

int main(int argc, char* argv[]) {
    int size = 128;
    if (argc > 1) size = atoi(argv[1]);

    write_checkerboard("assets/textures/checkerboard.png", size);
    write_color("assets/textures/red.png", size, 200, 60, 60);
    write_color("assets/textures/green.png", size, 60, 180, 60);
    write_color("assets/textures/blue.png", size, 60, 60, 200);
    write_color("assets/textures/yellow.png", size, 220, 200, 60);

    printf("Textures generated: checkerboard, red, green, blue, yellow (%dx%d)\n", size, size);
    return 0;
}
