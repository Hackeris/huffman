#ifndef HUFFMAN_FILE_ENCODER
#define HUFFMAN_FILE_ENCODER

#include <iostream>
#include "char_count_map.h"
#include "huffman_tree.h"

class huffman_file_encoder {
public:
	huffman_file_encoder(std::istream &in)
		:in(in) {
	}
	void output_to_file(std::ostream& out) {
		char_count_map cmap;
		huffman_tree tree;
		cmap.load(in);
		in.clear();
		in.seekg(0, std::ios::beg);
		tree.build_from(cmap);

		cmap.to_stream(out);
		huffman_stream_encoder encoder = tree.to_byte_encoder();
		encoder.encode(in);
		int length = 0;
		std::streamoff curr = out.tellp();
		out.write((char*)&length, sizeof(int));
		encoder.output_to_stream(out);
		length = encoder.get_code_length_of_bits();
		out.seekp(curr, std::ios::beg);
		out.write((char*)&length, sizeof(int));
	}
	~huffman_file_encoder() {
	}
private:
	std::istream& in;
};


#endif
