/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    double ratio = finalscale/initialscale;
    int i;
    for(i = 0; i < framecount; i++){
      double index = (double)i;
      Mandelbrot(threshold, max_iterations, center, initialscale * pow(ratio, index/(framecount-1)), resolution, *(output+i));
      //printf("\n\n\n");
    }
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function:
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order.

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/

	//YOUR CODE HERE
  if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 7\n", argv[0]);
		printUsage(argv);
		return 1;
	}
  double threshold, center_real, center_imaginary, initialscale, finalscale;
  int framecount;
  u_int64_t max_iterations, resolution;
  ComplexNumber* center;
  u_int64_t** output;
  uint8_t** colormap;
  char* output_folder;
  char* colorfile;
  int* colorcount;
  int i, j, num;
  char outputfile[50];
  uint8_t black[3] = {0,0,0};

  colorcount = malloc(sizeof(int));
  output_folder = malloc(sizeof(char)* 50);
  colorfile = malloc(sizeof(char)* 50);
  threshold = atof(argv[1]);
  max_iterations = (u_int64_t)atoi(argv[2]);
  center = newComplexNumber(atof(argv[3]), atof(argv[4]));
  initialscale = atof(argv[5]);
  finalscale = atof(argv[6]);
  framecount = atoi(argv[7]);
  resolution = (u_int64_t)atoi(argv[8]);
  sprintf(output_folder, "%s", argv[9]);
  sprintf(colorfile, "%s", argv[10]);
  colormap = FileToColorMap(colorfile, colorcount);

  if(threshold <= 0 || max_iterations <= 0 || initialscale <= 0 || finalscale <=0){
    return 1;
  }
  if(framecount > 10000 || framecount < 0){
    return 1;
  }
  if(resolution < 0){
    return 1;
  }
  if(framecount == 1 && initialscale != finalscale){
    return 1;
  }
  if(colormap == NULL){
    return 1;
  }

	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space.
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	//YOUR CODE HERE
  output = malloc(sizeof(u_int64_t*) * framecount);
  if(output == NULL){
    for(i = 0; i < *colorcount; i++){
      free(*(colormap + i));
    }
    free(colormap);
    free(colorcount);
    free(colorfile);
    freeComplexNumber(center);
    free(output_folder);
    return 1;
  }
  for(i = 0; i < framecount; i++){
    *(output + i) = malloc(sizeof(u_int64_t) * (2 * resolution + 1) * (2 * resolution + 1));
    if(*(output + i) == NULL){
      for(j = 0; j < i; j++){
        free(*(output + j));
      }
      free(output);
      for(i = 0; i < *colorcount; i++){
        free(*(colormap + i));
      }
      free(colormap);
      free(colorcount);
      free(colorfile);
      freeComplexNumber(center);
      free(output_folder);
      return 1;
    }
  }

  MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, output);

	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/

	//YOUR CODE HERE
    for (i = 0; i < framecount; i++) {
      sprintf(outputfile, "%s/frame%05d.ppm", output_folder, i);
      FILE* file = fopen(outputfile, "w");
      fprintf(file, "P6 %llu %llu 255\n", 2*resolution+1, 2*resolution+1);
      for(j = 0; j < (2 * resolution +1) * (2 * resolution + 1); j++){
        num =  output[i][j] % (*colorcount);
        if(num == 0){
          fwrite(black, 1, 3, file);
        }else{
          fwrite(colormap[num-1], 1, 3, file);
        }
      }
      fclose(file);
    }


	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	//YOUR CODE HERE

  for(i = 0; i < framecount; i++){
    free(*(output + i));
  }
  free(output);
  for(i = 0; i < *colorcount; i++){
    free(*(colormap + i));
  }
  free(colormap);
  free(colorcount);
  free(colorfile);
  freeComplexNumber(center);
  free(output_folder);



	return 0;
}
