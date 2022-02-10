#include"bmp.h"
#include<stdio.h>

struct bmp_image *read_bmp(FILE *input_file){
	
	struct bmp_header *header = read_bmp_header(input_file);
	
	if(header == NULL){
		printf("Error: This is not a BMP file.");
		return NULL;
	}
	
	struct bmp_pixel *data = read_data(input_file, header);
	
	if(data == NULL){
		printf("Error: Corrupted BMP file.");
		return NULL;
	}
	
	struct bmp_image *img = calloc(1, sizeof(struct bmp_image));

	img->header = header;
	img->pixels = data;

	return img;
}

struct bmp_header *read_bmp_header(FILE *input_file){
	if(input_file == NULL)
		return NULL;

	struct bmp_header *header = calloc(1, sizeof(struct bmp_header));

	fseek(input_file, 0, SEEK_SET);

	fread(&header->bfType, sizeof(header->bfType), 1, input_file);
	if(header->bfType != 0x4D42){
		free(header);
		return NULL;
	}
	fread(&header->bfSize, sizeof(header->bfSize), 1, input_file);
	fread(&header->bfReserved1, sizeof(header->bfReserved1), 1, input_file);
	fread(&header->bfReserved2, sizeof(header->bfReserved2), 1, input_file);
	fread(&header->bfOffBits, sizeof(header->bfOffBits), 1, input_file);
	fread(&header->biSize, sizeof(header->biSize), 1, input_file);
	fread(&header->biWidth, sizeof(header->biWidth), 1, input_file);
	fread(&header->biHeight, sizeof(header->biHeight), 1, input_file);
	fread(&header->biPlanes, sizeof(header->biPlanes), 1, input_file);
	fread(&header->biBitCount, sizeof(header->biBitCount), 1, input_file);
	fread(&header->biCompression, sizeof(header->biCompression), 1, input_file);
	fread(&header->biSizeImage, sizeof(header->biSizeImage), 1, input_file);
	fread(&header->biXPelsPerMeter, sizeof(header->biXPelsPerMeter), 1, input_file);
	fread(&header->biYPelsPerMeter, sizeof(header->biYPelsPerMeter), 1, input_file);
	fread(&header->biClrUsed, sizeof(header->biClrUsed), 1, input_file);
	fread(&header->biClrImportant, sizeof(header->biClrImportant), 1, input_file);
	
	return header;
}

struct bmp_pixel* read_data(FILE *input_file, const struct bmp_header *header){
	if(input_file == NULL)
		return NULL;

	uint32_t width = header->biWidth;
	uint32_t height = header->biHeight;

	struct bmp_pixel* data = calloc(width*height, sizeof(struct bmp_pixel));

	fseek(input_file, 54, SEEK_SET);
	
	uint32_t index = 0;
	uint32_t offset = (4-((width*3)%4))%4;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			fread(&(data[index].blue), 1, 1, input_file);
			fread(&(data[index].green), 1, 1, input_file);
			fread(&(data[index].red), 1, 1, input_file);
			++index;
		}
		fseek(input_file, offset, SEEK_CUR);
	}

	return data;
}

void free_bmp_image(struct bmp_image *img){
	if(img != NULL){
		if(img->header != NULL)
			free(img->header);
		if(img->pixels != NULL)
			free(img->pixels);
		free(img);
	}
}

bool write_bmp(FILE *output_file, const struct bmp_image *img){
	if(output_file == NULL)
		return 0;
	if(img == NULL)
		return 0;

	uint32_t width = img->header->biWidth;
	uint32_t height = img->header->biHeight;

	fwrite(&img->header->bfType, 1, sizeof(img->header->bfType), output_file);
	fwrite(&img->header->bfSize, 1, sizeof(img->header->bfSize), output_file);
	fwrite(&img->header->bfReserved1, 1, sizeof(img->header->bfReserved1), output_file);
	fwrite(&img->header->bfReserved2, 1, sizeof(img->header->bfReserved2), output_file);
	fwrite(&img->header->bfOffBits, 1, sizeof(img->header->bfOffBits), output_file);
	fwrite(&img->header->biSize, 1, sizeof(img->header->biSize), output_file);
	fwrite(&img->header->biWidth, 1, sizeof(img->header->biSize), output_file);
	fwrite(&img->header->biHeight, 1, sizeof(img->header->biWidth), output_file);
	fwrite(&img->header->biPlanes, 1, sizeof(img->header->biPlanes), output_file);
	fwrite(&img->header->biBitCount, 1, sizeof(img->header->biBitCount), output_file);
	fwrite(&img->header->biCompression, 1, sizeof(img->header->biCompression), output_file);
	fwrite(&img->header->biSizeImage, 1, sizeof(img->header->biSizeImage), output_file);
	fwrite(&img->header->biXPelsPerMeter, 1, sizeof(img->header->biXPelsPerMeter), output_file);
	fwrite(&img->header->biYPelsPerMeter, 1, sizeof(img->header->biYPelsPerMeter), output_file);
	fwrite(&img->header->biClrUsed, 1, sizeof(img->header->biClrUsed), output_file);
	fwrite(&img->header->biClrImportant, 1, sizeof(img->header->biClrImportant), output_file);

	uint32_t offset = (4-(width*3)%4)%4;
	uint32_t index = 0;

	for(int i = 0; i < height; ++i){
		for(int j = 0; j < width; ++j){
			fwrite(&img->pixels[index].blue, 1, 1, output_file);
			fwrite(&img->pixels[index].green, 1, 1, output_file);
			fwrite(&img->pixels[index].red, 1, 1, output_file);
			++index;
		}
		fwrite("\0", 1, offset, output_file);
	}

	return 1;
}
