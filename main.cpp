// Global dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string>
// Local dependencies
#include <denoiser.h>
#include <models/imageModel.h>
#include <managers/imageManager.h>
#include <handlers/errorHandler.h>
#include <const.h>

// TODO: Change images from float to char
// TODO: Change reader so that it ignores comments on images

// Main method
int main(int argc, char** argv){

	// Aux return code
	short code = OK;

	if (argc < 5) {
		error_msg(ERROR_MAIN_BAD_FORMATTING);
		exit (EXIT_FAILURE);
	}
	
	// Read arguments
	const float delta = atof(argv[1]);
	const short k = atoi(argv[2]);
	const std::string path_in(argv[3]);
	const std::string path_out(argv[4]);
	
	std::string path_prefiltered;
	if(argc == 6) {
		path_prefiltered = argv[5];
	}

	// Intiallize and read img_in and img_out
	ImageModel img_in, img_prefiltered, img_out;
	code = read_image(path_in, img_in); 
	if (code != OK) {
		error_msg(code);
		exit (EXIT_FAILURE);
	}
	copy_image(img_in, img_out);

	// If path_prefiltered is not NULL load img_prefiltered
	if(argv[5]) {
		code = read_image(path_prefiltered, img_prefiltered); 
		if (code != OK) {
			error_msg(code);
			exit (EXIT_FAILURE);
		}
	}

	// Apply DUDE to img_in, save result in img_out
	dude(delta, k, img_in, img_prefiltered, img_out);

	/*unsigned char **matrix_in = img_in.getMatrix();
	unsigned char **matrix_out = img_out.getMatrix();
	short height = img_in.getHeight();
	short width = img_in.getWidth();
	for (short i = 0; i < height; ++i) {
			for (short j = 0; j < width; ++j) {
					//printf("%i, ", val);
					if (matrix_out[i][j] != matrix_in[i][j]) {
							printf("%i, changed to: %i \n", matrix_in[i][j], matrix_out[i][j]);
					}
			}
			//printf("\n");
	}*/

	// Save matrix_out in path_out
	code = write_image(path_out, img_out); 
	if (code != OK) {
		error_msg(code);
		exit (EXIT_FAILURE);
	}

	return 0;
}

