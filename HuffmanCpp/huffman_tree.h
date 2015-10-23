#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include "def.h"
#include <iostream>
#include <queue>
#include "huffman_node.h"
#include "leaf_node.h"
#include "internal_node.h"
#include "priority_queue.h"
#include "huffman_file_encoder.h"
#include "huffman_file_decoder.h"

template <typename T>
class huffman_tree_compare {
public:
	static bool smaller(const T *l, const T *r) {
		return l->weight() < r->weight();
	}

	static bool larger(const T *l, const T *r) {
		return smaller(r, l);
	}
};

class huffman_tree {
public:
	huffman_tree(const byte &val, int freq);

	huffman_tree(huffman_tree *l, huffman_tree *r);

	huffman_node *root();

	int weight() const;

	static huffman_tree *biuld_huffman_tree(
		huffman_tree **tree_array, int count);

	void free_nodes();

	huffman_file_encoder to_file_encoder();

	huffman_file_decoder to_file_decoder();

	~huffman_tree();
private:
	void travel(huffman_node* node, std::string& code,
		encode_map& encode_map);
private:
	huffman_node *root_node;
};

#endif