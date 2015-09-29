#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

struct __attribute__((packed)) file_hdr {
    char magic[2];
    int32_t size;
    int16_t rsvd1;
    int16_t rsvd2;
    int32_t offset;
};

struct __attribute__((packed)) info_hdr_v1 {
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
};
    
struct __attribute__((packed)) pixel_rgb {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
    
struct __attribute__((packed)) pixel_rgba {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
};

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

void pixel_table_row(struct info_hdr_v1 *ihdr, uint32_t *block)
{
    uint32_t i;
    uint32_t count;
    struct pixel_rgba *palette;
    struct pixel_rgba *rgba;
    
    count = 0;
    palette = (struct pixel_rgba *)((char *)ihdr + sizeof(struct info_hdr_v1));
    
    do {
        for (i = 32 / ihdr->depth; i > 0; i--) {
            rgba = palette + palette_index(ihdr->depth, block, i);
            if (++count == ihdr->width)
                break;
            fprintf(stdout, "%02X%02X%02X%02X,", rgba->blue,
                                                 rgba->green,
                                                 rgba->red,
                                                 rgba->alpha);
        }
        block++;
    } while (count != ihdr->width);
    
    fprintf(stdout, "%02X%02X%02X%02X\n", rgba->blue,
                                          rgba->green,
                                          rgba->red,
                                          rgba->alpha);
    
    return;
}

void pixel_table_index(struct info_hdr_v1 *ihdr, uint32_t *row, int32_t pad)
{
    uint32_t i;
    
    for (i = 0; i < ihdr->height; i++) {
        pixel_table_row(ihdr, row);
        row += (ihdr->width * ihdr->depth / 32) + pad;
    }
    
    return;
}

void pixel_table_rgb(struct info_hdr_v1 *ihdr, uint32_t *row, int32_t pad)
{
    int32_t i;
    int32_t j;
    struct pixel_rgb *rgb;
    
    for (i = 0; i < ihdr->height; i++) {
        rgb = (struct pixel_rgb *)row;
        for (j = 0; j + 1 < ihdr->width; j++) {
            fprintf(stdout, "%02X%02X%02X,", rgb->blue,
                                             rgb->green,
                                             rgb->red);
            rgb++;
        }
        fprintf(stdout, "%02X%02X%02X\n", rgb->blue,
                                          rgb->green,
                                          rgb->red);
        row += (ihdr->width * ihdr->depth / 32) + pad;
    }
    
    return;
}

void bmp_info_hdr_v1(void *data, int32_t offset)
{
    int32_t pad;
    struct info_hdr_v1 *ihdr;
    
    ihdr = (struct info_hdr_v1 *)((char *)data + sizeof(struct file_hdr));
    if (0 == ihdr->width * ihdr->height) {
        fprintf(stderr, "Invalid dimension: %d x %d.\n", ihdr->width, ihdr->depth);
        return;
    }
    
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    switch (ihdr->depth) {
        case  1:
        case  2:
        case  4:
        case  8:
        case 16:
        case 32: pixel_table_index(ihdr, (uint32_t *)((char *)data + offset), pad);
                 break;
        case 24: pixel_table_rgb(ihdr, (uint32_t *)((char *)data + offset), pad);
                 break;
        default:
                 fprintf(stderr, "Color depth %d not supported yet.\n", ihdr->depth);
                 break;
    }
}

int main(int argc, char *argv[])
{
    char *path;
    FILE *file;
    void *data;
    int32_t size;

    struct file_hdr *fhdr;

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

    fhdr = (struct file_hdr *)data;
    if (0 != strncmp(fhdr->magic, "BM", 2) || size != fhdr->size) {
        fprintf(stderr, "Invalid BMP file.\n");
        free(data);
        fclose(file);
        return 5;
    }

    switch (*((int32_t *)((char *)data + sizeof(struct file_hdr)))) {
        case 40:
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
