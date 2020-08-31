#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check for improper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // assign filenames to variable
    char *infile = argv[1];
    char outfile[8];

    // open raw data and check it was successful
    FILE *f = fopen(infile, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Error opening raw file\n");
        return 2;
    }

    // allocate memory for temprary buffer
    BYTE *buffer = malloc(512 * sizeof(BYTE));
    if (buffer == NULL)
    {
        fprintf(stderr, "Error allocating memory for buffer\n");
        return 3;
    }

    int jpg_count = 0;
    bool jpg_found = false;
    FILE *img = NULL;
    // start reading data
    while (fread(buffer, 512, 1, f) == 1)
    {
        // if the start of a jpeg file is found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff  && (buffer[3] & 0xf0) == 0xe0)
        {
            // if this is the first jpg file
            if (jpg_count == 0)
            {
                jpg_found = true;
            }
            // otherwise close the current jpeg file
            else
            {
                fclose(img);
            }
            // create filename, open file and write buffer data
            sprintf(outfile, "%03i.jpg", jpg_count);
            img = fopen(outfile, "w");
            fwrite(buffer, 512, 1, img);
            jpg_count++;
        }
        // add data to file if jpg has already been found
        else if(jpg_found)
        {
            fwrite(buffer, 512, 1, img);
        }
    }
    fclose(img);
    free(buffer);
}
