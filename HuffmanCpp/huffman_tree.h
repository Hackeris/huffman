#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE 

#include "huffman_stream_decoder.h"
#include "huffman_stream_encoder.h"

class huffman_tree {
public:
	huffman_tree() {
	}
	huffman_stream_encoder to_byte_encoder() {
		huffman_char_code_map &cmap = *this->to_char_code_map();
		return huffman_stream_encoder(cmap);
	}
	huffman_stream_decoder to_byte_decoder() {
		return huffman_stream_decoder(this->root);
	}
	void build_from(const char_count_map& cmap) {
		init_data_from_map(cmap);
		build_huffman_tree();
	}
	void free_huffman_tree_node(huffman_node* node) {
		if (node->left != NULL) {
			free_huffman_tree_node(node->left);
		}
		if (node->right != NULL) {
			free_huffman_tree_node(node->right);
		}
		delete node;
	}
	virtual ~huffman_tree() {
		free_huffman_tree_node(this->root);
		delete[]chars;
	}
private:
	static int get_avaliable_count(const char_count_map& cmap) {
		int len = cmap.length();
		int count = 0;
		for (int i = 0; i < len; i++) {
			if (cmap[i] > 0) {
				count++;
			}
		}
		return count;
	}
	static int cmp_node(const void* l, const void* r) {
		return (*(const huffman_node**)l)->count - (*(const huffman_node**)r)->count;
	}
	void init_data_from_map(const char_count_map& cmap) {
		this->avaliable_count = get_avaliable_count(cmap);
		chars = new huffman_node*[avaliable_count];

		int i = 0, j = 0;
		for (i = 0; i < cmap.length(); i++) {
			if (cmap[i] > 0) {
				chars[j] = new huffman_node;
				chars[j]->c = i;
				chars[j]->count = cmap[i];
				j++;
			}
		}
	}
	//return value:
	//	are there more than 2 nodes in the chars?
	bool resort_huffman_nodes(int start, int length) {

		if (start < length - 2) {
			if (this->chars[start]->count > this->chars[start + 1]->count
				&& this->chars[start]->count > this->chars[start + 2]->count) {
				huffman_node *tmp = this->chars[start];
				this->chars[start] = this->chars[start + 2];
				this->chars[start + 2] = tmp;
			}
			return true;
		}
		else if (start == length - 2) {
			return true;
		}
		return false;
	}
	//return value:
	//	are there more than 2 nodes?
	bool get_minimal_2nodes(huffman_node**nodes, int start, int length) {

		if (resort_huffman_nodes(start, length)) {
			nodes[0] = this->chars[start];
			nodes[1] = this->chars[start + 1];
			return true;
		}
		else {
			nodes[0] = this->chars[start];
			nodes[1] = NULL;
			return false;
		}
	}
	huffman_node* merge_2nodes(huffman_node* left, huffman_node* right) {
		huffman_node* root = new huffman_node(left, right);
		return root;
	}
	void build_huffman_tree() {
		std::qsort(this->chars,
			this->avaliable_count,
			sizeof(huffman_node*),
			cmp_node);

		int start = 0;
		huffman_node* nodes[2];
		while (get_minimal_2nodes(nodes, start, this->avaliable_count)) {
			this->root = merge_2nodes(nodes[0], nodes[1]);
			start++;
			this->chars[start] = this->root;
		}
		this->root = nodes[0];
	}
	void travel_huffman_tree(std::string& huffman_code, huffman_node* node, huffman_char_code_map& ccmap) {

		if (node->left == NULL && node->right == NULL) {
			ccmap[node->c] = huffman_code;
		}
		if (node->left != NULL) {
			huffman_code.push_back('0');
			travel_huffman_tree(huffman_code, node->left, ccmap);
			huffman_code.pop_back();
		}
		if (node->right != NULL) {
			huffman_code.push_back('1');
			travel_huffman_tree(huffman_code, node->right, ccmap);
			huffman_code.pop_back();
		}
	}
	huffman_char_code_map *to_char_code_map() {
		huffman_char_code_map *ccmap = new huffman_char_code_map;
		std::string huffman_code;
		travel_huffman_tree(huffman_code, this->root, *ccmap);
		return ccmap;
	}
private:
	int avaliable_count;
	huffman_node** chars;
	huffman_node* root;
};

#endif