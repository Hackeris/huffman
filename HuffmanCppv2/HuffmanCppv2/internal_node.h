#ifndef INTERNAL_NODE 
#define INTERNAL_NODE 

#include "def.h"
#include "huffman_node.h"

class internal_node : public huffman_node {
private:
	huffman_node *lc;
	huffman_node *rc;
	int wgt;
public:
	internal_node(huffman_node *l, huffman_node *r) {
		this->lc = l;
		this->rc = r;
		this->wgt = l->weight() + r->weight();
	}

	int weight() {
		return this->wgt;
	}

	bool is_leaf() {
		return false;
	}

	huffman_node *left() const {
		return this->lc;
	}

	huffman_node *right() const {
		return this->rc;
	}

	void set_left(huffman_node *l) {
		this->lc = l;
	}

	void set_right(huffman_node *r) {
		this->rc = r;
	}
};

#endif