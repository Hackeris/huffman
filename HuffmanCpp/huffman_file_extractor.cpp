#include "huffman_file_extractor.h"

void huffman_file_extractor::extract(std::istream& in, std::ostream& out) {
	byte_freq_map table;
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