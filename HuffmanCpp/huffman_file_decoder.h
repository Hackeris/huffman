#ifndef HUFFMAN_FILE_DECODER
#define HUFFMAN_FILE_DECODER

#include "def.h"
#include <iostream>
#include "huffman_node.h"
#include "leaf_node.h"
#include "internal_node.h"
#include "byte_to_bit_stream_adapter.h"

class huffman_file_decoder {
public:
	huffman_file_decoder(huffman_node* tree) {
		this->tree_root = tree;
	}
	void decode_file(std::istream& in, int code_length) {
		this->in = new byte_to_bit_stream_adapter(in);
		this->in->init(code_length);
	}
	void write_to_file(std::ostream& out) {
		while (!in->end()) {
			byte b = read_encoded_byte();
			out.write((char*)&b, sizeof(byte));
		}
	}
	~huffman_file_decoder() {
		delete in;
	}
private:
	byte read_encoded_byte() {
		huffman_node* node = this->tree_root;
		while (!node->is_leaf()) {
			internal_node* inode = static_cast<internal_node*>(node);
			int bit = this->in->read_bit();
			if (bit == 0) {
				node = inode->left();
			}
			else {
				node = inode->right();
			}
		}
		leaf_node* leaf = static_cast<leaf_node*>(node);
		return leaf->val();
	}
private:
	huffman_node* tree_root;
	byte_to_bit_stream_adapter *in;
	byte buffer;
};

#endif