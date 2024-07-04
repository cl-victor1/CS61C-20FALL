/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Victor 
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.

uint8_t arrayToUint8(int *bits) {
    uint8_t result = 0;
    for (int i = 0; i < 8; i++) {
        if (bits[i] == 1) {
            result |= (1 << (7 - i));
        }
    }
    return result;
}

int getBit(uint8_t value, int position) {
	int my_state = (value >> (7 - position)) & 1;
	return my_state;
}

Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	// //YOUR CODE HERE
	int left = col - 1, right = col + 1,  bottom = row - 1, top = row + 1; //we store pixels in “reading order”, from top to bottom, left to right.
	if (left < 0) {
		left = image->cols - 1;
	}
	if (right > image->cols - 1) {
		right = 0;
	}
	if (bottom < 0) { // bottom is actually top in the matrix
		bottom = image->rows - 1;
	}
	if (top > image->rows - 1) { // top is actually bottom in the matrix
		top = 0;
	}

	int *red_my_state = (int *)malloc(8 * sizeof(int));	
	int *green_my_state = (int *)malloc(8 * sizeof(int));	
	int *blue_my_state = (int *)malloc(8 * sizeof(int));	
	int *red_num_alives = (int *)malloc(8 * sizeof(int));	
	int *green_num_alives = (int *)malloc(8 * sizeof(int));
	int *blue_num_alives = (int *)malloc(8 * sizeof(int));
	// number of alives for each bit position of each color
	for (int i = 0; i < 8; i++) {
		//red my state
		red_my_state[i] = getBit(image->image[row][col].R, i);
		//green my state
		green_my_state[i] = getBit(image->image[row][col].G, i);
		//blue my state
		blue_my_state[i] = getBit(image->image[row][col].B, i);

		//red num of alives        
		red_num_alives[i] = getBit(image->image[bottom][left].R, i) + getBit(image->image[bottom][right].R, i) + getBit(image->image[bottom][col].R, i) + \
				+ getBit(image->image[row][left].R, i) + getBit(image->image[row][right].R, i) + getBit(image->image[top][left].R, i) + \
				+ getBit(image->image[top][right].R, i) + getBit(image->image[top][col].R, i);
		//green num of alives 
		green_num_alives[i] = getBit(image->image[bottom][left].G, i) + getBit(image->image[bottom][right].G, i) + getBit(image->image[bottom][col].G, i) + \
				+ getBit(image->image[row][left].G, i) + getBit(image->image[row][right].G, i) + getBit(image->image[top][left].G, i) + \
				+ getBit(image->image[top][right].G, i) + getBit(image->image[top][col].G, i);
		//blue num of alives 
		blue_num_alives[i] = getBit(image->image[bottom][left].B, i) + getBit(image->image[bottom][right].B, i) + getBit(image->image[bottom][col].B, i) + \
				+ getBit(image->image[row][left].B, i) + getBit(image->image[row][right].B, i) + getBit(image->image[top][left].B, i) + \
				+ getBit(image->image[top][right].B, i) + getBit(image->image[top][col].B, i);
    }

	Color *this_color = (Color *)malloc(sizeof(Color));	// new Color to be returned
	
	int *red_new_state = (int *)malloc(8 * sizeof(int));	
	int *green_new_state = (int *)malloc(8 * sizeof(int));	
	int *blue_new_state = (int *)malloc(8 * sizeof(int));
	for (int i = 0; i < 8; i++) {
		// red
		if (red_my_state[i] == 1) { // alive
			red_new_state[i] = (rule >> (9 + red_num_alives[i])) & 1;
		} else { // dead
			red_new_state[i] = (rule >> (red_num_alives[i])) & 1;
		}
		// green
		if (green_my_state[i] == 1) { // alive
			green_new_state[i] = (rule >> (9 + green_num_alives[i])) & 1;
		} else { // dead
			green_new_state[i] = (rule >> (green_num_alives[i])) & 1;
		}
		// blue
		if (blue_my_state[i] == 1) { // alive
			blue_new_state[i] = (rule >> (9 + blue_num_alives[i])) & 1;
		} else { // dead
			blue_new_state[i] = (rule >> (blue_num_alives[i])) & 1;
		}
		
	}

	this_color->R = arrayToUint8(red_new_state);
	this_color->G = arrayToUint8(green_new_state);
	this_color->B = arrayToUint8(blue_new_state);
	free(red_my_state);
	free(red_new_state);
	free(red_num_alives);
	free(green_my_state);
	free(green_new_state);
	free(green_num_alives);
	free(blue_my_state);
	free(blue_new_state);
	free(blue_num_alives);
	return this_color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	uint32_t rows = image->rows;
	uint32_t cols = image->cols;
	Color **color_list = (Color **)malloc(rows * sizeof(Color *));
	for (uint32_t i = 0; i < rows; i++) {
		color_list[i] = (Color *)malloc(cols * sizeof(Color));
		for (uint32_t j = 0; j < cols; j++) {
			Color *pixel_color = evaluateOneCell(image, i, j, rule); 
            color_list[i][j] = *pixel_color; //copy data
            free(pixel_color);
		}
	}
	Image *result = (Image *)malloc(sizeof(Image));	
	result->cols = cols, result->rows = rows, result->image = color_list;
	return result; 
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
        printf("usage: ./gameOfLife filename rule\n\
    			filename is an ASCII PPM file (type P3) with maximum value 255.\n\
    			rule is a hex number beginning with 0x; Life is 0x1808.");
        exit(-1);
    }

    // Read the original image data
    Image *original_image = readData(argv[1]);

	// turn string into int
	char *endptr; // for strtol
	uint32_t rule = (uint32_t) strtol(argv[2], &endptr, 0); // A base of 0 means that the base will be determined by the format of the string (e.g., "0x" for hex, "0" for octal).
    if (*endptr != '\0') {
        exit(-1); // the rule is illegal
    }
	
	if (original_image == NULL) {
        fprintf(stderr, "Failed to read data from %s\n", argv[1]);
        exit(-1);
    }

    // Perform steganography on the image
    Image *new_image = life(original_image, rule);
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
