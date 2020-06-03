// Global dependencies
#include <stdlib.h>
#include <stdio.h>
#include <exception>
// Local dependencies
#include <models/imageModel.h>
#include <managers/imageManager.h>
#include <denoiser.h>

// Main method
int main(int argc, char** argv){

	const char* path_in;
	const char* path_out;

	if (argc < 3){
		printf("Invocar como: './main img_in img_out'.\n");
		exit (EXIT_FAILURE);
	} 
	
	// Read arguments
	path_in = argv[1];
	path_out = argv[2];    

	// Intiallize and read img_in and img_out
	ImageModel img_in, img_out;
	if (!read_image(path_in, img_in)) {
		printf("(Error) Cannot read input image.\n");
		exit (EXIT_FAILURE);
	}
	copy_image(img_in, img_out);

	// Apply DUDE to img_in, save result in img_out
	dude(img_in.getMatrix(), img_out.getMatrix());

	// Save matrix_out in path_out
	write_image(path_out, img_out);

	return 0;
}

