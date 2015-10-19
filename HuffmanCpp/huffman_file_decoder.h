#ifndef HUFFMAN_FILE_DECODER
#define HUFFMAN_FILE_DECODER

#include <iostream>
#include "char_count_map.h"
#include "huffman_tree.h"

class huffman_file_decoder {
public:
	huffman_file_decoder(std::istream& in)
		:in(in) {
	}
	void output_to_file(std::ostream& out) {
		char_count_map cmap;
		huffman_tree tree;
		int length;
		cmap.from_stream(this->in);
		tree.build_from(cmap);
		in.read((char*)&length, sizeof(int));

		huffman_stream_decoder decoder = tree.to_byte_decoder();
		decoder.set_code_length_of_bit(length);
		decoder.decode(this->in);
		decoder.output_to_stream(out);
	}
	~huffman_file_decoder() {
	}
private:
	std::istream& in;
};

#endif