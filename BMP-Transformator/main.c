#include"bmp.c"
#include"transformations.c"
#include<string.h>

int main(int argc, char**argv){
	if(argc < 4)
		return 1;
	char cmds[8];
	cmds[0] = '\0';
	int cmds_index = 0;
	int input_file_index = 0;
	int output_file_index = 0;
	for(int i = 1; i < argc; ++i){
		if(strcmp("-h", argv[i]) == 0){
			cmds[cmds_index] = 'h';
			cmds[++cmds_index]='\0';
		}

		if(strcmp("-l", argv[i]) == 0){
			cmds[cmds_index] = 'l';
			cmds[++cmds_index] = '\0';
		}
		if(strcmp("-r", argv[i]) == 0){
			cmds[cmds_index] = 'r';
			cmds[++cmds_index] = '\0';
		}
		if(strcmp("-v", argv[i]) == 0){
			cmds[cmds_index] = 'v';
			cmds[++cmds_index] = '\0';
		}
		if(strcmp("-c", argv[i]) == 0)
			printf("WIP\n"); //crop and add 4 to i since those are params for crop

		if(strcmp("-s", argv[i]) == 0)
			printf("WIP\n"); //scale and add 1 cuz next the scale param

		if(strcmp("-e", argv[i]) == 0)
			printf("WIP\n"); //extract colours and add 1 cuz u know the drill

		if(strcmp("-o", argv[i]) == 0){
				output_file_index = i+1;
		}
		if(strcmp("-i", argv[i]) == 0){
				input_file_index = i+1;
			}
	}

	FILE *in_img = fopen(argv[input_file_index], "r");

	if(in_img == NULL){
		return 1;
	}

	struct bmp_image *img = read_bmp(in_img);

	struct bmp_image *img_out = NULL;

	int i = 0;
	while(cmds[i] != '\0'){
		switch(cmds[i]){
			case 'h':
				if(i == 0)
					img_out = flip_horizontally(img);
				else
					img_out = flip_horizontally(img_out);
				break;
			case 'v':
				if(i == 0)
					img_out = flip_vertically(img);
				else
					img_out = flip_vertically(img_out);
				break;
			case 'l':
				if(i == 0)
					img_out = rotate_left(img);
				else
					img_out = rotate_left(img_out);
				break;;
			case 'r':
				if(i == 0)
					img_out = rotate_right(img);
				else
					img_out = rotate_left(img_out);
				break;
		}
		++i;
	}

	fclose(in_img);

	FILE *out_file = fopen(argv[output_file_index], "wb");

	if(out_file == NULL)
		return 1;

	write_bmp(out_file, img_out);

	fclose(out_file);

	free_bmp_image(img);
	free_bmp_image(img_out);
	return 0;
}
