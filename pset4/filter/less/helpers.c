#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop for height
    for (int i = 0; i < height; i++)
    {
        // loop for width
        for (int j = 0; j < width; j++)
        {
            // save current RGB values
            int originalBlue = image[i][j].rgbtBlue;
            int originalGreen = image[i][j].rgbtGreen;
            int originalRed = image[i][j].rgbtRed;

            // average them and round to get grayscale value
            // notice the decimal so that a float result is produced
            int grey_value = round((originalBlue + originalGreen + originalRed) / 3.00);

            // assign grey value to all rgb values
            image[i][j].rgbtBlue = grey_value;
            image[i][j].rgbtGreen = grey_value;
            image[i][j].rgbtRed = grey_value;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop for height
    for (int i = 0; i < height; i++)
    {
        // loop for width
        for (int j = 0; j < width; j++)
        {
            // save current RGB values
            int originalBlue = image[i][j].rgbtBlue;
            int originalGreen = image[i][j].rgbtGreen;
            int originalRed = image[i][j].rgbtRed;

            // create new RGB values based on sepia Formula
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
			int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
			int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            // assign sepia value to all rgb values
            // limit to 255 for values greater than 255 (Ternary Operator)
            image[i][j].rgbtBlue = (sepiaBlue > 255) ? 255 : sepiaBlue;
            image[i][j].rgbtGreen = (sepiaGreen > 255) ? 255 : sepiaGreen;
            image[i][j].rgbtRed = (sepiaRed > 255) ? 255 : sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // preallocate memory the size of the image
    RGBTRIPLE (*image_copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image_copy == NULL)
    {
        fprintf(stderr, "Calloc failed.\n");
        return;
    }
    // copy image contents from duplicate in reverse order
    // loop for height
    for (int i = 0; i < height; i++)
    {
        // loop for width
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][width - (j + 1)];
        }
    }
    // copy reversed image back to original
    for (int i = 0; i < height; i++)
    {
        // loop for width
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
    // free memory used to store copy
    free(image_copy);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // preallocate memory the size of the image
    RGBTRIPLE (*image_copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image_copy == NULL)
    {
        fprintf(stderr, "Calloc error.\n");
        return;
    }
    for (int i = 0; i < height; i++)
    {
        // loop for width
        for (int j = 0; j < width; j++)
        {
            // define values that will be incremented
            float pixel_grids = 0.00;
            int red_sum = 0;
            int green_sum = 0;
            int blue_sum = 0;
            // create arrays for the 3x3 grids to be iterated over
            // height grid
            for (int k = i - 1, x = i + 2; k < x; k++ )
            {
                // width grid
                for (int l = j - 1, y = j + 2; l < y; l++)
                {
                    // proceed only if the pixels are within bounds
                    //i.e. not past the edges
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        red_sum += image[k][l].rgbtRed;
                        green_sum += image[k][l].rgbtGreen;
                        blue_sum += image[k][l].rgbtBlue;
                        pixel_grids++;
                    }
                }
            }
            // assign new values to the image copy
            image_copy[i][j].rgbtBlue = round(blue_sum / pixel_grids);
            image_copy[i][j].rgbtGreen = round(green_sum / pixel_grids);
            image_copy[i][j].rgbtRed = round(red_sum / pixel_grids);
        }
    }
    // copy blurred values back to original
    for (int i = 0; i < height; i++)
    {
        // loop for width
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_copy[i][j];
        }
    }
    // free allocated space
    free(image_copy);
    return;
}
