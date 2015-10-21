#ifndef HUFFMAN_NODE 
#define HUFFMAN_NODE 

class huffman_node {
public:
	virtual ~huffman_node() { }

	virtual int weight() = 0;

	virtual bool is_leaf() = 0;
};

#endif