#include "huffman_file_extractor.h"

//	解压文件的功能，从文件中读取出符号与频数的映射表，建立对应的哈夫曼树
//	将文件输入流转化为位流，根据哈夫曼树进行解码
//	写入新的文件
void huffman_file_extractor::extract(std::istream& in, std::ostream& out) {
	byte_freq_map table;
	table.read_from_file(in);
	huffman_tree** forest = table.to_simple_huffman_forest();
	huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

	long long length;
	std::streamoff offset = in.tellg();
	in.read((char*)&length, sizeof(long long));

	huffman_file_decoder decoder = tree->to_file_decoder();
	decoder.decode_file(in, length);
	decoder.write_to_file(out);

	tree->free_nodes();
	delete forest;
	delete tree;
}