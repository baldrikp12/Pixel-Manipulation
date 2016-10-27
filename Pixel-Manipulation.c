/*
 *      Author: Kenneth Baldridge
 */

#include <stdio.h>
#include <string.h>

int getSize_W_H(unsigned char *theHeaderArray, int theOffset, int theByteSize);
void blend(int theHeight, int theWidth,
        unsigned char pixelArray_1[][theWidth * 3],
        unsigned char pixelArray_2[][theWidth * 3],
        unsigned char OutFilePixelArray[][theWidth * 3]);
void checker(int theHeight, int theWidth,
        unsigned char pixelArray_1[][theWidth * 3],
        unsigned char pixelArray_2[][theWidth],
        unsigned char OutFilePixelArray[][theWidth * 3]);
void closeOpenFiles(FILE *theInFile1, FILE *theInFile2, FILE *theOutFile1,
        FILE *theOutFile2);

int main(void) {

    //opens required file streams
    FILE *infile_1 = fopen("in1.bmp", "rb");
    FILE *infile_2 = fopen("in2.bmp", "rb");
    FILE *outfile_1 = fopen("blend.bmp", "wb");
    FILE *outfile_2 = fopen("checker.bmp", "wb");

    unsigned char header[54]; // We can assume same sized images so one header will do.
    fread(header, 1, 54, infile_1);
    fseek(infile_2, 54, SEEK_SET);

    int imgWidth = getSize_W_H(header, 18, 4);
    int imgHeight = getSize_W_H(header, 22, 4);
    int pixelArraySize = imgHeight * imgWidth * 3;

    unsigned char inFile_1_Pixels[imgHeight][imgWidth * 3];
    unsigned char inFile_2_Pixels[imgHeight][imgWidth * 3];
    unsigned char outFile_Blend_Pixels[imgHeight][imgWidth * 3];
    unsigned char outFile_Checker_Pixels[imgHeight][imgWidth * 3];

    //read in pizel arrays of both images.
    fread(inFile_1_Pixels, 1, pixelArraySize, infile_1);
    fread(inFile_2_Pixels, 1, pixelArraySize, infile_2);

    //blend create and write out
    blend(imgHeight, imgWidth, inFile_1_Pixels, inFile_2_Pixels,
            outFile_Blend_Pixels);
    fwrite(header, 1, 54, outfile_1);
    fwrite(outFile_Blend_Pixels, 1, pixelArraySize, outfile_1);

    //checkerboard create and write out
    checker(imgHeight, imgWidth, inFile_1_Pixels, inFile_2_Pixels,
            outFile_Checker_Pixels);
    fwrite(header, 1, 54, outfile_2);
    fwrite(outFile_Checker_Pixels, 1, pixelArraySize, outfile_2);

    closeOpenFiles(infile_1, infile_2, outfile_1, outfile_2);

    return 0;
}

/*
 * Returns the integer value at a specified location in a provided array of hex values (the header of a bmp file.)
 * param: theHeaderArray        Array of first 54 bytes of a .bmp file
 * param: theOffset             The starting index of the value to retrieve
 * param: theByteSize           The number of bytes to read
 * return:                      An integer representation of the N byte hex value
 *
 */
int getSize_W_H(unsigned char *theHeaderArray, int theOffset, int theByteSize) {

    unsigned char subArray[theByteSize + 1];
    memcpy(subArray, &theHeaderArray[theOffset], theByteSize);
    int *theSize = (int*) subArray;

    return *theSize;

}

/*
 * Blends two images together to create a third image.
 * param: theHeight             The pixel height of the image
 * param: theWidth              The width width of the image
 * param: pixelArray_1          2D pixel array of first image
 * param: pixelArray_2          2D pixel array of second image
 * param: OutFilePixelArray     2D pixel array of newly created image
 * return: void
 *
 */
void blend(int theHeight, int theWidth,
        unsigned char pixelArray_1[][theWidth * 3],
        unsigned char pixelArray_2[][theWidth * 3],
        unsigned char OutFilePixelArray[][theWidth * 3]) {

    int row, col;

    for (row = 0; row < theHeight; row++) {

        for (col = 0; col < theWidth * 3; col++) {

            OutFilePixelArray[row][col] = (pixelArray_1[row][col]
                    + pixelArray_2[row][col]) / 2;

        }
    }

}

/*
 * Creates a checkerboard pattern using two images.
 * param: theHeight             The pixel height of the image
 * param: theWidth              The width width of the image
 * param: pixelArray_1          2D pixel array of first image
 * param: pixelArray_2          2D pixel array of second image
 * param: OutFilePixelArray     2D pixel array of newly created image
 * return: void
 *
 */
void checker(int theHeight, int theWidth,
        unsigned char pixelArray_1[][theWidth * 3],
        unsigned char pixelArray_2[][theWidth * 3],
        unsigned char OutFilePixelArray[][theWidth * 3]) {

    int checkerRow = theHeight / 8;
    int checkerCol = checkerRow * 3;

    int row, col, changeBit = 1;

    for (row = 0; row < theHeight; row++) {

        if ((row % checkerRow) == (col % checkerCol)) {
            changeBit *= -1;
        }

        for (col = 0; col < theWidth * 3; col++) {

            if ((col % checkerCol) == 0) {
                changeBit *= -1;
            }

            if (changeBit == 1) {
                OutFilePixelArray[row][col] = pixelArray_1[row][col];
            } else {
                OutFilePixelArray[row][col] = pixelArray_2[row][col];

            }

        }

    }
}

/*
 * Closes the file streams.
 * param: theInFile1    First filed opened for read in
 * param: theInFile2    Second file opened for read in
 * param: theOutFile1   Third file opened for write out
 * param: theOutFile2   Fourth file opened for write out
 *
 */
void closeOpenFiles(FILE *theInFile1, FILE *theInFile2, FILE *theOutFile1,
        FILE *theOutFile2) {

    fclose(theInFile1);
    fclose(theInFile2);
    fclose(theOutFile1);
    fclose(theOutFile2);

}
