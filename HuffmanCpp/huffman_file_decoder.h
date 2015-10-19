#ifndef HUFFMAN_FILE_DECODER
#define HUFFMAN_FILE_DECODER

#include <iostream>
#include "char_count_map.h"
#include "huffman_tree.h"

class huffman_file_decoder {
public:
	huffman_file_decoder(std::istream& in);
	void output_to_file(std::ostream& out);
	~huffman_file_decoder();
private:
	std::istream& in;
};

#endif