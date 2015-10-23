#ifndef _H_ENCODE_
#define _H_ENCODE_

#include "huffman.h"

#define MAX_CODE_LENGTH 256

typedef struct _byte_code_map {
	char* map[MAX_BYTE_COUNT];
}byte_code_map;

byte_code_map* build_byte_to_code_map(huffman_tree* htree);

void free_byte_to_code_map(byte_code_map* bcmap);

int encode_file_to(byte_code_map* bcmap, FILE* fp_in, FILE* fp_out);

#endif