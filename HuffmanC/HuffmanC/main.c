#include <stdio.h>

#define MAX_BYTE_COUNT	256

typedef unsigned char byte;

typedef struct _huffman_node {
	byte value;
	int freq;
	struct _huffman_node *left, *right;
}huffman_node;


typedef struct _byte_freq_map {
	int map[MAX_BYTE_COUNT];
}byte_freq_map;


typedef struct _file_bit_stream {
	int start;
	byte buffer;

}file_bit_stream;


int main() {

	return 0;
}
