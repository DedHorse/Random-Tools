#ifndef H_TRANSFORMATIONS
#define H_TRANSFORMATIONS
#include "bmp.h"

struct bmp_image *flip_horizontally(const struct bmp_image *input_img);

struct bmp_image *flip_vertically(const struct bmp_image *input_img);

struct bmp_image *rotate_right(const struct bmp_image *input_img);

struct bmp_image *rotate_left(const struct bmp_image *input_img);

struct bmp_image *crop(const struct bmp_image *input_img, uint32_t start_x, uint32_t start_y, uint32_t height_crop, uint32_t width_crop);

struct bmp_image *scale(const struct bmp_image *input_img, float factor);

struct bmp_image *extract(const struct bmp_image *input_img, int *colors_to_keep);

#endif
