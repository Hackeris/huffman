#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BYTE_COUNT	256
#define MAX_CODE_LENGTH 256

typedef unsigned char byte;

typedef struct _huffman_node {
	byte value;
	int freq;
	struct _huffman_node *left, *right;
}huffman_node;

typedef huffman_node huffman_tree;

typedef struct _byte_freq_map {
	int map[MAX_BYTE_COUNT];
}byte_freq_map;

typedef struct _byte_code_map {
	char* map[MAX_BYTE_COUNT];
}byte_code_map;

byte_freq_map* get_freq_map(FILE* fp) {

	byte_freq_map* freq_map = 
		(byte_freq_map*)malloc(sizeof(byte_freq_map));
	memset(freq_map, 0, sizeof(byte_freq_map));


	byte b;
	fread(&b, sizeof(byte), 1, fp);
	while (!feof(fp)) {
		freq_map->map[b] ++;
		fread(&b, sizeof(byte), 1, fp);
	}

	return freq_map;
}

int get_available_count_of_map_item(byte_freq_map* fmap) {

	int available_count = 0;
	int i;
	for (i = 0; i < MAX_BYTE_COUNT; i++) {
		if (fmap->map[i] != 0) {
			available_count++;
		}
	}
	return available_count;
}

int huffman_cmp(const void* lc,const void* rc) {
	return -((*(huffman_tree**)lc)->freq 
		- (*(huffman_tree**)rc)->freq);
}

huffman_tree** dump_to_forest(byte_freq_map* fmap, int* size) {

	int available_count = get_available_count_of_map_item(fmap);
	huffman_tree** forest = (huffman_tree**)malloc(
		sizeof(huffman_tree*) * available_count);
	memset(forest, 0, sizeof(huffman_tree*) * available_count);
	int i, j = 0;
	for (i = 0; i < MAX_BYTE_COUNT; i++) {
		if (fmap->map[i] != 0) {
			forest[j] = (huffman_tree*)malloc(sizeof(huffman_tree));
			forest[j]->value = i;
			forest[j]->freq = fmap->map[i];
			forest[j]->left = forest[j]->right = 0;
			j++;
		}
	}
	*size = available_count;
	return forest;
}

void insert_to_huffman_forest(huffman_tree *tree, 
	huffman_tree** forest, int* size) {

	forest[(*size)++] = tree;
	int last = *size - 1;
	while (last > 0 && forest[last]->freq > forest[last - 1]->freq) {
		huffman_tree* tmp = forest[last];
		forest[last] = forest[last - 1];
		forest[last - 1] = tmp;
		last--;
	}
}

huffman_tree *get_minimal_tree(huffman_tree** forest, int*size) {
	return forest[--*size];
}

huffman_tree *build_huffman_tree(byte_freq_map* fmap) {

	int forest_size;
	huffman_tree *htree;
	huffman_tree** forest = dump_to_forest(fmap, &forest_size);
	qsort(forest, forest_size, sizeof(huffman_tree*), huffman_cmp);

	while (forest_size > 1) {
		huffman_tree* tmp1 = get_minimal_tree(forest, &forest_size);
		huffman_tree* tmp2 = get_minimal_tree(forest, &forest_size);
		huffman_tree* tmp3 = (huffman_tree*)malloc(sizeof(huffman_tree));
		tmp3->left = tmp1;
		tmp3->right = tmp2;
		tmp3->freq = tmp1->freq + tmp2->freq;
		insert_to_huffman_forest(tmp3, forest, &forest_size);
	}
	htree = forest[0];
	free(forest);
	return htree;
}

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

void free_buffman_node(huffman_tree* hnode) {
	if (hnode->left == NULL && hnode->right == NULL) {
		free(hnode);
		return;
	}
	if (hnode->left != NULL) {
		free_buffman_node(hnode->left);
	}
	if (hnode->right != NULL) {
		free_buffman_node(hnode->right);
	}
	free(hnode);
}

void free_huffman_tree(huffman_tree* htree) {
	free_buffman_node(htree);
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

typedef struct _byte_to_bit_stream {
	FILE* fp_in;
	byte buffer;
	int code_length;
	int offset;
	int buffer_offset;
}byte_to_bit_stream;

byte_to_bit_stream* open_bit_stream(FILE* fp_in, int code_length) {

	byte_to_bit_stream* btb =
		(byte_to_bit_stream*)malloc(sizeof(byte_to_bit_stream));
	btb->fp_in = fp_in;
	fread(&btb->buffer, sizeof(byte), 1, btb->fp_in);
	btb->offset = btb->buffer_offset = 0;
	btb->code_length = code_length;
	return btb;
}

int bfeof(byte_to_bit_stream* btbs) {
	return btbs->offset >= btbs->code_length;
}

void close_bit_stream(byte_to_bit_stream* btbs) {
	free(btbs);
}

int read_bit(byte_to_bit_stream* btbs) {

	if (btbs->buffer_offset >= 8) {
		btbs->buffer_offset = 0;
		fread(&btbs->buffer, sizeof(byte), 1, btbs->fp_in);
	}
	int bit = (btbs->buffer & (1 << (7 - btbs->buffer_offset))) ? 1 : 0;
	btbs->offset++;
	btbs->buffer_offset++;
	return bit;
}

byte read_decoded_byte(byte_to_bit_stream* in, huffman_tree* htree) {
	huffman_node* hnode = htree;
	while (!(hnode->left == NULL && hnode->right == NULL)) {
		int bit = read_bit(in);
		if (bit == 0) {
			hnode = hnode->left;
		}
		else if (bit == 1) {
			hnode = hnode->right;
		}
	}
	return hnode->value;
}

void decode_to_file(huffman_tree* htree, FILE* fp_in,
	FILE* fp_out, int code_length) {

	byte_to_bit_stream* btbs = open_bit_stream(fp_in, code_length);
	while (!bfeof(btbs)) {
		byte b = read_decoded_byte(btbs, htree);
		fwrite(&b, sizeof(byte), 1, fp_out);
	}
	close_bit_stream(btbs);
}

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

void test_freq_map() {

	FILE* fp = fopen("test.txt", "rb");
	byte_freq_map *freq_map = get_freq_map(fp);

	free(freq_map);
	fclose(fp);
}

void test_build_huffman_tree() {

	FILE* fp = fopen("test.txt", "rb");
	byte_freq_map *freq_map = get_freq_map(fp);

	huffman_tree* htree = build_huffman_tree(freq_map);

	free_huffman_tree(htree);
	free(freq_map);
	fclose(fp);
}

void test_build_code_map() {

	FILE* fp = fopen("test.txt", "rb");
	byte_freq_map *freq_map = get_freq_map(fp);

	huffman_tree* htree = build_huffman_tree(freq_map);
	byte_code_map* bcmap = build_byte_to_code_map(htree);

	free_huffman_tree(htree);
	free(freq_map);
	free_byte_to_code_map(bcmap);
	fclose(fp);
}

void test_encode_file() {

	FILE* fp = fopen("test.txt", "rb");
	byte_freq_map *freq_map = get_freq_map(fp);

	huffman_tree* htree = build_huffman_tree(freq_map);
	byte_code_map* bcmap = build_byte_to_code_map(htree);

	FILE* fp_out = fopen("com.txt", "wb");
	fseek(fp, 0, SEEK_SET);
	int code_length = encode_file_to(bcmap, fp, fp_out);

	free_huffman_tree(htree);
	free(freq_map);
	free_byte_to_code_map(bcmap);
	fclose(fp);
	fclose(fp_out);
}

void test_decode_file() {

	FILE* fp = fopen("test.txt", "rb");
	byte_freq_map *freq_map = get_freq_map(fp);

	huffman_tree* htree = build_huffman_tree(freq_map);
	byte_code_map* bcmap = build_byte_to_code_map(htree);

	FILE* fp_out = fopen("com.txt", "wb");
	fseek(fp, 0, SEEK_SET);
	int code_length = encode_file_to(bcmap, fp, fp_out);
	fclose(fp_out);

	fp_out = fopen("com.txt", "rb");
	FILE* fp_to = fopen("out.txt", "wb");
	decode_to_file(htree, fp_out, fp_to, code_length);
	
	free_huffman_tree(htree);
	free(freq_map);
	free_byte_to_code_map(bcmap);
	fclose(fp);
	fclose(fp_out);
	fclose(fp_to);
}

int main() {

	test_freq_map();
	test_build_huffman_tree();
	test_build_code_map();
	test_encode_file();
	test_decode_file();

	_CrtDumpMemoryLeaks();

	return 0;
}
