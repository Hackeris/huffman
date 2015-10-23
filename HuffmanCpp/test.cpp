#include <iostream>
#include <fstream>
#include "byte_freq_map.h"
#include "huffman_tree.h"
#include "huffman_file_encoder.h"
#include "huffman_file_compressor.h"
#include "huffman_file_extractor.h"


void test_tree() {

	std::ifstream in("test.txt");
	byte_freq_map table;
	table.load_from_file(in);
	in.close();
	huffman_tree** forest = table.to_simple_huffman_forest();
	huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

	tree->free_nodes();
	delete forest;
	delete tree;
}

void test_encode() {

	std::ifstream in("test.txt");
	std::ofstream out("comp.txt");
	byte_freq_map table;
	table.load_from_file(in);
	huffman_tree** forest = table.to_simple_huffman_forest();
	huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

	in.clear();
	in.seekg(0, std::ios::beg);

	huffman_file_encoder encoder = tree->to_file_encoder();
	encoder.encode_file(in);
	long long length = encoder.write_to_file(out);

	in.close();
	out.close();

	huffman_file_decoder decoder = tree->to_file_decoder();
	std::ifstream cmp("comp.txt", std::ios::binary);
	std::ofstream ex("ext.txt", std::ios::binary);
	decoder.decode_file(cmp, length);
	decoder.write_to_file(ex);

	tree->free_nodes();
	delete forest;
	delete tree;
}

void test_compress() {
	std::ifstream in("test.txt", std::ios::binary);
	std::ofstream out("compressed.txt", std::ios::binary);

	huffman_file_compressor compressor;
	compressor.compress(in, out);

	in.close();
	out.close();
}

void test_extract() {
	std::ifstream in("compressed.txt", std::ios::binary);
	std::ofstream out("extracted.txt", std::ios::binary);

	huffman_file_extractor extractor;
	extractor.extract(in, out);

	in.close();
	out.close();
}

void test_compress_and_extract() {

	test_compress();
	test_extract();
}

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

//int main() {
//
//	test_tree();
//	test_encode();
//	test_compress_and_extract();
//
//	_CrtDumpMemoryLeaks();
//
//	return 0;
//}
