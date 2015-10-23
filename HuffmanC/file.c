#include "file.h"

void compress_file(FILE* fp_in, FILE* fp_out) {

	byte_freq_map *freq_map = get_freq_map(fp_in);

	huffman_tree* htree = build_huffman_tree(freq_map);
	byte_code_map* bcmap = build_byte_to_code_map(htree);

	fseek(fp_in, 0, SEEK_SET);
	save_freq_map(freq_map, fp_out);
	int offset = ftell(fp_out);
	fseek(fp_out, sizeof(int), SEEK_CUR);
	int code_length = encode_file_to(bcmap, fp_in, fp_out);
	fseek(fp_out, offset, SEEK_SET);
	fwrite(&code_length, sizeof(int), 1, fp_out);

	free_huffman_tree(htree);
	free(freq_map);
	free_byte_to_code_map(bcmap);
}

void extract_file(FILE* fp_in, FILE* fp_out) {

	byte_freq_map* freq_map = read_freq_map(fp_in);
	huffman_tree* htree = build_huffman_tree(freq_map);

	int code_length;
	fread(&code_length, sizeof(int), 1, fp_in);

	decode_to_file(htree, fp_in, fp_out, code_length);

	free_huffman_tree(htree);
	free(freq_map);
}
