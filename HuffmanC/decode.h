#ifndef _H_DECODE_
#define _H_DECODE_

#include "huffman.h"

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

#endif