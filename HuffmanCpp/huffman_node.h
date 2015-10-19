#ifndef HUFFMAN_NODE
#define HUFFMAN_NODE

#include <cstdlib>
#include "def.h"

class huffman_node {
public:
	byte c;
	int count;
	huffman_node* left, *right;
public:
	huffman_node() {
		this->c = 0;
		this->count = 0;
		this->left = this->right = NULL;
	}
	huffman_node(huffman_node* left, huffman_node* right) {
		this->count = 0;
		if (left != NULL) {
			this->count += left->count;
		}
		if (right != NULL) {
			this->count += right->count;
		}
		this->c = 0;
		this->left = left;
		this->right = right;
	}
};


#endif