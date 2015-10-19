#ifndef HUFFMAN_BYTE_DECODER
#define HUFFMAN_BYTE_DECODER

#include <iostream>
#include "def.h"
#include "byte_stream_to_bit_adapter.h"
#include "huffman_node.h"

class huffman_stream_decoder {
public:
	huffman_stream_decoder(huffman_node* tree) {
		this->root = tree;
		bit_stream = NULL;
	}
	void decode(std::istream& in) {
		this->bit_stream = new byte_stream_to_bit_adapter(in);
	}
	void set_code_length_of_bit(int length) {
		this->code_length = length;
	}
	void output_to_stream(std::ostream& out) {
		byte b;
		while (read_byte(b)) {
			out.write((char*)&b, sizeof(byte));
		}
	}
	~huffman_stream_decoder() {
		if (bit_stream != NULL) {
			delete bit_stream;
			bit_stream = NULL;
		}
	}
private:
	int read_byte(byte& b) {
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
private:
	int code_length;
	huffman_node* root;
	byte_stream_to_bit_adapter *bit_stream;
};

#endif