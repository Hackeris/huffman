#ifndef LEAF_NODE
#define LEAF_NODE

#include "def.h"
#include "huffman_node.h"

class leaf_node : public huffman_node {
private:
	byte it;
	int wgt;
public:
	leaf_node(const byte &val, int freq) {
		this->it = val;
		this->wgt = freq;
	}

	int weight() {
		return this->wgt;
	}

	byte val() {
		return this->it;
	}

	bool is_leaf() {
		return true;
	}
};

#endif