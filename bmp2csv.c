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
    
struct __attribute__((packed)) pixel_1b {
    uint8_t index : 1;
};
    
struct __attribute__((packed)) pixel_2b {
    uint8_t index : 2;
};

struct __attribute__((packed)) pixel_4b {
    uint8_t index : 4;
};
    
struct __attribute__((packed)) pixel_8b {
    uint8_t index : 8;
};
    
struct __attribute__((packed)) pixel_16b {
    uint16_t index : 16;
};
    
struct __attribute__((packed)) pixel_24b {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
};
    
struct __attribute__((packed)) pixel_32b {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
};

void pixel_table_1b (void *data, struct pixel_1b *pixel)
{
    int32_t i;
    int32_t j;
    int32_t pad;
    uint32_t *row;
    
    struct file_hdr *fhdr;
    struct info_hdr_v1 *ihdr;
    struct pixel_32b *palette;
    
    fhdr = (struct file_hdr *)data;
    ihdr = (struct info_hdr_v1 *)((char *)fhdr + sizeof(struct file_hdr));
    palette = (struct pixel_32b *)((char *)ihdr + sizeof(struct info_hdr_v1));
    row = (uint32_t *)pixel;
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    /* TODO */
    
    return;
}

void pixel_table_2b (void *data, struct pixel_2b *pixel)
{
    int32_t i;
    int32_t j;
    int32_t pad;
    uint32_t *row;
    
    struct file_hdr *fhdr;
    struct info_hdr_v1 *ihdr;
    struct pixel_32b *palette;
    
    fhdr = (struct file_hdr *)data;
    ihdr = (struct info_hdr_v1 *)((char *)fhdr + sizeof(struct file_hdr));
    palette = (struct pixel_32b *)((char *)ihdr + sizeof(struct info_hdr_v1));
    row = (uint32_t *)pixel;
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    /* TODO */
    
    return;
}

void pixel_table_4b (void *data, struct pixel_4b *pixel)
{
    int32_t i;
    int32_t j;
    int32_t pad;
    uint32_t *row;
    
    struct file_hdr *fhdr;
    struct info_hdr_v1 *ihdr;
    struct pixel_32b *palette;
    
    fhdr = (struct file_hdr *)data;
    ihdr = (struct info_hdr_v1 *)((char *)fhdr + sizeof(struct file_hdr));
    palette = (struct pixel_32b *)((char *)ihdr + sizeof(struct info_hdr_v1));
    row = (uint32_t *)pixel;
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    /* TODO */
    
    return;
}

void pixel_table_8b (void *data, struct pixel_8b *pixel)
{
    int32_t i;
    int32_t j;
    int32_t pad;
    uint32_t *row;
    
    struct file_hdr *fhdr;
    struct info_hdr_v1 *ihdr;
    struct pixel_32b *palette;
    
    fhdr = (struct file_hdr *)data;
    ihdr = (struct info_hdr_v1 *)((char *)fhdr + sizeof(struct file_hdr));
    palette = (struct pixel_32b *)((char *)ihdr + sizeof(struct info_hdr_v1));
    row = (uint32_t *)pixel;
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    for (i = 0; i < ihdr->height; i++) {
        for (j = 0; j + 1 < ihdr->width; j++) {
            fprintf(stdout, "%02X%02X%02X%02X,", (palette + pixel->index)->blue,
                                                 (palette + pixel->index)->green,
                                                 (palette + pixel->index)->red,
                                                 (palette + pixel->index)->alpha);
            pixel++;
        }
        fprintf(stdout, "%02X%02X%02X%02X\n", (palette + pixel->index)->blue,
                                              (palette + pixel->index)->green,
                                              (palette + pixel->index)->red,
                                              (palette + pixel->index)->alpha);
        row += (ihdr->width * ihdr->depth / 32) + pad;
        pixel = (struct pixel_8b *)row;
    }
    
    return;
}

void pixel_table_16b (void *data, struct pixel_16b *pixel)
{
    int32_t i;
    int32_t j;
    int32_t pad;
    uint32_t *row;
    
    struct file_hdr *fhdr;
    struct info_hdr_v1 *ihdr;
    struct pixel_32b *palette;
    
    fhdr = (struct file_hdr *)data;
    ihdr = (struct info_hdr_v1 *)((char *)fhdr + sizeof(struct file_hdr));
    palette = (struct pixel_32b *)((char *)ihdr + sizeof(struct info_hdr_v1));
    row = (uint32_t *)pixel;
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    for (i = 0; i < ihdr->height; i++) {
        for (j = 0; j + 1 < ihdr->width; j++) {
            fprintf(stdout, "%02X%02X%02X%02X,", (palette + pixel->index)->blue,
                                                 (palette + pixel->index)->green,
                                                 (palette + pixel->index)->red,
                                                 (palette + pixel->index)->alpha);
            pixel++;
        }
        fprintf(stdout, "%02X%02X%02X%02X\n", (palette + pixel->index)->blue,
                                              (palette + pixel->index)->green,
                                              (palette + pixel->index)->red,
                                              (palette + pixel->index)->alpha);
        row += (ihdr->width * ihdr->depth / 32) + pad;
        pixel = (struct pixel_16b *)row;
    }
    
    return;
}

void pixel_table_24b (void *data, struct pixel_24b *pixel)
{
    int32_t i;
    int32_t j;
    int32_t pad;
    uint32_t *row;
    
    struct file_hdr *fhdr;
    struct info_hdr_v1 *ihdr;
    
    fhdr = (struct file_hdr *)data;
    ihdr = (struct info_hdr_v1 *)((char *)fhdr + sizeof(struct file_hdr));
    row = (uint32_t *)pixel;
    pad = ((ihdr->width * ihdr->depth) % 32) ? 1 : 0;
    
    for (i = 0; i < ihdr->height; i++) {
        for (j = 0; j + 1 < ihdr->width; j++) {
            fprintf(stdout, "%02X%02X%02X,", pixel->blue,
                                             pixel->green,
                                             pixel->red);
            pixel++;
        }
        fprintf(stdout, "%02X%02X%02X\n", pixel->blue,
                                          pixel->green,
                                          pixel->red);
        row += (ihdr->width * ihdr->depth / 32) + pad;
        pixel = (struct pixel_24b *)row;
    }
    
    return;
}

void pixel_table_32b (void *data, struct pixel_32b *pixel)
{
    int32_t i;
    int32_t j;
    
    struct info_hdr_v1 *ihdr;
    
    ihdr = (struct info_hdr_v1 *)((char *)data + sizeof(struct file_hdr));
    
    for (i = 0; i < ihdr->height; i++) {
        for (j = 0; j + 1 < ihdr->width; j++) {
            fprintf(stdout, "%02X%02X%02X%02X,", pixel->blue,
                                                 pixel->green,
                                                 pixel->red,
                                                 pixel->alpha);
            pixel++;
        }
        fprintf(stdout, "%02X%02X%02X%02X\n", pixel->blue,
                                              pixel->green,
                                              pixel->red,
                                              pixel->alpha);
        pixel++;
    }
    
    return;
}

int main(int argc, char *argv[])
{
    char *path;
    FILE *file;
    void *data;
    int32_t size;

    struct file_hdr *fhdr;
    struct info_hdr_v1 *ihdr;

    if (2 != argc) {
        fprintf(stderr, "Usage: ./bmp2csv <path>\n");
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
            ihdr = (struct info_hdr_v1 *)((char *)data + sizeof(struct file_hdr));
            switch (ihdr->depth) {
                case  1: pixel_table_1b(data,  (struct pixel_1b  *)((char *)data + fhdr->offset));
                         break;
                case  2: pixel_table_2b(data,  (struct pixel_2b  *)((char *)data + fhdr->offset));
                         break;
                case  4: pixel_table_4b(data,  (struct pixel_4b  *)((char *)data + fhdr->offset));
                         break;
                case  8: pixel_table_8b(data,  (struct pixel_8b  *)((char *)data + fhdr->offset));
                         break;
                case 16: pixel_table_16b(data, (struct pixel_16b *)((char *)data + fhdr->offset));
                         break;
                case 24: pixel_table_24b(data, (struct pixel_24b *)((char *)data + fhdr->offset));
                         break;
                case 32: pixel_table_32b(data, (struct pixel_32b *)((char *)data + fhdr->offset));
                         break;
                default:
                         fprintf(stderr, "Color depth %d not supported yet.\n", ihdr->depth);
                         free(data);
                         fclose(file);
                         return 6;
            }
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
