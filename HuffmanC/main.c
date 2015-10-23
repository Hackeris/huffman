#include <stdio.h>

#include "file.h"

char* get_command_file_name(char* path) {

	path = &path[strlen(path) - 1];
	while (*path != '\\' && *path != '/') {
		path--;
	}
	return ++path;
}

void usage(char* path) {

	printf("Usage:\n");
	printf("\tcompress: %s -c in out\n", get_command_file_name(path));
	printf("\textract: %s -x in out\n", get_command_file_name(path));
}

void compress(char* input_file, char* output_file) {

	FILE* fp_in = fopen(input_file, "rb");
	FILE* fp_out = fopen(output_file, "wb");

	compress_file(fp_in, fp_out);

	fclose(fp_in);
	fclose(fp_out);
}

void extract(char* input_file, char* output_file) {

	FILE* fp_in = fopen(input_file, "rb");
	FILE* fp_out = fopen(output_file, "wb");

	extract_file(fp_in, fp_out);

	fclose(fp_in);
	fclose(fp_out);
}

int main(int argc, char **argv) {

	if (argc < 4) {
		usage(argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "-c") == 0) {
		compress(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "-x") == 0) {
		extract(argv[2], argv[3]);
	}

	return 0;
}
