// Global dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <filesystem>

// Local dependencies
#include <denoiser.h>
#include <models/imageModel.h>
#include <managers/imageManager.h>
#include <handlers/fileHandler.h>
#include <handlers/errorHandler.h>
#include <const.h>

namespace fs = std::filesystem;

// Main method
int main(int argc, char** argv){

	// Aux return code
	short code = OK;

	const float delta = 0.05f;
	const short k = 1;

	const std::string global_path_in = "data/input/img_noisy/salt-and-pepper/";
	const std::string global_path_out = "data/output/";
	
	for (const auto & entry : fs::directory_iterator(global_path_in)) {
		
		//
		ImageModel img_in, img_prefiltered, img_out;
		
		//
		code = read_image(entry.path().string(), img_in); 
		if (code != OK) {
			error_msg(code);
			exit (EXIT_FAILURE);
		}

		//
		copy_image(img_in, img_out);

		//
		// dude(delta, k, img_in, img_prefiltered, img_out);

		//
		std::string path_out = global_path_out + getFileName(entry.path().string());

		// Save matrix_out in path_out
		code = write_image(path_out, img_in); 
		if (code != OK) {
			error_msg(code);
			exit (EXIT_FAILURE);
		}
		
	}

	return 0;
}

