#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE 

#include "char_count_map.h"
#include "huffman_stream_decoder.h"
#include "huffman_stream_encoder.h"

class huffman_tree {
public:
	huffman_tree();
	huffman_stream_encoder to_byte_encoder();
	huffman_stream_decoder to_byte_decoder();
	void build_from(const char_count_map& cmap);
	void free_huffman_tree_node(huffman_node* node);
	~huffman_tree();
private:
	static int get_avaliable_count(const char_count_map& cmap);
	static int cmp_node(const void* l, const void* r);
	void init_data_from_map(const char_count_map& cmap);
	//return value:
	//	are there more than 2 nodes in the chars?
	bool resort_huffman_nodes(int start, int length);
	//return value:
	//	are there more than 2 nodes?
	bool get_minimal_2nodes(huffman_node**nodes, int start, int length);
	huffman_node* merge_2nodes(huffman_node* left, huffman_node* right);
	void build_huffman_tree();
	void travel_huffman_tree(std::string& huffman_code, 
		huffman_node* node, huffman_char_code_map& ccmap);
	huffman_char_code_map *to_char_code_map();
private:
	int avaliable_count;
	huffman_node** chars;
	huffman_node* root;
};

#endif