/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	//YOUR CODE HERE
	int* colorcount;
	uint8_t** colormap;
	int w, h, c;
	FILE* file;
	int red, green, blue;
	int i;
	file = fopen(outputfile, "w");
	if(file == NULL){
		return 1;
	}
	if(width < 1 || heightpercolor < 1){
		return 1;
	}
	colorcount = malloc(sizeof(int));
	colormap = FileToColorMap(colorfile, colorcount);


	if(colormap == NULL){
		free(colorcount);
		return 1;
	}
	fprintf(file, "P3 %d %d 255\n", width, heightpercolor * (*colorcount));

	for(c = 0; c < *colorcount; c++){
		red = colormap[c][0];
		green = colormap[c][1];
		blue = colormap[c][2];
		for(h = 0; h < heightpercolor; h++){
			for(w = 0; w < width-1; w ++){
				fprintf(file, "%d %d %d ", red, green, blue);
			}
			fprintf(file, "%d %d %d\n", red, green, blue);
		}
	}

	for(i =0; i< *(colorcount); i++){
		free(*(colormap + i));
	}
	free(colormap);
	free(colorcount);
	fclose(file);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	//YOUR CODE HERE
	int* colorcount;
	uint8_t** colormap;
	uint8_t* arr;
	FILE* file;
	int times = width * heightpercolor;
	int i, j;
	file = fopen(outputfile, "w");
	if(file == NULL){
		return 1;
	}

	if(width < 1 || heightpercolor < 1){
		return 1;
	}
	colorcount = malloc(sizeof(int));
	colormap = FileToColorMap(colorfile, colorcount);

	if(colormap == NULL){
		free(colorcount);
		return 1;
	}
	fprintf(file, "P6 %d %d 255\n", width, heightpercolor * (*colorcount));


	for(i = 0; i < *colorcount; i++){
		for(j = 0; j < times; j++){
				fwrite(colormap[i], 1, 3, file);
		}
	}

	for(i = 0; i < *(colorcount); i++){
		free(*(colormap + i));
	}
	free(colormap);
	free(colorcount);
	fclose(file);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}