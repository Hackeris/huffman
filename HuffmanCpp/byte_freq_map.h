#ifndef BYTE_COUNT_TABLE
#define BYTE_COUNT_TABLE

#include "def.h"
#include <iostream>
#include <map>
#include "huffman_tree.h"

class byte_freq_map {
public:
	void load_from_file(std::istream &in);

	void save_to_file(std::ostream& out);

	void read_from_file(std::istream& in);

	huffman_tree **to_simple_huffman_forest();

	int size() const;

private:
	std::map<byte, int> byte_counts;
	int count;
};

#endif