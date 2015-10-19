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
	huffman_node();
	huffman_node(huffman_node* left, huffman_node* right);
};


#endif