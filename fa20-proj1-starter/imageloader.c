/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	Image *image = (Image *)malloc(sizeof(Image));	
	FILE *fp = fopen(filename, "r");
	// read meta data
	uint32_t cols, rows, scale;
	char format[10];
	fscanf(fp, "%s %u %u %u", format, &cols, &rows, &scale);
	Color **color_list = (Color **)malloc(rows * sizeof(Color *));
	for (uint32_t i = 0; i < rows; i++) {
		color_list[i] = (Color *)malloc(cols * sizeof(Color));
		for (uint32_t j = 0; j < cols; j++) {
			fscanf(fp, "%hhu %hhu %hhu", &color_list[i][j].R, &color_list[i][j].G, &color_list[i][j].B);
		}
	}
	image->rows = rows;
	image->cols = cols;	
	image->image = color_list;
	fclose(fp);
	
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	for (uint32_t i = 0; i < image->rows; i++) {
		uint32_t j;
		for (j = 0; j < image->cols - 1; j++) {
			printf("%3d %3d %3d   ", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
	} // last column
		printf("%3d %3d %3d\n", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
	}
}

// Frees an image
void freeImage(Image *image) {
    if (image == NULL) return;
    for (uint32_t i = 0; i < image->rows; i++) {
        free(image->image[i]);
    }
    free(image->image);
    free(image);
}
