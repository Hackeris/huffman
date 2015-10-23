#ifndef HUFFMAN_FILE_EXTRACTOR
#define HUFFMAN_FILE_EXTRACTOR

#include "def.h"
#include <iostream>
#include "byte_freq_map.h"
#include "huffman_tree.h"

class huffman_file_extractor {
public:
	void extract(std::istream& in, std::ostream& out);
};

#endif