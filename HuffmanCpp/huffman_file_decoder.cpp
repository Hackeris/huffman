#include "huffman_file_decoder.h"

huffman_file_decoder::huffman_file_decoder(huffman_node* tree) {
	this->tree_root = tree;
}

void huffman_file_decoder::decode_file(std::istream& in, long long code_length) {
	this->in = new byte_to_bit_adapter(in);
	this->in->init(code_length);
}

//	从文件中读出编码，获得对应的，符号，写入解压后的文件
void huffman_file_decoder::write_to_file(std::ostream& out) {
	while (!in->end()) {
		byte b = read_encoded_byte();
		out.write((char*)&b, sizeof(byte));
	}
}

huffman_file_decoder::~huffman_file_decoder() {
	delete in;
}

byte huffman_file_decoder::read_encoded_byte() {
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
