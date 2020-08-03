// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n1\n");
        return 1;
    }
    
    float scale = atof(argv[1]);
    printf("%f\n", scale);
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
    int oldBiHeight = bi.biHeight;
    bi.biWidth = round(oldBiWidth * scale);
    bi.biHeight = round(oldBiHeight * scale);
    int hScale = round(fabs(oldBiHeight/(float)bi.biHeight)); 
    int wScale = round(oldBiWidth/(float)bi.biWidth); 
    
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bi.biSizeImage = abs(bi.biHeight) * (bi.biWidth * sizeof(RGBTRIPLE) + padding);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    int oldPadding = (4 - (oldBiWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int oldRowWidth = oldBiWidth * sizeof(RGBTRIPLE) + oldPadding;

    // iterate over infile's scanliness
    if (scale < 1){
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
                if (i > 0){
                    fseek(inptr, (hScale - 1) * oldRowWidth, SEEK_CUR);
                }
             // iterate over pixels in scanlines
                for (int j = 0; j < bi.biWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
        
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    fseek(inptr, (wScale - 1)*sizeof(RGBTRIPLE), SEEK_CUR);
                    
                    // write RGB triple to outfile scale times
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
                // skip over padding, if any
                fseek(inptr, oldPadding, SEEK_CUR);
                for (int m = 0; m < padding; m++)
                {
                    fputc(0x00, outptr);
                }
        }
    } else {
    
    for (int i = 0, biHeight = abs(oldBiHeight); i < biHeight; i++)
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
    }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
