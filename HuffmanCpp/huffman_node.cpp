#include "huffman_node.h"

huffman_node::huffman_node() {
	this->c = 0;
	this->count = 0;
	this->left = this->right = NULL;
}

huffman_node::huffman_node(huffman_node* left, huffman_node* right) {
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
