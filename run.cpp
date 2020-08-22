// Global dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <map>

// Local dependencies
#include <denoiser.h>
#include <models/imageModel.h>
#include <managers/imageManager.h>
#include <managers/dataManager.h>
#include <handlers/fileHandler.h>
#include <handlers/logHandler.h>
#include <handlers/stringHandler.h>
#include <handlers/commandHandler.h>
#include <const.h>

namespace fs = std::filesystem;

// Main method
int main(int argc, char** argv) {

	// Aux return code
	short code = OK;

	if (argc < 2) {
		error_msg(ERROR_MAIN_BAD_FORMATTING);
		exit (EXIT_FAILURE);
	}

	// Read arguments
	const short op = atoi(argv[1]);
	run_msg(op);

	// Define directories' path constants
	const std::string global_path_in = "data/input/img_noisy/salt-and-pepper/";
	const std::string global_path_out = "data/output/";
	const std::string global_path_original = "data/input/img_clean/";
	const std::string global_path_results = "data/results/";
	
	// Clean and create directory for results
	remove_directory(global_path_results);
	add_directory(global_path_results);

	// Map for best K for each image
	std::map<std::string, short> best_contexts;
	best_contexts["Img07S0.05"] = 2;
	best_contexts["Img07S0.10"] = 2;
	best_contexts["Img07S0.35"] = 1;
	best_contexts["Img20S0.05"] = 4;
	best_contexts["Img20S0.10"] = 6;
	best_contexts["Img20S0.35"] = 0;
	best_contexts["Img99S0.05"] = 4;
	best_contexts["Img99S0.10"] = 5;
	best_contexts["Img99S0.35"] = 1;

	switch (op) {

		case 1:
			for (const auto & entry : fs::directory_iterator(global_path_in)) {
				
				// Create auxiliary variables
				ImageModel img_in, img_prefiltered, img_out;

				// Get current image's file name
				const std::string file_path = entry.path().string();
				const std::string file_name = get_file_name(file_path);
				log_msg("");
				log_msg("Running DUDE over image " + file_name);

				// Get current image's params
				const std::string root = parse_base(file_name);
				const std::string base = parse_extension(file_name);
				const float delta = parse_delta(file_name);

				// Create directory if it doesn't exist for saving image's denoised version
				add_directory(global_path_out + base);
				add_directory(global_path_results + root);
				
				// Read current image and copy it for out image
				code = read_image(file_path, img_in); 
				if (code != OK) {
					error_msg(code);
					exit (EXIT_FAILURE);
				}
				
				// Iterate over k to generate each result
				for (short k = 0; k < 9; k++) {

					// Get img_in once more
					copy_image(img_in, img_out);

					// Execute DUDE with given params
					log_msg("Image " + file_name + " - Context K = " + std::to_string(k));
					dude(delta, k, img_in, img_prefiltered, img_out);

					// Format current image's denoised version file name
					std::string path_out = global_path_out + base + "/" + "K" + std::to_string(k) + file_name;
					log_msg("Image " + file_name + " - Result path = " + path_out);

					// Write current image's denoised version
					code = write_image(path_out, img_out); 
					if (code != OK) {
						error_msg(code);
						exit (EXIT_FAILURE);
					}
					
					// Execute diff between original image and denoised image
					const std::string original_path = global_path_original + root + ".pgm";
					const std::string diff_cmd = "./bin/diffpnm " + original_path + " " + path_out;
					const std::string diff_result = parse_PSNR(exec(diff_cmd.c_str()));
					log_msg("Image " + file_name + " - PSNR = " + diff_result);

					// Write CSV line with PSNR for k value
					path_out = global_path_results + root + "/" + base + ".csv";
					std::string csv_text = csv_text + std::to_string(k) + "," + diff_result;
					write_table(path_out, csv_text);
				}
			}
		break;
	
		case 2:
			for (const auto & entry : fs::directory_iterator(global_path_in)) {
				
				// Create auxiliary variables
				ImageModel img_in, img_prefiltered, img_out;

				// Get current image's file name
				const std::string file_path = entry.path().string();
				const std::string file_name = get_file_name(file_path);

				// Get current image's params
				const std::string original_root = parse_base(file_name);
				const std::string root = original_root + "P";
				const std::string base = parse_extension(file_name);
				const float delta = parse_delta(file_name);
				const short k = best_contexts[base];
				log_msg("");
				log_msg("Running prefilter DUDE over image " + base + " with context K=" + std::to_string(k));

				// Create directory if it doesn't exist for saving image's denoised version
				add_directory(global_path_out + base);
				add_directory(global_path_results + root);
				
				// Read current image and copy it for out image
				code = read_image(file_path, img_in); 
				if (code != OK) {
					error_msg(code);
					exit (EXIT_FAILURE);
				}
				copy_image(img_in, img_out);

				// Execute DUDE with given params
				log_msg("Image " + base + " - Context K = " + std::to_string(k));
				dude(delta, k, img_in, img_prefiltered, img_out);

				/*
				// Format current image's denoised version file name
				std::string path_out = global_path_out + base + "/" + "K" + std::to_string(k) + file_name;
				log_msg("Image " + file_name + " - Result path = " + path_out);

				// Write current image's denoised version
				code = write_image(path_out, img_out); 
				if (code != OK) {
					error_msg(code);
					exit (EXIT_FAILURE);
				}
				
				// Execute diff between original image and denoised image
				const std::string original_path = global_path_original + original_root + ".pgm";
				const std::string diff_cmd = "./bin/diffpnm " + original_path + " " + path_out;
				const std::string diff_result = parse_PSNR(exec(diff_cmd.c_str()));
				log_msg("Image " + file_name + " - PSNR = " + diff_result);

				// Write CSV line with PSNR for k value
				path_out = global_path_results + root + "/" + base + ".csv";
				std::string csv_text = csv_text + std::to_string(k) + "," + diff_result;
				write_table(path_out, csv_text);
				*/
			}
		break;
	
		default:
			error_msg(ERROR_MAIN_BAD_FORMATTING);
		break;
	
	}

	return 0;
}

