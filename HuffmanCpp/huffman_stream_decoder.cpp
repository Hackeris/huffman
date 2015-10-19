#include "huffman_stream_decoder.h"

huffman_stream_decoder::huffman_stream_decoder(huffman_node* tree) {
	this->root = tree;
	bit_stream = NULL;
}

void huffman_stream_decoder::decode(std::istream& in) {
	this->bit_stream = new byte_stream_to_bit_adapter(in);
}

void huffman_stream_decoder::set_code_length_of_bit(int length) {
	this->code_length = length;
}

void huffman_stream_decoder::output_to_stream(std::ostream& out) {
	byte b;
	while (read_byte(b)) {
		out.write((char*)&b, sizeof(byte));
	}
}

huffman_stream_decoder::~huffman_stream_decoder() {
	if (bit_stream != NULL) {
		delete bit_stream;
		bit_stream = NULL;
	}
}

int huffman_stream_decoder::read_byte(byte& b) {
	huffman_node* node = root;
	if (this->bit_stream->current() >= this->code_length) {
		return 0;
	}
	while (node->left != NULL &&
		node->right != NULL) {
		int bit = this->bit_stream->read_bit();
		if (bit == 0) {
			node = node->left;
		}
		if (bit == 1) {
			node = node->right;
		}
	}
	if (node->left != NULL || node->right != NULL) {
		return 0;
	}
	b = node->c;
	return 1;
}
