/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
  FILE *file = fopen(colorfile, "r");
  uint8_t** toReturn;
  int i, j;
  int red, green, blue;
  fscanf(file, "%d", colorcount);

  if(*colorcount < 1){
    return NULL;
  }

  toReturn = malloc(*colorcount * sizeof(uint8_t*));

  for (i = 0; i < *colorcount; i++) {
    /* code */
    if(fscanf(file, "%d %d %d", &red, &green, &blue) == 3){
      *(toReturn + i) = malloc(3 * sizeof(int));
      if(red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255){
        for(j = 0; j < i+1; j++){
          free(*(toReturn + j));
        }
        free(toReturn);
        return NULL;
      }
      toReturn[i][0] = red;
      toReturn[i][1] = green;
      toReturn[i][2] = blue;
    }else{
      for(j = 0; j < i+1; j++){
        free(*(toReturn + j));
      }
      free(toReturn);
      return NULL;
    }
  }

  fclose(file);
  return toReturn;
	//YOUR CODE HERE
}
