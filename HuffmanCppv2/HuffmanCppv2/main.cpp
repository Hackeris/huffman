#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <ios>
#include <vector>
#include <queue>

typedef unsigned char byte;

class huffman_node {
public:
	virtual ~huffman_node() { }

	virtual int weight() = 0;

	virtual bool is_leaf() = 0;
};

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

template<typename E, typename Comp>
class heap {
public:
	heap(E *h, int num, int max) {
		this->buffer = h;
		this->n = num;
		this->maxSize = max;
	}

	int size() const {
		return n;
	}

	bool is_leaf(int pos) const {
		return (pos >= n / 2) && (pos < n);
	}

	int left_child(int pos) const {
		return pos * 2 + 1;
	}

	int right_child(int pos) const {
		return pos * 2 + 2;
	}

	int parent(int pos) const {
		return (pos - 1) / 2;
	}

	void build_heap() {
		for (int i = n / 2 - 1; i >= 0; i--) {
			siftdown(i);
		}
	}

	E remove_first() {
		swap(buffer, 0, --n);
		if (n != 0) {
			siftdown(0);
		}
		return buffer[n];
	}

	void insert(const E &it) {
		int curr = n++;
		this->buffer[curr] = it;
		while ((curr != 0) &&
			Comp::smaller(buffer[curr], buffer[parent(curr)])) {
			swap(this->buffer, curr, parent(curr));
			curr = parent(curr);
		}
	}

private:
	void siftdown(int pos) {
		while (!is_leaf(pos)) {
			int j = left_child(pos);
			int rc = right_child(pos);
			if ((rc < n) && Comp::smaller(buffer[rc], buffer[j])) {
				j = rc;
			}
			if (Comp::smaller(buffer[pos], buffer[j])) {
				return;
			}
			swap(buffer, pos, j);
			pos = j;
		}
	}

	void swap(E *h, int p, int q) {
		E tmp = h[p];
		h[p] = h[q];
		h[q] = tmp;
	}

private:
	E *buffer;
	int n;
	int maxSize;
};

template<typename E, typename Comp>
class priority_queue {
public:
	priority_queue(E *heap, int num, int max)
		: heap(heap, num, max) {
		this->heap.build_heap();
	}

	void enqueue(const E &it) {
		this->heap.insert(it);
	}

	E dequeue() {
		return this->heap.remove_first();
	}

	int size() const {
		return this->heap.size();
	}

private:
	heap<E, Comp> heap;
};

template <typename T>
class huffman_tree_compare {
public:
	static bool smaller(const T *l, const T *r) {
		return l->weight() < r->weight();
	}

	static bool larger(const T *l, const T *r) {
		return smaller(r, l);
	}
};

class encode_map {
public:
	void set_code(const byte &key, const std::string &code) {
		this->code_map[key] = code;
	}

	const std::string &get_code(const byte &key) {
		return this->code_map[key];
	}

private:
	std::map<byte, std::string> code_map;
};


class huffman_file_encoder {
public:
	huffman_file_encoder(const encode_map& code_map) {
		this->encode_map = code_map;
		this->code_length = 0;
		this->start = this->end = 0;
	}
	void encode_file(std::istream& in) {
		this->in = &in;
	}
	int write_to_file(std::ostream& out) {
		byte b;
		while (this->in->read((char*)&b, sizeof(byte))) {
			const std::string& code = encode_map.get_code(b);
			write_code_to_buffer(code);
			write_buffer_to_file(out);
		}
		int n = min(end - start, 8);
		if (n == 0) {
			return this->code_length;
		}
		this->code_length += n;
		b = string_to_byte(&heap[start], n);
		out.write((char*)&b, sizeof(b));
		start += n;

		return code_length;
	}
private:
	void write_code_to_buffer(const std::string& code) {
		strcpy(&heap[end], code.c_str());
		end += code.length();
	}
	void write_buffer_to_file(std::ostream& out) {
		while (end - start > 8) {
			byte b = string_to_byte(&heap[start], 8);
			out.write((char*)&b, sizeof(b));
			start += 8;
			code_length += 8;
		}
		reload_buffer();
	}
	void reload_buffer() {
		int i;
		for (i = 0; i < end - start; i++) {
			heap[i] = heap[i + start];
		}
		end -= start;
		start = 0;
	}
	static inline int min(int a, int b) {
		return a < b ? a : b;
	}
	byte string_to_byte(const char* str, int n) {
		byte c = 0;
		int i;
		for (i = 0; i < n; i++) {
			c |= (str[i] == '1' ? (1 << (7 - i)) : 0);
		}
		return c;
	}
private:
	int start, end;
	char heap[256];
	encode_map encode_map;
	int code_length;
	std::istream *in;
};

class byte_to_bit_stream_adapter {
public:
	byte_to_bit_stream_adapter(std::istream& in)
		:in(in) {
		this->pos = 0;
		this->start = 0;
	}
	void init(int count_of_bit) {
		this->stream_size_of_bit = count_of_bit;
		in.read((char*)&this->heap, sizeof(byte));
	}
	bool end() {
		return pos >= stream_size_of_bit;
	}
	int read_bit() {
		if (start >= 8) {
			start = 0;
			in.read((char*)&this->heap, sizeof(byte));
		}
		int bit = (heap & (1 << (7 - start))) ? 1 : 0;
		start++;
		pos++;
		return bit;
	}
private:
	std::istream& in;
	byte heap;
	int start;
	int pos;
	int stream_size_of_bit;
};

class huffman_file_decoder {
public:
	huffman_file_decoder(huffman_node* tree) {
		this->tree_root = tree;
	}
	void decode_file(std::istream& in, int code_length) {
		this->in = new byte_to_bit_stream_adapter(in);
		this->in->init(code_length);
	}
	void write_to_file(std::ostream& out) {
		while (!in->end()) {
			byte b = read_encoded_byte();
			out.write((char*)&b, sizeof(byte));
		}
	}
	~huffman_file_decoder() {
		delete in;
	}
private:
	byte read_encoded_byte() {
		huffman_node* node = this->tree_root;
		while (!node->is_leaf()) {
			internal_node* inode = static_cast<internal_node*>(node);
			int bit = this->in->read_bit();
			if (bit == 0) {
				node = inode->left();
			}
			else {
				node = inode->right();
			}
		}
		leaf_node* leaf = static_cast<leaf_node*>(node);
		return leaf->val();
	}
private:
	huffman_node* tree_root;
	byte_to_bit_stream_adapter *in;
	byte heap;
};

class huffman_tree {
private:
	huffman_node *root_node;
public:
	huffman_tree(const byte &val, int freq) {
		this->root_node = new leaf_node(val, freq);
	}

	huffman_tree(huffman_tree *l, huffman_tree *r) {
		this->root_node = new internal_node(l->root(), r->root());
	}

	huffman_node *root() {
		return this->root_node;
	}

	int weight() const {
		return this->root_node->weight();
	}

	static huffman_tree *biuld_huffman_tree(
		huffman_tree **tree_array, int count) {

		priority_queue<huffman_tree *, huffman_tree_compare<huffman_tree>> *forest =
			new priority_queue<huffman_tree *, huffman_tree_compare<huffman_tree>>(
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

	void free_nodes() {
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

	huffman_file_encoder to_file_encoder() {
		std::string code;
		encode_map encode_map;
		travel(this->root(), code, encode_map);
		return huffman_file_encoder(encode_map);
	}

	huffman_file_decoder to_file_decoder() {
		return huffman_file_decoder(this->root());
	}

	~huffman_tree() { }
private:
	void travel(huffman_node* node, std::string& code, 
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
};


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
		for (i = 0; i < table_size; i++){
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

class huffman_file_compressor{
public:
	void compress(std::istream& in, std::ostream& out) {
		byte_count_table table;
		table.load_from_file(in);
		table.save_to_file(out);
		huffman_tree** forest = table.to_simple_huffman_forest();
		huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

		in.clear();
		in.seekg(0, std::ios::beg);

		huffman_file_encoder encoder = tree->to_file_encoder();
		encoder.encode_file(in);
		int length_offset = out.tellp();
		out.seekp(sizeof(int), std::ios::cur);
		int length = encoder.write_to_file(out);
		out.seekp(length_offset, std::ios::beg);
		out.write((char*)&length, sizeof(int));

		tree->free_nodes();
		delete forest;
		delete tree;
	}
};

class huffman_file_extractor {
public:
	void extract(std::istream& in, std::ostream& out) {
		byte_count_table table;
		table.read_from_file(in);
		huffman_tree** forest = table.to_simple_huffman_forest();
		huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

		int length;
		int offset = in.tellg();
		in.read((char*)&length, sizeof(int));

		huffman_file_decoder decoder = tree->to_file_decoder();
		decoder.decode_file(in, length);
		decoder.write_to_file(out);

		tree->free_nodes();
		delete forest;
		delete tree;
	}
};

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

void test_tree() {

	std::ifstream in("test.txt");
	byte_count_table table;
	table.load_from_file(in);
	in.close();
	huffman_tree** forest = table.to_simple_huffman_forest();
	huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

	tree->free_nodes();
	delete forest;
	delete tree;
}

void test_encode() {

	std::ifstream in("test.txt");
	std::ofstream out("comp.txt");
	byte_count_table table;
	table.load_from_file(in);
	huffman_tree** forest = table.to_simple_huffman_forest();
	huffman_tree* tree = huffman_tree::biuld_huffman_tree(forest, table.size());

	in.clear();
	in.seekg(0, std::ios::beg);

	huffman_file_encoder encoder = tree->to_file_encoder();
	encoder.encode_file(in);
	int length = encoder.write_to_file(out);

	in.close();
	out.close();

	huffman_file_decoder decoder = tree->to_file_decoder();
	std::ifstream cmp("comp.txt", std::ios::binary);
	std::ofstream ex("ext.txt", std::ios::binary);
	decoder.decode_file(cmp, length);
	decoder.write_to_file(ex);

	tree->free_nodes();
	delete forest;
	delete tree;
}

void test_compress() {
	std::ifstream in("test.txt", std::ios::binary);
	std::ofstream out("compressed.txt", std::ios::binary);

	huffman_file_compressor compressor;
	compressor.compress(in, out);

	in.close();
	out.close();
}

void test_extract() {
	std::ifstream in("compressed.txt", std::ios::binary);
	std::ofstream out("extracted.txt", std::ios::binary);

	huffman_file_extractor extractor;
	extractor.extract(in, out);

	in.close();
	out.close();
}

void test_compress_and_extract() {

	test_compress();
	test_extract();
}

int main() {

	test_tree();
	test_encode();
	test_compress_and_extract();

	_CrtDumpMemoryLeaks();

	return 0;
}
