#ifndef BYTE_COUNT_TABLE
#define BYTE_COUNT_TABLE

#include "def.h"
#include <iostream>
#include <map>
#include "huffman_tree.h"

class byte_count_table {
public:
	void load_from_file(std::istream &in) {
		byte b;
		while (in.read((char *)&b, sizeof(byte))) {
			this->byte_counts[b]++;
		}
	}

	void save_to_file(std::ostream& out) {
		int table_size = byte_counts.size();
		out.write((char*)&table_size, sizeof(int));

		for (auto& kv : this->byte_counts) {
			out.write((char*)&kv.first, sizeof(kv.first));
			out.write((char*)&kv.second, sizeof(kv.second));
		}
	}

	void read_from_file(std::istream& in) {

		int table_size;
		in.read((char*)&table_size, sizeof(int));

		int i;
		for (i = 0; i < table_size; i++) {
			byte b;
			int count;
			in.read((char*)&b, sizeof(byte));
			in.read((char*)&count, sizeof(int));
			this->byte_counts[b] = count;
		}
	}

	huffman_tree **to_simple_huffman_forest() {

		std::vector<huffman_tree*> tmp_forest;
		size_t i;
		for (i = 0; i < 256; i++) {
			if (this->byte_counts[static_cast<byte>(i)] != 0) {

				huffman_tree *tmp = new huffman_tree(
					static_cast<byte>(i),
					this->byte_counts[static_cast<byte>(i)]);
				tmp_forest.push_back(tmp);
			}
		}
		huffman_tree **forest = new huffman_tree *[tmp_forest.size()];
		this->count = tmp_forest.size();
		for (i = 0; i < tmp_forest.size(); i++) {
			forest[i] = tmp_forest[i];
		}
		return forest;
	}

	int size() const {
		return this->count;
	}

private:
	std::map<byte, int> byte_counts;
	int count;
};

#endif