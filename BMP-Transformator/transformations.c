#include"transformations.h"
#include<stdlib.h>

struct bmp_image *flip_vertically(const struct bmp_image *input_img){
	if(input_img == NULL)
		return NULL;

	struct bmp_image *output_img = calloc(1, sizeof(struct bmp_image));
	struct bmp_header *header = calloc(1, sizeof(struct bmp_header));

	output_img->header = header;

	output_img->header->bfType = input_img->header->bfType;
	output_img->header->bfSize = input_img->header->bfSize;
	output_img->header->bfReserved1 = input_img->header->bfReserved1;
	output_img->header->bfReserved2 = input_img->header->bfReserved2;
	output_img->header->bfOffBits = input_img->header->bfOffBits;
	output_img->header->biSize = input_img->header->biSize;
	output_img->header->biWidth = input_img->header->biWidth;
	output_img->header->biHeight = input_img->header->biHeight;
	output_img->header->biPlanes = input_img->header->biPlanes;
	output_img->header->biBitCount = input_img->header->biBitCount;
	output_img->header->biCompression = input_img->header->biCompression;
	output_img->header->biSizeImage = input_img->header->biSizeImage;
	output_img->header->biXPelsPerMeter = input_img->header->biXPelsPerMeter;
	output_img->header->biYPelsPerMeter = input_img->header->biYPelsPerMeter;
	output_img->header->biClrUsed = input_img->header->biClrUsed;
	output_img->header->biClrImportant = input_img->header->biClrImportant;

	uint32_t width = output_img->header->biWidth;
	uint32_t height = output_img->header->biHeight;

	struct bmp_pixel* pixels = calloc(width*height, sizeof(struct bmp_pixel));

	uint32_t index = 0;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			pixels[index].red = input_img->pixels[(i*width-1)+width-j].red;
			pixels[index].green = input_img->pixels[(i*width-1)+width-j].green;
			pixels[index].blue = input_img->pixels[(i*width-1)+width-j].blue;

			++index;
		}
	}
	
	output_img->pixels = pixels;

	int offset = (width*3)%4;
	if(offset != 0)
		offset = 4-offset;
	

	output_img->header->bfSize = 54 + width*height*3 + offset*height;
	output_img->header->biSizeImage = output_img->header->biSize - 54;

	return output_img;
}



struct bmp_image *flip_horizontally(const struct bmp_image *input_img){
	if(input_img == NULL)
		return NULL;

	struct bmp_image *output_img = calloc(1, sizeof(struct bmp_image));
	struct bmp_header *header = calloc(1, sizeof(struct bmp_header));

	output_img->header = header;

	output_img->header->bfType = input_img->header->bfType;
	output_img->header->bfSize = input_img->header->bfSize;
	output_img->header->bfReserved1 = input_img->header->bfReserved1;
	output_img->header->bfReserved2 = input_img->header->bfReserved2;
	output_img->header->bfOffBits = input_img->header->bfOffBits;
	output_img->header->biSize = input_img->header->biSize;
	output_img->header->biWidth = input_img->header->biWidth;
	output_img->header->biHeight = input_img->header->biHeight;
	output_img->header->biPlanes = input_img->header->biPlanes;
	output_img->header->biBitCount = input_img->header->biBitCount;
	output_img->header->biCompression = input_img->header->biCompression;
	output_img->header->biSizeImage = input_img->header->biSizeImage;
	output_img->header->biXPelsPerMeter = input_img->header->biXPelsPerMeter;
	output_img->header->biYPelsPerMeter = input_img->header->biYPelsPerMeter;
	output_img->header->biClrUsed = input_img->header->biClrUsed;
	output_img->header->biClrImportant = input_img->header->biClrImportant;

	uint32_t width = output_img->header->biWidth;
	uint32_t height = output_img->header->biHeight;

	struct bmp_pixel* pixels = calloc(width*height, sizeof(struct bmp_pixel));

	uint32_t index = 0;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			pixels[index].red = input_img->pixels[(width)*(height-1-i)+j].red;
			pixels[index].green = input_img->pixels[(width)*(height-1-i)+j].green;
			pixels[index].blue = input_img->pixels[(width)*(height-1-i)+j].blue;

			++index;
		}
	}
	
	output_img->pixels = pixels;

	int offset = (width*3)%4;
	if(offset != 0)
		offset = 4-offset;
	

	output_img->header->bfSize = 54 + width*height*3 + offset*height;
	output_img->header->biSizeImage = output_img->header->biSize - 54;

	return output_img;
}


struct bmp_image *rotate_left(const struct bmp_image *input_img){
	if(input_img == NULL)
		return NULL;

	struct bmp_image *output_img = calloc(1, sizeof(struct bmp_image));
	struct bmp_header *header = calloc(1, sizeof(struct bmp_header));

	output_img->header = header;

	output_img->header->bfType = input_img->header->bfType;
	output_img->header->bfSize = input_img->header->bfSize;
	output_img->header->bfReserved1 = input_img->header->bfReserved1;
	output_img->header->bfReserved2 = input_img->header->bfReserved2;
	output_img->header->bfOffBits = input_img->header->bfOffBits;
	output_img->header->biSize = input_img->header->biSize;
	output_img->header->biWidth = input_img->header->biHeight;
	output_img->header->biHeight = input_img->header->biWidth;
	output_img->header->biPlanes = input_img->header->biPlanes;
	output_img->header->biBitCount = input_img->header->biBitCount;
	output_img->header->biCompression = input_img->header->biCompression;
	output_img->header->biSizeImage = input_img->header->biSizeImage;
	output_img->header->biXPelsPerMeter = input_img->header->biYPelsPerMeter;
	output_img->header->biYPelsPerMeter = input_img->header->biXPelsPerMeter;
	output_img->header->biClrUsed = input_img->header->biClrUsed;
	output_img->header->biClrImportant = input_img->header->biClrImportant;

	uint32_t width = input_img->header->biWidth;
	uint32_t height = input_img->header->biHeight;

	struct bmp_pixel* pixels = calloc(width*height, sizeof(struct bmp_pixel));

	uint32_t index = 0;

	int x;
	int y;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			x = height-i-1;
			y = j;
			
			pixels[y*height+x].red = input_img->pixels[index].red;
			pixels[y*height+x].green = input_img->pixels[index].green;
			pixels[y*height+x].blue = input_img->pixels[index].blue;

			++index;
		}
	}
	
	output_img->pixels = pixels;

	int offset = (width*3)%4;
	if(offset != 0)
		offset = 4-offset;
	

	output_img->header->bfSize = 54 + width*height*3 + offset*height;
	output_img->header->biSizeImage = output_img->header->biSize - 54;

	return output_img;
}

struct bmp_image *rotate_right(const struct bmp_image *input_img){
	if(input_img == NULL)
		return NULL;

	struct bmp_image *output_img = calloc(1, sizeof(struct bmp_image));
	struct bmp_header *header = calloc(1, sizeof(struct bmp_header));

	output_img->header = header;

	output_img->header->bfType = input_img->header->bfType;
	output_img->header->bfSize = input_img->header->bfSize;
	output_img->header->bfReserved1 = input_img->header->bfReserved1;
	output_img->header->bfReserved2 = input_img->header->bfReserved2;
	output_img->header->bfOffBits = input_img->header->bfOffBits;
	output_img->header->biSize = input_img->header->biSize;
	output_img->header->biWidth = input_img->header->biHeight;
	output_img->header->biHeight = input_img->header->biWidth;
	output_img->header->biPlanes = input_img->header->biPlanes;
	output_img->header->biBitCount = input_img->header->biBitCount;
	output_img->header->biCompression = input_img->header->biCompression;
	output_img->header->biSizeImage = input_img->header->biSizeImage;
	output_img->header->biXPelsPerMeter = input_img->header->biYPelsPerMeter;
	output_img->header->biYPelsPerMeter = input_img->header->biXPelsPerMeter;
	output_img->header->biClrUsed = input_img->header->biClrUsed;
	output_img->header->biClrImportant = input_img->header->biClrImportant;

	uint32_t width = input_img->header->biWidth;
	uint32_t height = input_img->header->biHeight;

	struct bmp_pixel* pixels = calloc(width*height, sizeof(struct bmp_pixel));

	uint32_t index = 0;

	int x;
	int y;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			x = i;
			y = width-j-1;
			
			pixels[y*height+x].red = input_img->pixels[index].red;
			pixels[y*height+x].green = input_img->pixels[index].green;
			pixels[y*height+x].blue = input_img->pixels[index].blue;

			++index;
		}
	}
	
	output_img->pixels = pixels;

	int offset = (width*3)%4;
	if(offset != 0)
		offset = 4-offset;
	

	output_img->header->bfSize = 54 + width*height*3 + offset*height;
	output_img->header->biSizeImage = output_img->header->biSize - 54;

	return output_img;
}
