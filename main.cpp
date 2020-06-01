#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include "src/util/CImg.h"

using namespace cimg_library;

void dude(float * img_in, float * img_out);

int main(int argc, char** argv){

	const char* path_in;
	const char* path_out;

	if (argc < 3) printf("Invocar como: './main img_in img_out'.\n");
    else
        path_in = argv[1];
        path_out = argv[2];


	CImg<float> image_in(path_in);
	CImg<float> image_out(image_in.width(), image_in.height(),1,1,0);

	float *img_in_matrix = image_in.data();
    float *img_out_matrix = image_out.data();

	dude(img_in_matrix, img_out_matrix);
   	image_out.save(path_out);

	return 0;
}

