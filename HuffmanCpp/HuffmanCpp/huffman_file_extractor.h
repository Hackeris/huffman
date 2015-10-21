#ifndef HUFFMAN_FILE_EXTRACTOR
#define HUFFMAN_FILE_EXTRACTOR

#include "def.h"
#include <iostream>
#include "byte_count_map.h"
#include "huffman_tree.h"

class huffman_file_extractor {
public:
	void extract(std::istream& in, std::ostream& out) {
		byte_count_table table;
		table.read_from_file(in);
		huffman_tree** forest = table.to_simple_huffman_forest();
		huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

		int length;
		int offset = in.tellg();
		in.read((char*)&length, sizeof(int));

		huffman_file_decoder decoder = tree->to_file_decoder();
		decoder.decode_file(in, length);
		decoder.write_to_file(out);

		tree->free_nodes();
		delete forest;
		delete tree;
	}
};

#endif