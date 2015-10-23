#include "huffman_file_compressor.h"

void huffman_file_compressor::compress(std::istream& in, std::ostream& out) {
	byte_freq_map table;
	table.load_from_file(in);
	table.save_to_file(out);
	huffman_tree** forest = table.to_simple_huffman_forest();
	huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

	in.clear();
	in.seekg(0, std::ios::beg);

	huffman_file_encoder encoder = tree->to_file_encoder();
	encoder.encode_file(in);
	std::streamoff length_offset = out.tellp();
	out.seekp(sizeof(long long), std::ios::cur);
	long long length = encoder.write_to_file(out);
	out.seekp(length_offset, std::ios::beg);
	out.write((char*)&length, sizeof(long long));

	tree->free_nodes();
	delete forest;
	delete tree;
}
