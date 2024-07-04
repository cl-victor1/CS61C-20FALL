/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{	
	//YOUR CODE HERE
	Color *this_color = (Color *)malloc(sizeof(Color));
	uint8_t blue_value = image->image[row][col].B;
	if (blue_value & 1) {
        this_color->R = 255, this_color->G = 255, this_color->B = 255;
    } else {
        this_color->R = 0, this_color->G = 0, this_color->B = 0;
    }
	return this_color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
	uint32_t rows = image->rows;
	uint32_t cols = image->cols;
	Color **color_list = (Color **)malloc(rows * sizeof(Color *));
	for (uint32_t i = 0; i < rows; i++) {
		color_list[i] = (Color *)malloc(cols * sizeof(Color));
		for (uint32_t j = 0; j < cols; j++) {
			Color *pixel_color = evaluateOnePixel(image, i, j); 
            color_list[i][j] = *pixel_color; //copy data
            free(pixel_color);
		}
	}
	Image *result = (Image *)malloc(sizeof(Image));	
	result->cols = cols, result->rows = rows, result->image = color_list;
	return result; 
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	// Check if the correct number of arguments is provided
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(-1);
    }

    // Read the original image data
    Image *original_image = readData(argv[1]);
    if (original_image == NULL) {
        fprintf(stderr, "Failed to read data from %s\n", argv[1]);
        exit(-1);
    }

    // Perform steganography on the image
    Image *new_image = steganography(original_image);
    if (new_image == NULL) {
        fprintf(stderr, "Steganography processing failed\n");
        freeImage(original_image); // Ensure the original image is freed
        exit(-1);
    }
	writeData(new_image);
    // Free the image memory
    freeImage(original_image);
    freeImage(new_image);
	return 0;
}
