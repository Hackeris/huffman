#ifndef HUFFMAN_FILE_DECODER
#define HUFFMAN_FILE_DECODER

#include "def.h"
#include <iostream>
#include "huffman_node.h"
#include "leaf_node.h"
#include "internal_node.h"
#include "byte_to_bit_adapter.h"

class huffman_file_decoder {
public:
	huffman_file_decoder(huffman_node* tree);

	void decode_file(std::istream& in, int code_length);

	void write_to_file(std::ostream& out);

	~huffman_file_decoder();
private:
	byte read_encoded_byte();
private:
	huffman_node* tree_root;
	byte_to_bit_adapter *in;
	byte buffer;
};

#endif