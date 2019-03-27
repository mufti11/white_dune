#include "pngLoad.h"
#include "stdio.h"
#include "stdlib.h"
#include "png.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file.png\n", argv[0]);
        exit(1);
    }
    int width, height, components;
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        myperror(argv[1]);
        return 1;
    }
    unsigned char *data = pngread(file, &width, &height, &components);
    if (data == NULL) {
        fprintf(stderr, "unexpected error 8-(\n");
        return 1;
    }
    printf("PixelTexture {\n");
    printf("  image\n");
    printf("    %d\n", width);
    printf("    %d\n", height);
    printf("    %d\n", components);
    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++) {
            int index = components * ((j * width) + i);
            unsigned int color = 0;
            for (int c = 0; c < components; c++)
                color = color + (data[index + components - c - 1] << (c * 8));
            printf("    0x%x\n", color);
        }
    printf("}\n");
}
