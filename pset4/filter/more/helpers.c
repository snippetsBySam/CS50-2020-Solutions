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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // preallocate memory the size of the image
    RGBTRIPLE (*image_copy)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image_copy == NULL)
    {
        fprintf(stderr, "Calloc error.\n");
        return;
    }
    // create Gx and Gy grid arrays
    int gx_grid[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy_grid[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // loop for height
    for (int i = 0; i < height; i++)
    {
        // loop for width
        for (int j = 0; j < width; j++)
        {
            // define values that will be incremented
            int gxy_red;
            int gxy_green;
            int gxy_blue;
            int gx_red = 0;
            int gx_green = 0;
            int gx_blue = 0;
            int gy_red = 0;
            int gy_green = 0;
            int gy_blue = 0;
            // create arrays for the 3x3 pixel grids to be iterated over
            // height grid
            // edge_h, edge_w are used to iterate the edge grids
            for (int k = i - 1, x = i + 2, edge_h = 0; k < x; k++, edge_h++)
            {
                // width grid
                for (int l = j - 1, y = j + 2, edge_w = 0; l < y; l++, edge_w++)
                {
                    // proceed only if the pixels are within bounds
                    //i.e. not past the edges
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        gx_red += image[k][l].rgbtRed * gx_grid[edge_h][edge_w];
                        gx_green += image[k][l].rgbtGreen * gx_grid[edge_h][edge_w];
                        gx_blue += image[k][l].rgbtBlue * gx_grid[edge_h][edge_w];
                        gy_red += image[k][l].rgbtRed * gy_grid[edge_h][edge_w];
                        gy_green += image[k][l].rgbtGreen * gy_grid[edge_h][edge_w];
                        gy_blue += image[k][l].rgbtBlue * gy_grid[edge_h][edge_w];
                    }
                }
            }
            // compute combined values
            gxy_red = round(sqrt(pow(gx_red, 2) + pow(gy_red, 2)));
            gxy_green = round(sqrt(pow(gx_green, 2) + pow(gy_green, 2)));
            gxy_blue = round(sqrt(pow(gx_blue, 2) + pow(gy_blue, 2)));

            // assign new values to the image copy but cap it at 255
            image_copy[i][j].rgbtRed = (gxy_red > 255) ? 255 : gxy_red;
            image_copy[i][j].rgbtGreen = (gxy_green > 255) ? 255 : gxy_green;
            image_copy[i][j].rgbtBlue = (gxy_blue > 255) ? 255 : gxy_blue;
        }
    }
    // copy edge values back to original
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
