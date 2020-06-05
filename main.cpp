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

// Main method
int main(int argc, char** argv){

	// Aux return code
	short code = OK;

	if (argc < 3){
		error_msg(ERROR_MAIN_BAD_FORMATTING);
		exit (EXIT_FAILURE);
	} 
	
	// Read arguments
	const std::string path_in(argv[1]);
	const std::string path_out(argv[2]);   

	// Intiallize and read img_in and img_out
	ImageModel img_in, img_out;
	code = read_image(path_in, img_in); 
	if (code != OK) {
		error_msg(code);
		exit (EXIT_FAILURE);
	}
	copy_image(img_in, img_out);

	// Apply DUDE to img_in, save result in img_out
	dude(img_in.getMatrix(), img_out.getMatrix());

	// Save matrix_out in path_out
	code = write_image(path_out, img_out); 
	if (code != OK) {
		error_msg(code);
		exit (EXIT_FAILURE);
	}

	return 0;
}

