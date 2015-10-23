#ifndef HUFFMAN_FILE_COMPRESSOR
#define HUFFMAN_FILE_COMPRESSOR

#include "def.h"
#include <iostream>
#include "huffman_tree.h"
#include "huffman_file_encoder.h"
#include "byte_freq_map.h"

class huffman_file_compressor {
public:
	void compress(std::istream& in, std::ostream& out);
};

#endif