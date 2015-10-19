#ifndef HUFFMAN_BYTE_DECODER
#define HUFFMAN_BYTE_DECODER

#include <iostream>
#include "def.h"
#include "byte_stream_to_bit_adapter.h"
#include "huffman_node.h"

class huffman_stream_decoder {
public:
	huffman_stream_decoder(huffman_node* tree);
	void decode(std::istream& in);
	void set_code_length_of_bit(int length);
	void output_to_stream(std::ostream& out);
	~huffman_stream_decoder();
private:
	int read_byte(byte& b);
private:
	int code_length;
	huffman_node* root;
	byte_stream_to_bit_adapter *bit_stream;
};

#endif