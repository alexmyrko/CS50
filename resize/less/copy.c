// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n1\n");
        return 1;
    }
    
    int scale = atoi(argv[1]);
    if (scale < 0 || scale > 100){
        fprintf(stderr, "Usage: copy infile outfile\n2\n");
        return 2;
    }
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n3\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n4\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n5\n");
        return 5;
    }
    
    int oldBiWidth = bi.biWidth;
    int oldBiHeight = abs(bi.biHeight);
    bi.biWidth = oldBiWidth * scale;
    bi.biHeight = oldBiHeight * scale;
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi.biSizeImage = abs(bi.biHeight) * (bi.biWidth * sizeof(RGBTRIPLE) + padding);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    int oldPadding = (4 - (oldBiWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanliness
    for (int i = 0, biHeight = oldBiHeight; i < biHeight; i++)
    {
        for (int k = 0; k < scale; k++){
        if (k > 0)
            fseek(inptr, -(oldBiWidth*sizeof(RGBTRIPLE) + oldPadding), SEEK_CUR);
         // iterate over pixels in scanlines
            for (int j = 0; j < oldBiWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;
    
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
                // write RGB triple to outfile scale times
                for (int l = 0; l < scale; l++){
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
            }
            // skip over padding, if any
            fseek(inptr, oldPadding, SEEK_CUR);
            for (int m = 0; m < padding; m++)
            {
                fputc(0x00, outptr);
            }
        }
        

        // then add it back (to demonstrate how)

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
