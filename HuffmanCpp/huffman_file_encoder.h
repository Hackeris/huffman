#ifndef HUFFMAN_FILE_ENCODER
#define HUFFMAN_FILE_ENCODER

#include <iostream>
#include "char_count_map.h"
#include "huffman_tree.h"

class huffman_file_encoder {
public:
	huffman_file_encoder(std::istream &in);
	void output_to_file(std::ostream& out);
	~huffman_file_encoder();
private:
	std::istream& in;
};


#endif
