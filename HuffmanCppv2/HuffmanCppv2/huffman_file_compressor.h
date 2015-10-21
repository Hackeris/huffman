#ifndef HUFFMAN_FILE_COMPRESSOR
#define HUFFMAN_FILE_COMPRESSOR

#include "def.h"
#include <iostream>
#include "huffman_tree.h"
#include "huffman_file_encoder.h"
#include "byte_count_map.h"

class huffman_file_compressor {
public:
	void compress(std::istream& in, std::ostream& out) {
		byte_count_table table;
		table.load_from_file(in);
		table.save_to_file(out);
		huffman_tree** forest = table.to_simple_huffman_forest();
		huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

		in.clear();
		in.seekg(0, std::ios::beg);

		huffman_file_encoder encoder = tree->to_file_encoder();
		encoder.encode_file(in);
		int length_offset = out.tellp();
		out.seekp(sizeof(int), std::ios::cur);
		int length = encoder.write_to_file(out);
		out.seekp(length_offset, std::ios::beg);
		out.write((char*)&length, sizeof(int));

		tree->free_nodes();
		delete forest;
		delete tree;
	}
};

#endif