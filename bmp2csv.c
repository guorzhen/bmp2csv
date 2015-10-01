#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct __attribute__((packed)) {
    char magic[2];
    int32_t size;
    int16_t rsvd1;
    int16_t rsvd2;
    int32_t offset;
} file_hdr;

typedef struct __attribute__((packed)) {
    int32_t size;
    int32_t width;
    int32_t height;
    int16_t plane;
    int16_t depth;
    int32_t compress;
    int32_t image;
    int32_t xres;
    int32_t yres;
    int32_t palette;
    int32_t color;
} info_hdr_v1;

typedef struct __attribute__((packed)) {
    uint16_t blue  : 4,
             green : 4,
             red   : 4,
             alpha : 4;
} pixel_u16;
    
typedef struct __attribute__((packed)) {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} pixel_u24;
    
typedef struct __attribute__((packed)) {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} pixel_u32;

#define RGB(px)  px->blue,px->green,px->red
#define RGBA(px) px->blue,px->green,px->red,px->alpha
#define RGB_FMT  "%02X%02X%02X00,"
#define RGB_END  "%02X%02X%02X00\n"
#define RGBA_FMT "%02X%02X%02X%02X,"
#define RGBA_END "%02X%02X%02X%02X\n"

#define PIXEL_TABLE_COLOR(type, color) do { \
    uint32_t i; \
    uint32_t j; \
    pixel_##type *pixel; \
    uint32_t *row = (uint32_t *)((char *)data + offset); \
    for (i = 0; i < ihdr->height; i++) { \
        pixel = (pixel_##type *)row; \
        for (j = 0; j + 1 < ihdr->width; j++) { \
            fprintf(stdout, color##_FMT, color(pixel)); \
            pixel++; \
        } \
        fprintf(stdout, color##_END, color(pixel)); \
        row += (ihdr->width * ihdr->depth / 32) + pad; \
    } \
} while (0)

uint32_t palette_index(int16_t depth, uint32_t *block, uint32_t i)
{
    uint32_t offset;
    uint32_t index;
    
    offset = depth * (i - 1);
    /* swap endian */
    index = ((*block >> 24) & 0xFF) |
            ((*block >>  8) & 0xFF00) |
            ((*block <<  8) & 0xFF0000) |
            ((*block << 24) & 0xFF000000);
    
    return (index & (((1 << depth) - 1) << offset)) >> offset;
}

void pixel_table_row(info_hdr_v1 *ihdr, pixel_u32 *palette, uint32_t *block)
{
    uint32_t i;
    uint32_t count;
    pixel_u32 *pixel;
    
    count = 0;
    
    do {
        for (i = 32 / ihdr->depth; i > 0; i--) {
            pixel = palette + palette_index(ihdr->depth, block, i);
            if (++count == ihdr->width)
                break;
            fprintf(stdout, RGBA_FMT, RGBA(pixel));
        }
        block++;
    } while (count != ihdr->width);
    
    fprintf(stdout, RGBA_END, RGBA(pixel));
    
    return;
}

void pixel_table_index(info_hdr_v1 *ihdr, uint32_t *row, int32_t pad)
{
    uint32_t i;
    pixel_u32 *palette;
    
    palette = (pixel_u32 *)((char *)ihdr + sizeof(info_hdr_v1));
    
    for (i = 0; i < ihdr->height; i++) {
        pixel_table_row(ihdr, palette, row);
        row += (ihdr->width * ihdr->depth / 32) + pad;
    }
    
    return;
}

void bmp_info_hdr_v1(void *data, int32_t offset)
{
    int32_t pad;
    info_hdr_v1 *ihdr;
    
    ihdr = (info_hdr_v1 *)((char *)data + sizeof(file_hdr));
    if (0 == ihdr->width * ihdr->height) {
        fprintf(stderr, "Invalid dimensions: %d x %d.\n", ihdr->width, ihdr->depth);
        return;
    }
    
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    switch (ihdr->depth) {
        case  1:
        case  2:
        case  4:
        case  8: pixel_table_index(ihdr, (uint32_t *)((char *)data + offset), pad);
                 break;
        case 16: PIXEL_TABLE_COLOR(u16, RGBA);
                 break;
        case 24: PIXEL_TABLE_COLOR(u24, RGB);
                 break;
        case 32: PIXEL_TABLE_COLOR(u32, RGBA);
                 break;
        default:
                 fprintf(stderr, "Color depth %d not supported yet.\n", ihdr->depth);
                 break;
    }
    
    return;
}

int main(int argc, char *argv[])
{
    char *path;
    FILE *file;
    void *data;
    int32_t size;

    file_hdr *fhdr;

    if (2 != argc) {
        fprintf(stderr, "Usage: ./bmp2csv file.bmp > file.csv\n");
        return 1;
    }
    path = argv[1];
    
    file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "File %s open failed.\n", path);
        return 2;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = calloc(1, size);
    if (!data) {
        fprintf(stderr, "Allocate memory of %d bytes failed.\n", size);
        fclose(file);
        return 3;
    }

    if (size != fread(data, 1, size, file)) {
        fprintf(stderr, "Incomplete read of file %s.\n", path);
        free(data);
        fclose(file);
        return 4;
    }

    fhdr = (file_hdr *)data;
    if (0 != strncmp(fhdr->magic, "BM", 2) || size != fhdr->size) {
        fprintf(stderr, "Invalid BMP file.\n");
        free(data);
        fclose(file);
        return 5;
    }

    switch (*((int32_t *)((char *)data + sizeof(file_hdr)))) {
        case sizeof(info_hdr_v1):
            bmp_info_hdr_v1(data, fhdr->offset);
            break;
        default:
            fprintf(stderr, "DIB version not supported yet.\n");
            free(data);
            fclose(file);
            return 7;
    }

    free(data);
    fclose(file);
    return 0;
}
