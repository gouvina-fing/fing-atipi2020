// Global dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <filesystem>
#include <map>

// Local dependencies
#include <denoiser.h>
#include <models/imageModel.h>
#include <handlers/imageHandler.h>
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
	const std::string global_path_results_prefilter = "data/results_prefilter/";

	// Create auxiliary variables
	std::string file_path, file_name, root, base, path_out, original_path, diff_cmd, diff_result, csv_text, original_base, path_out_csv;
	float delta;
	short k;
	ImageModel img_in, img_prefiltered, img_out;
	img_prefiltered.setEmpty(true);

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

			// Clean and create directory for results
			remove_directory(global_path_results);
			add_directory(global_path_results);

			for (const auto & entry : fs::directory_iterator(global_path_in)) {
				// Get current image's file name
				file_path = entry.path().string();
				file_name = get_file_name(file_path);
				log_msg("");
				log_msg("Running DUDE over image " + file_name);

				// Get current image's params
				root = parse_base(file_name);
				base = parse_extension(file_name);
				delta = parse_delta(file_name);

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
				for (k = 0; k < 9; k++) {
					// Get img_in once more
					copy_image(img_in, img_out);

					// Execute DUDE with given params
					log_msg("Image " + file_name + " - Context K = " + std::to_string(k));
					dude(delta, k, img_in, img_prefiltered, img_out);

					// Format current image's denoised version file name
					path_out = global_path_out + base + "/" + "K" + std::to_string(k) + file_name;
					log_msg("Image " + file_name + " - Result path = " + path_out);

					// Write current image's denoised version
					code = write_image(path_out, img_out); 
					if (code != OK) {
						error_msg(code);
						exit (EXIT_FAILURE);
					}
					
					// Execute diff between original image and denoised image
					original_path = global_path_original + root + ".pgm";
					diff_cmd = "./bin/diffpnm " + original_path + " " + path_out;
					diff_result = parse_PSNR(exec(diff_cmd.c_str()));
					log_msg("Image " + file_name + " - PSNR = " + diff_result);

					// Write CSV line with PSNR for k value
					path_out = global_path_results + root + "/" + base + ".csv";
					csv_text = std::to_string(k) + "," + diff_result;
					write_CSV(path_out, csv_text);
				}
			}
		break;
	
		case 2:

			// Clean and create directory for results
			remove_directory(global_path_results_prefilter);
			add_directory(global_path_results_prefilter);
			
			for (const auto & entry : fs::directory_iterator(global_path_in)) {
				// Get current image's file name
				file_path = entry.path().string();
				file_name = get_file_name(file_path);

				// Get current image's params
				root = parse_base(file_name);
				original_base = parse_extension(file_name);
				base = "Pre_" + parse_extension(file_name);

				delta = parse_delta(file_name);
				k = best_contexts[original_base];
				img_prefiltered.setEmpty(true);
				log_msg("");
				log_msg("Running prefilter DUDE over image " + original_base + " with context K=" + std::to_string(k));

				// Create directory if it doesn't exist for saving image's denoised version
				add_directory(global_path_out + base);
				add_directory(global_path_results_prefilter + root);
				
				// Read current image and copy it for out image
				code = read_image(file_path, img_in); 
				if (code != OK) {
					error_msg(code);
					exit (EXIT_FAILURE);
				}
				copy_image(img_in, img_out);

				// Execute DUDE with given params
				log_msg("Image " + file_name + " - Context K = " + std::to_string(k));
				dude(delta, k, img_in, img_prefiltered, img_out);

				// Format current image's denoised version file name
				path_out = global_path_out + base + "/" + original_base + "_0.pgm";
				log_msg("Image " + file_name + " - Result path = " + path_out);

				// Write current image's denoised version
				code = write_image(path_out, img_out); 
				if (code != OK) {
					error_msg(code);
					exit (EXIT_FAILURE);
				}
				
				// Execute diff between original image and denoised image
				original_path = global_path_original + root + ".pgm";
				diff_cmd = "./bin/diffpnm " + original_path + " " + path_out;
				diff_result = parse_PSNR(exec(diff_cmd.c_str()));
				log_msg("Image " + file_name + " - PSNR = " + diff_result);

				// Write CSV line with PSNR for k value
				path_out_csv = global_path_results_prefilter + root + "/" + original_base + ".csv";
				csv_text = std::to_string(0) + "," + diff_result;
				write_CSV(path_out_csv, csv_text);

				img_prefiltered.setEmpty(false);

				for (short i = 1; i <= 10; i++) {

					copy_image(img_out, img_prefiltered);
					copy_image(img_in, img_out);

					// Execute DUDE with given params
					log_msg("Image " + file_name + " - Iteration " + std::to_string(i));
					dude(delta, k, img_in, img_prefiltered, img_out);

					// Format current image's denoised version file name
					path_out = global_path_out + base + "/" + original_base + "_" + std::to_string(i) + ".pgm";
					log_msg("Image " + file_name + " - Result path = " + path_out);

					// Write current image's denoised version
					code = write_image(path_out, img_out); 
					if (code != OK) {
						error_msg(code);
						exit (EXIT_FAILURE);
					}
		
					// Execute diff between original image and denoised image
					diff_cmd = "./bin/diffpnm ";
					diff_cmd += original_path;
					diff_cmd += " ";
					diff_cmd += path_out;
					
					diff_result = parse_PSNR(exec(diff_cmd.c_str()));
					log_msg("Image " + file_name + " - PSNR = " + diff_result);
					
					// Write CSV line with PSNR for k value
					csv_text = std::to_string(i) + "," + diff_result;
					write_CSV(path_out_csv, csv_text);
				}
			}
		break;
	
		default:
			error_msg(ERROR_MAIN_BAD_FORMATTING);
		break;
	
	}

	return 0;
}

