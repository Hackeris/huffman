#include "decode.h"

byte_to_bit_stream* open_bit_stream(FILE* fp_in, long long code_length) {

	byte_to_bit_stream* btb =
		(byte_to_bit_stream*)malloc(sizeof(byte_to_bit_stream));
	btb->fp_in = fp_in;
	fread(&btb->buffer, sizeof(byte), 1, btb->fp_in);
	btb->offset = btb->buffer_offset = 0;
	btb->code_length = code_length;
	return btb;
}

static int bfeof(byte_to_bit_stream* btbs) {
	return btbs->offset >= btbs->code_length;
}

void close_bit_stream(byte_to_bit_stream* btbs) {
	free(btbs);
}

//	从文件中读取字符，并转化为编码位流
static int read_bit(byte_to_bit_stream* btbs) {

	if (btbs->buffer_offset >= 8) {
		btbs->buffer_offset = 0;
		fread(&btbs->buffer, sizeof(byte), 1, btbs->fp_in);
	}
	int bit = (btbs->buffer & (1 << (7 - btbs->buffer_offset))) ? 1 : 0;
	btbs->offset++;
	btbs->buffer_offset++;
	return bit;
}

static byte read_decoded_byte(byte_to_bit_stream* in, huffman_tree* htree) {
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

//	将输入文件中的编码，按照哈夫曼树的结构，解码后写入输出文件
void decode_to_file(huffman_tree* htree, FILE* fp_in,
	FILE* fp_out, long long code_length) {

	byte_to_bit_stream* btbs = open_bit_stream(fp_in, code_length);
	while (!bfeof(btbs)) {
		byte b = read_decoded_byte(btbs, htree);
		fwrite(&b, sizeof(byte), 1, fp_out);
	}
	close_bit_stream(btbs);
}