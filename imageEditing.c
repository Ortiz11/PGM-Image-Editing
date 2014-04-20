//Nicholas Ortiz

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX 20
int* loadFile(int *row, int *col, int *maxVal);
void sobelFunc(int *row, int *col, int *maxVal, int* image);
void flipUD(int *row, int *col, int *maxVal, int* image);
void flipLR(int *row, int *col, int *maxVal, int* image);
void rotateCW(int *row, int *col, int *maxVal, int* image);
void rotateCCW(int *row, int *col, int *maxVal, int* image);
FILE* writeHeader(FILE* out, int r, int c, int max);

int main(void){
	int *row, *col, *maxVal, *image = NULL;
	int temp = 0, choice = 1;

	while (choice == 1){
		row = malloc(sizeof(int));
		col = malloc(sizeof(int));
		maxVal = malloc(sizeof(int));
		//get information from the inputfile
		image = loadFile(row, col, maxVal);
		while (image == NULL){
			image = loadFile(row, col, maxVal);
		}
		//ask user for type of function to use.
		printf("Which function would you like to perform?\n1: soble, 2: flip up/down, 3: flip left/right, 4: rotate CW, 5: rotate CCW\n");
		scanf("%d", &temp);

		while (temp < 1 || temp > 5){
			printf("Wrong function choice pick again\n");
			printf("Which function would you like to perform?\n1: soble, 2: flip up/down, 3: flip left/right, 4: rotate CW, 5: rotate CCW\n");
			scanf("%d", &temp);
		}
		//run the function the user picked.
		switch (temp){
		case 1:
			sobelFunc(row, col, maxVal, image);
			break;
		case 2:
			flipUD(row, col, maxVal, image);
			break;
		case 3:
			flipLR(row, col, maxVal, image);
			break;
		case 4:
			rotateCW(row, col, maxVal, image);
			break;
		case 5:
			rotateCCW(row, col, maxVal, image);
			break;
		default:
			break;
		}
		printf("Would you like to run the program again? 1 for yes 0 for no\n");
		scanf("%d", &choice);
	}
	return 0;
}
//gets values from the picture header and mallocs a 1d array to store values.
int* loadFile(int *row, int *col, int *maxVal){
	FILE *in;
	char inputFile[MAX];
	char pgmCheck[5];
	int r, c, ft, i, num;
	char comments[100];
	int *imageList = NULL;

	printf("Enter a pgm image: ");
	scanf("%s", inputFile);

	if ((in = fopen(inputFile, "r")) == NULL){
		printf("Unable to open file.\n");
		return imageList;
	}

	fscanf(in, "%s", pgmCheck);
	if (strcmp(pgmCheck, "P2") != 0)
	{
		printf("Error, not a .pgm file.\n");
		return imageList;
	}

	//Gets comments from file.
	fgetc(in);
	fgets(comments, 100, in);

	//Gets rows and columns from file.
	fscanf(in, "%d %d", &r, &c);

	//Gets the threshold from file.
	fscanf(in, "%d", &ft);

	*maxVal = ft;
	*row = r;
	*col = c;
	//stores file data into a memory location.
	imageList = malloc(r*c*sizeof(int));
	for (i = 0; i < r*c; i++){
		fscanf(in, "%d", &num);
		imageList[i] = num;
	}
	if (imageList == NULL){
		printf("Memory problem\n");
		return imageList;
	}
	fclose(in);
	return imageList;
}
void sobelFunc(int *row, int *col, int *maxVal, int* image){
	int i = 0, j = 0, r = *row, c = *col, m = *maxVal, big = 0;
	int gradientx = 0, gradienty = 0, gradient = 0;
	int *outImage = NULL;
	int num[12] = { 0 };
	FILE * out = NULL;

	//write header to output file.
	out = writeHeader(out, r, c, m);
	//malloc an output memory location.
	outImage = malloc(r*c*sizeof(int));
	//fill the border with 0s in the output memory location.
	for (i = 0; i < r; i++){
		outImage[i] = 0;
	}
	for (i = r; i < (r*c) - 1; i = i + r){
		outImage[i] = 0;
	}
	for (i = r + r - 1; i < (r*c) - 1; i = i + r){
		outImage[i] = 0;
	}
	for (i = (r*c) - r; i < (r*c); i++){
		outImage[i] = 0;
	}
	//only use the values not on the edge and find the gradient of each value and put it in the memory location.
	for (j = 1; j < c - 1; j++){
		for (i = (r*j) + 1; i < (r*j) + (r - 1); i++){
			num[0] = image[(i - r) - 1];
			num[1] = image[i - r];
			num[2] = image[(i - r) + 1];
			num[3] = image[(i + r) - 1];
			num[4] = image[i + r];
			num[5] = image[(i + r) + 1];
			num[6] = image[(i - r) - 1];
			num[7] = image[(i + r) - 1];
			num[8] = image[i - 1];
			num[9] = image[i + 1];
			num[10] = image[(i + r) - 1];
			num[11] = image[(i + r) + 1];
			gradientx = (num[0] + (2 * num[1]) + num[2]) - (num[3] + (2 * num[4]) + num[5]);
			gradienty = (num[6] + (2 * num[8]) + num[10]) - (num[7] + (2 * num[9]) + num[11]);
			gradientx = (int)pow(gradientx, 2);
			gradienty = (int)pow(gradienty, 2);
			gradient = (int)sqrt(gradientx + gradienty);
			outImage[i] = gradient;
			//save the largest value for later use
			if (outImage[i] > big){
				big = outImage[i];
			}

		}
	}
	//use the largest value to make values proportial to the max value.
	for (i = 0; i < r*c; i++){
		outImage[i] = (int)(((float)outImage[i] / (float)big) * *maxVal);
	}
	//write the memory to a file.
	for (i = 0; i < r*c; i++){
		fprintf(out, "%d \n", outImage[i]);
	}
	//close the file and free memory.
	fclose(out);
	free(outImage);
	free(image);
	free(row);
	free(col);
	free(maxVal);
}
void flipUD(int *row, int *col, int *maxVal, int* image){
	int *outImage = NULL;
	int i = 0, j = 0, x = 0, r = *row, c = *col, m = *maxVal, big = 0;
	FILE * out = NULL;

	//write header to output file.
	out = writeHeader(out, r, c, m);
	//malloc an output memory location.
	outImage = malloc(r * c * sizeof(int));

	//flip the bottom row with the top.
	for (j = 1; j <= c; j++){
		for (i = (r * c) - (r*j); i <= ((r * c) - (r*j)) + (r - 1); i++){
			outImage[x] = image[i];
			x++;
		}
	}
	//Write the memory to a file.
	for (i = 0; i < (r * c); i++){
		fprintf(out, "%d\n", outImage[i]);
	}
	fclose(out);
	free(image);
	free(outImage);
	free(row);
	free(col);
	free(maxVal);
}
void flipLR(int *row, int *col, int *maxVal, int* image){
	int *outImage = NULL;
	int i = 0, j = 0, x = 0, r = *row, c = *col, m = *maxVal, big = 0;
	FILE * out = NULL;

	//write header to output file.
	out = writeHeader(out, r, c, m);
	//malloc an output memory location.
	outImage = malloc(r * c * sizeof(int));

	//Flip the values for each row.
	for (j = 1; j <= c; j++){
		for (i = (r * j) - 1; i >= ((r * j) - r); i--){
			outImage[x] = image[i];
			x++;
		}
	}

	for (i = 0; i < (r * c); i++){
		fprintf(out, "%d\n", outImage[i]);
	}

	fclose(out);
	free(image);
	free(outImage);
	free(row);
	free(col);
	free(maxVal);
}
void rotateCW(int *row, int *col, int *maxVal, int* image){
	int *outImage = NULL;
	int i = 0, j = 0, x = 0, r = *row, c = *col, m = *maxVal, big = 0;
	FILE * out = NULL;

	//write header to output file with rows and cols switched.
	out = writeHeader(out, c, r, m);
	//malloc an output memory location.
	outImage = malloc(r * c * sizeof(int));
	//turn the image 90 degrees CW
	for (j = 0; j < r; j++){
		for (i = (r * c) - (r - j); i >= 0; i = i - r){
			outImage[x] = image[i];
			x++;
		}
	}

	for (i = 0; i < (r * c); i++){
		fprintf(out, "%d\n", outImage[i]);
	}

	fclose(out);
	free(image);
	free(outImage);
	free(row);
	free(col);
	free(maxVal);
}
void rotateCCW(int *row, int *col, int *maxVal, int* image){
	int *outImage = NULL;
	int i = 0, j = 0, x = 0, r = *row, c = *col, m = *maxVal, big = 0;
	FILE * out = NULL;

	//write header to output file with rows and cols switched.
	out = writeHeader(out, c, r, m);
	//malloc an output memory location.
	outImage = malloc(r * c * sizeof(int));
	//turn the image 90 degrees CCW
	for (j = 1; j <= r; j++){
		for (i = r - j; i <= (r*c); i = i + r){
			outImage[x] = image[i];
			x++;
		}
	}

	for (i = 0; i < (r * c); i++){
		fprintf(out, "%d\n", outImage[i]);
	}

	fclose(out);
	free(image);
	free(outImage);
	free(row);
	free(col);
	free(maxVal);
}
FILE* writeHeader(FILE* out, int r, int c, int max){
	char outputFile[MAX];

	printf("Enter an output file: ");
	scanf("%s", outputFile);
	//write the output files header.
	out = fopen(outputFile, "w");
	fprintf(out, "P2\n");
	fprintf(out, "# comment\n");
	fprintf(out, "%d %d\n", r, c);
	fprintf(out, "%d\n", max);

	return out;
}