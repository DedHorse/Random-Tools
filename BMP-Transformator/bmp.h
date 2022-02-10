#ifndef BMP_H
#define BMP_H

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdio.h>

struct bmp_header{

/*         FILE HEADER           */

	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;

/*         INFO HEADER           */

	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

struct bmp_pixel{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct bmp_image{
	struct bmp_header *header;
	struct bmp_pixel *pixels;
};

//get the header
struct bmp_header *read_bmp_header(FILE *input_file);

//get the pixel data
struct bmp_pixel *read_data(FILE *input_file, const struct bmp_header *header);

//unite the header and data as an image struct and return reference
struct bmp_image *read_bmp(FILE *input_file);

//new bmp file after transformations
bool write_bmp(FILE *input_file, const struct bmp_image *img);

//free the bmp image from memory
void free_bmp_image(struct bmp_image *img);

#endif
