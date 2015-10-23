#include "huffman_tree.h"

huffman_tree::huffman_tree(const byte &val, int freq) {
	this->root_node = new leaf_node(val, freq);
}

huffman_tree::huffman_tree(huffman_tree *l, huffman_tree *r) {
	this->root_node = new internal_node(l->root(), r->root());
}

huffman_node *huffman_tree::root() {
	return this->root_node;
}

int huffman_tree::weight() const {
	return this->root_node->weight();
}

huffman_tree *huffman_tree::biuld_huffman_tree(
	huffman_tree **tree_array, int count) {

	priority_queue<huffman_tree *, huffman_tree_compare<huffman_tree> > *forest =
		new priority_queue<huffman_tree *, huffman_tree_compare<huffman_tree> >(
			tree_array, count, count);
	huffman_tree *temp1, *temp2, *temp3 = NULL;
	while (forest->size() > 1) {
		temp1 = forest->dequeue();
		temp2 = forest->dequeue();
		temp3 = new huffman_tree(temp1, temp2);
		forest->enqueue(temp3);
		delete temp1;
		delete temp2;
	}
	delete forest;
	return temp3;
}

void huffman_tree::free_nodes() {
	huffman_node* root = this->root();
	std::queue<huffman_node*> q;
	q.push(root);
	while (!q.empty()) {
		huffman_node *node = q.front();
		q.pop();
		if (!node->is_leaf()) {
			internal_node* inode = static_cast<internal_node*>(node);
			if (inode->left() != NULL) {
				q.push(inode->left());
			}
			if (inode->right() != NULL) {
				q.push(inode->right());
			}
		}
		delete node;
	}
}

huffman_file_encoder huffman_tree::to_file_encoder() {
	std::string code;
	encode_map encode_map;
	travel(this->root(), code, encode_map);
	return huffman_file_encoder(encode_map);
}

huffman_file_decoder huffman_tree::to_file_decoder() {
	return huffman_file_decoder(this->root());
}

huffman_tree::~huffman_tree() { }

void huffman_tree::travel(huffman_node* node, std::string& code,
	encode_map& encode_map) {

	if (node->is_leaf()) {
		leaf_node* leaf = static_cast<leaf_node*>(node);
		encode_map.set_code(leaf->val(), code);
	}
	else {
		internal_node* inode = static_cast<internal_node*>(node);
		if (inode->left() != NULL) {
			code.push_back('0');
			travel(inode->left(), code, encode_map);
			code.pop_back();
		}
		if (inode->right() != NULL) {
			code.push_back('1');
			travel(inode->right(), code, encode_map);
			code.pop_back();
		}
	}
}
