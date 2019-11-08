// --------------------------------------------------------------------------------------------------
// EE569 Homework Assignment #3
// Date: March 3, 2019
// Name: Suchismita Sahu
// ID: 7688176370
// email: suchisms@usc.edu
// ---------------------------------------------------------------------------------------------------

#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <cmath>
#include <vector>

using namespace std;

//Function to check defect count and fill holes
unsigned char erode(int i, int j, unsigned char filter[9], unsigned char image[693][552][1]) {
    int count = 0;
    for (int m = -1, l = 0; m <= 1; m++) {
        for (int n = -1; n <= 1; n++, l++) {
            if (filter[l] == image[i + m][j + n][0]){
                count++;
            }
            if (count == 9){
                return 1;
            }
        }
    }
    //cout << max(count) << endl;
return image[i][j][0];
//return 0;
}

unsigned char dilate(int i, int j, unsigned char filter[9], unsigned char image[693][552][1]) {
    int count = 0;
    for (int m = -1, l = 0; m <= 1; m++) {
        for (int n = -1; n <= 1; n++, l++) {
            if (filter[l] == image[i + m][j + n][0]){
                count++;
            }
            if (count == 9){
                return 1;
            }
        }
    }
    //cout << max(count) << endl;
return image[i][j][0];
//return 0;
}

int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel = 1;
    int Size = 375;
    int length = 500;
    int width = 690;

    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is greyscale or color
    if (argc < 4) {
        BytesPerPixel = 1; // default is grey image
    } else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5) {
            Size = atoi(argv[4]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[length][width][3];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length * width * 1, file);
    fclose(file);


    // Allocate image data array for Gray scale Conversion
    unsigned char ImagedataGray[length][width][1];

    //Converting RGB to Gray using Luminosity weighted average method
    for (int i = 0 ; i < length ; i++){
        for (int j = 0 ; j < width ; j++){
            ImagedataGray[i][j][0] = (unsigned char)((0.21*Imagedata[i][j][0]) + (0.72*Imagedata[i][j][1]) + (0.07*Imagedata[i][j][2]));
        }
    }


    // Binarizing the input Image
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (Imagedata[i][j][0] < 128) {
                Imagedata[i][j][0] = (unsigned char) 1;
            } else {
                Imagedata[i][j][0] = (unsigned char) 0;
            }
        }
    }


    // Binarizing the input Image
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (Imagedata[i][j][0] < 128) {
                Imagedata[i][j][0] = (unsigned char) 1;
            } else {
                Imagedata[i][j][0] = (unsigned char) 0;
            }
        }
    }


    // MORPHOLOGICAL APPLICATION: DEFECT DETECTION AND CORRECTION

    // Allocating New Image Data Array with Boundary Extensions :
    unsigned char NewImagedataThree[693][552][1];

    // Creating New Image Data Array by mirroring the second last rows and columns to create length+2*width+2 Array
    for (int i = 1; i < 1 + width; i++) { // Rows added to top and bottom
        NewImagedataThree[0][i][0] = Imagedata[1][i - 1][0];
        NewImagedataThree[length + 1][i][0] = Imagedata[length - 1][i - 1][0];
    }

    for (int i = 1; i < 1 + length; i++) {
        NewImagedataThree[i][0][0] = Imagedata[i - 1][1][0];
        NewImagedataThree[i][width + 1][0] = Imagedata[i - 1][width - 1][0];
    }

    for (int i = 1; i < length + 1; i++) {
        for (int j = 1; j < width + 1; j++) {
            NewImagedataThree[i][j][0] = Imagedata[i - 1][j - 1][0];
        }
    }

    // Corner elements
    NewImagedataThree[0][0][0] = NewImagedataThree[0][2][0];
    NewImagedataThree[0][width + 1][0] = NewImagedataThree[0][width - 1][0];
    NewImagedataThree[length + 1][0][0] = NewImagedataThree[length + 1][2][0];
    NewImagedataThree[length + 1][width + 1][0] = NewImagedataThree[length + 1][width - 1][0];


    //Filter to check 8-Connectivity

    unsigned char filter[9] = {1,1,1,1,0,1,1,1,1};

    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImage[691][550][1];

    for(int i=1; i < length+1; i++){
        for(int j=1; j < width+1; j++){
            OutputImage[i-1][j-1][0] = erode(i, j, filter, NewImagedataThree);
        }
    }


    // Converting to Visual Image
    unsigned char OutputImageFinal[length][width][1];

    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if (OutputImage[i][j][0] > 0) {
                OutputImageFinal[i][j][0] = 255;
            }
            else{
                OutputImageFinal[i][j][0] = 0;
            }
        }
    }

    // Converting to Visual Image
    unsigned char ImagedataFinal[length][width][1];

    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if (Imagedata[i][j][0] > 0) {
                ImagedataFinal[i][j][0] = 255;
            }
            else{
                ImagedataFinal[i][j][0] = 0;
            }
        }
    }



    // Write image data (filename specified by second argument) from image data matrix

    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(ImagedataGray, sizeof(unsigned char), length*width*1, file);
    fclose(file);

    return 0;

}
