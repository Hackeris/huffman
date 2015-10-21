#ifndef _H_ENCODE_
#define _H_ENCODE_

#include "huffman.h"

#define MAX_CODE_LENGTH 256

typedef struct _byte_code_map {
	char* map[MAX_BYTE_COUNT];
}byte_code_map;


void travel_huffman_tree(huffman_node* node, byte_code_map* bcmap,
	char* tmp_code, int *code_len) {

	if (node->left == NULL && node->right == NULL) {
		bcmap->map[node->value] = strdup(tmp_code);
	}
	if (node->left != NULL) {
		tmp_code[(*code_len)++] = '0';
		travel_huffman_tree(node->left, bcmap, tmp_code, code_len);
		tmp_code[--(*code_len)] = '\0';
	}
	if (node->right != NULL) {
		tmp_code[(*code_len)++] = '1';
		travel_huffman_tree(node->right, bcmap, tmp_code, code_len);
		tmp_code[--(*code_len)] = '\0';
	}
}

byte_code_map* build_byte_to_code_map(huffman_tree* htree) {

	char tmp_code[256] = { 0 };
	int code_length = 0;
	byte_code_map* bcmap = (byte_code_map*)malloc(sizeof(byte_code_map));
	memset(bcmap, 0, sizeof(byte_code_map));

	travel_huffman_tree(htree, bcmap, tmp_code, &code_length);
	return bcmap;
}

void free_byte_to_code_map(byte_code_map* bcmap) {
	int i;
	for (i = 0; i < MAX_BYTE_COUNT; i++) {
		if (bcmap->map[i] != NULL) {
			free(bcmap->map[i]);
		}
	}
	free(bcmap);
}

int mini(int a, int b) {
	return a < b ? a : b;
}

byte string_to_byte(const char* str, int n) {
	byte c = 0;
	int i;
	for (i = 0; i < n; i++) {
		c |= (str[i] == '1' ? (1 << (7 - i)) : 0);
	}
	return c;
}

void shift_left_buffer(char* buffer, int* start, int* end) {

	int i;
	if (*start > 8) {
		int i;
		for (i = 0; i < *end - *start; i++) {
			buffer[i] = buffer[i + *start];
		}
		*end -= *start;
		*start = 0;
	}
}

int encode_file_to(byte_code_map* bcmap, FILE* fp_in, FILE* fp_out) {

	char code_buffer[MAX_CODE_LENGTH];
	int start = 0, end = 0;
	int code_length = 0;

	byte b;
	fread(&b, sizeof(byte), 1, fp_in);
	while (!feof(fp_in)) {

		strcpy(&code_buffer[end], bcmap->map[b]);
		code_length += strlen(bcmap->map[b]);
		end += strlen(bcmap->map[b]);

		while (end - start > 8) {
			byte bo = string_to_byte(&code_buffer[start], 8);
			fwrite(&bo, sizeof(byte), 1, fp_out);
			start += 8;
		}

		shift_left_buffer(code_buffer, &start, &end);

		fread(&b, sizeof(byte), 1, fp_in);
	}
	int n = mini(end - start, 8);
	if (n == 0) {
		return code_length;
	}
	b = string_to_byte(&code_buffer[start], n);
	start += n;
	fwrite(&b, sizeof(byte), 1, fp_out);
	return code_length;
}

#endif