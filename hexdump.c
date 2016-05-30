/*
 * hexdump.c
 *
 *  Created on: May 30, 2016
 *      Author: Moawiya
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define A 10
#define B 11
#define C 12
#define D 13
#define E 14
#define F 15
#define HEX 16

char itoc(int n) {
	switch (n) {
	case A:
		return 'A';
	case B:
		return 'B';
	case C:
		return 'C';
	case D:
		return 'D';
	case E:
		return 'E';
	case F:
		return 'F';
	default:
		return n + '0';
	}
}

void iton(int input, char* output, int base) {
	if (output == NULL) {
		return;
	}
	int count = 0;
	char buf[10], *pntr;
	pntr = buf;
	while ((input / base) != 0) {
		int div = input % base;
		char c = itoc(div);
		*pntr++ = c;
		input = input / base;
		count++;
	}
	*pntr++ = itoc(input);
	if (count == 0)
		*pntr++ = '0';
	*pntr-- = 0;
	while (pntr != buf) {
		*output++ = *pntr--;
	}
	*output++ = *pntr;
	*output = 0;
}

void dumpFile(FILE * input, FILE* output) {
	if (input == NULL || output == NULL)
		return;
	char c;
	int count = 0;
	char in[17], *pntr;
	pntr = in;
	while (fscanf(input, "%c", &c) != EOF) {
		char wr[5];
		iton((int) c, wr, HEX);
		if (c < 32)
			c = '.';
		if (count == 15) {
			*pntr++ = c;
			*pntr = 0;
			fprintf(output, "%s     %s\n", wr, in);
			count = 0;
			pntr = in;
		} else {
			fprintf(output, "%s ", wr);
			*pntr++ = c;
			count++;
		}
	}
	*pntr = 0;
	while (count++ != 15) {
		fprintf(output, "%s ", "  ");
	}
	fprintf(output, "       %s", in);
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		errno = EINVAL;
		perror("No input file was given");
		return 1;
	}
	if (argc > 2) {
		errno = E2BIG;
		perror("Only one argument allowed");
		return 1;
	}
	FILE* input, *output;
	char *file_name, *output_file;
	file_name = argv[1];
	//while (fgets(file_name, INPUT_SIZE, stdin) == NULL)
		//;
	//file_name[strlen(file_name) - 1] = 0;
	input = fopen(file_name, "r");
	if (!input) {
		perror(file_name);
		return 1;
	}
	output_file = strcat(file_name, ".dump");
	output = fopen(output_file, "w");
	//main function
	dumpFile(input, output);

	fclose(input);
	fclose(output);
	fflush( stdout);
	printf("\nDone\nresult saved to file: %s\n", output_file);
	return 0;
}
