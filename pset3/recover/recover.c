#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2){
        fprintf(stderr, "Usage: recover infile\n1\n");
            return 1;
    }
    
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL){
        fprintf(stderr, "Usage: recover infile\n2\n");
            return 2;
    }
    FILE *outptr = NULL;
    
    bool first = true;
    unsigned char buffer[512];
    int count = 0;
    char name[10];
    while (fread(&buffer, 512, 1, inptr)) {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && 240 > buffer[3] && buffer[3] > 223){
            sprintf(name, "%03i.jpg", count);
            if (!first){
                fclose(outptr);
            }
            outptr = fopen(name, "w");
            count++;
            first = false;
        }
        if (outptr != NULL){
            fwrite(&buffer, 512, 1, outptr);
        } 
    }
    
    buffer[50] = 'l';
    fclose(outptr);
    fclose(inptr);
    }