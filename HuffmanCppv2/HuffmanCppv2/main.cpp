#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <ios>
#include <vector>
#include <queue>

typedef unsigned char byte;

class HuffmanNode {
public:
	virtual ~HuffmanNode() { }

	virtual int weight() = 0;

	virtual bool isLeaf() = 0;
};

class LeafNode : public HuffmanNode {
private:
	byte it;
	int wgt;
public:
	LeafNode(const byte &val, int freq) {
		this->it = val;
		this->wgt = freq;
	}

	int weight() {
		return this->wgt;
	}

	byte val() {
		return this->it;
	}

	bool isLeaf() {
		return true;
	}
};

class IntlNode : public HuffmanNode {
private:
	HuffmanNode *lc;
	HuffmanNode *rc;
	int wgt;
public:
	IntlNode(HuffmanNode *l, HuffmanNode *r) {
		this->lc = l;
		this->rc = r;
		this->wgt = l->weight() + r->weight();
	}

	int weight() {
		return this->wgt;
	}

	bool isLeaf() {
		return false;
	}

	HuffmanNode *left() const {
		return this->lc;
	}

	HuffmanNode *right() const {
		return this->rc;
	}

	void setLeft(HuffmanNode *l) {
		this->lc = l;
	}

	void setRight(HuffmanNode *r) {
		this->rc = r;
	}
};

template<typename E, typename Comp>
class Heap {
public:
	Heap(E *h, int num, int max) {
		this->heap = h;
		this->n = num;
		this->maxSize = max;
	}

	int size() const {
		return n;
	}

	bool isLeaf(int pos) const {
		return (pos >= n / 2) && (pos < n);
	}

	int leftChild(int pos) const {
		return pos * 2 + 1;
	}

	int rightChild(int pos) const {
		return pos * 2 + 2;
	}

	int parent(int pos) const {
		return (pos - 1) / 2;
	}

	void buildHeap() {
		for (int i = n / 2 - 1; i >= 0; i--) {
			siftdown(i);
		}
	}

	E removeFirst() {
		swap(heap, 0, --n);
		if (n != 0) {
			siftdown(0);
		}
		return heap[n];
	}

	void insert(const E &it) {
		int curr = n++;
		this->heap[curr] = it;
		while ((curr != 0) &&
			Comp::smaller(heap[curr], heap[parent(curr)])) {
			swap(this->heap, curr, parent(curr));
			curr = parent(curr);
		}
	}

private:
	void siftdown(int pos) {
		while (!isLeaf(pos)) {
			int j = leftChild(pos);
			int rc = rightChild(pos);
			if ((rc < n) && Comp::smaller(heap[rc], heap[j])) {
				j = rc;
			}
			if (Comp::smaller(heap[pos], heap[j])) {
				return;
			}
			swap(heap, pos, j);
			pos = j;
		}
	}

	void swap(E *h, int p, int q) {
		E tmp = h[p];
		h[p] = h[q];
		h[q] = tmp;
	}

private:
	E *heap;
	int n;
	int maxSize;
};

template<typename E, typename Comp>
class PriorityQueue {
public:
	PriorityQueue(E *buffer, int num, int max)
		: heap(buffer, num, max) {
		this->heap.buildHeap();
	}

	void enqueue(const E &it) {
		this->heap.insert(it);
	}

	E dequeue() {
		return this->heap.removeFirst();
	}

	int size() const {
		return this->heap.size();
	}

private:
	Heap<E, Comp> heap;
};

template <typename T>
class HuffmanTreeCompare {
public:
	static bool smaller(const T *l, const T *r) {
		return l->weight() < r->weight();
	}

	static bool larger(const T *l, const T *r) {
		return smaller(r, l);
	}
};

class EncodeMap {
public:
	void setCode(const byte &key, const std::string &code) {
		this->codeMap[key] = code;
	}

	const std::string &getCode(const byte &key) {
		return this->codeMap[key];
	}

private:
	std::map<byte, std::string> codeMap;
};


class HuffmanFileEncoder {
public:
	HuffmanFileEncoder(const EncodeMap& codeMap) {
		this->encodeMap = codeMap;
		this->codeLength = 0;
		this->start = this->end = 0;
	}
	void encodeFile(std::istream& in) {
		this->in = &in;
	}
	int writeToFile(std::ostream& out) {
		byte b;
		while (this->in->read((char*)&b, sizeof(byte))) {
			const std::string& code = encodeMap.getCode(b);
			writeCodeToBuffer(code);
			writeBufferToFile(out);
		}
		int n = min(end - start, 8);
		if (n == 0) {
			return this->codeLength;
		}
		this->codeLength += n;
		b = stringToByte(&buffer[start], n);
		out.write((char*)&b, sizeof(b));
		start += n;

		return codeLength;
	}
private:
	void writeCodeToBuffer(const std::string& code) {
		std::cout << code;
		strcpy(&buffer[end], code.c_str());
		end += code.length();
	}
	void writeBufferToFile(std::ostream& out) {
		while (end - start > 8) {
			byte b = stringToByte(&buffer[start], 8);
			out.write((char*)&b, sizeof(b));
			start += 8;
			codeLength += 8;
		}
		reloadBuffer();
	}
	void reloadBuffer() {
		int i;
		for (i = 0; i < end - start; i++) {
			buffer[i] = buffer[i + start];
		}
		end -= start;
		start = 0;
	}
	static inline int min(int a, int b) {
		return a < b ? a : b;
	}
	byte stringToByte(const char* str, int n) {
		byte c = 0;
		int i;
		for (i = 0; i < n; i++) {
			c |= (str[i] == '1' ? (1 << (7 - i)) : 0);
		}
		return c;
	}
private:
	int start, end;
	char buffer[256];
	EncodeMap encodeMap;
	int codeLength;
	std::istream *in;
};

class ByteToBitStreamAdapter {
public:
	ByteToBitStreamAdapter(std::istream& in)
		:in(in) {
		this->pos = 0;
		this->start = 0;
	}
	void init(int countOfBit) {
		this->streamSizeOfBit = countOfBit;
		in.read((char*)&this->buffer, sizeof(byte));
	}
	bool end() {
		return pos >= streamSizeOfBit;
	}
	int readBit() {
		if (start >= 8) {
			start = 0;
			in.read((char*)&this->buffer, sizeof(byte));
		}
		int bit = (buffer & (1 << (7 - start))) ? 1 : 0;
		start++;
		pos++;
		return bit;
	}
private:
	std::istream& in;
	byte buffer;
	int start;
	int pos;
	int streamSizeOfBit;
};

class HuffmanFileDecoder {
public:
	HuffmanFileDecoder(HuffmanNode* tree) {
		this->treeRoot = tree;
	}
	void decodeFile(std::istream& in, int codeLength) {
		this->in = new ByteToBitStreamAdapter(in);
		this->in->init(codeLength);
	}
	void writeToFile(std::ostream& out) {
		while (!in->end()) {
			byte b = readEncodedByte();
			out.write((char*)&b, sizeof(byte));
		}
	}
	~HuffmanFileDecoder() {
		delete in;
	}
private:
	byte readEncodedByte() {
		HuffmanNode* node = this->treeRoot;
		while (!node->isLeaf()) {
			IntlNode* inode = static_cast<IntlNode*>(node);
			int bit = this->in->readBit();
			if (bit == 0) {
				node = inode->left();
			}
			else {
				node = inode->right();
			}
		}
		LeafNode* leaf = static_cast<LeafNode*>(node);
		return leaf->val();
	}
private:
	HuffmanNode* treeRoot;
	ByteToBitStreamAdapter *in;
	byte buffer;
};

class HuffmanTree {
private:
	HuffmanNode *Root;
public:
	HuffmanTree(const byte &val, int freq) {
		this->Root = new LeafNode(val, freq);
	}

	HuffmanTree(HuffmanTree *l, HuffmanTree *r) {
		this->Root = new IntlNode(l->root(), r->root());
	}

	HuffmanNode *root() {
		return this->Root;
	}

	int weight() const {
		return this->Root->weight();
	}

	static HuffmanTree *biuldHuffmanTree(
		HuffmanTree **treeArray, int count) {

		PriorityQueue<HuffmanTree *, HuffmanTreeCompare<HuffmanTree>> *forest =
			new PriorityQueue<HuffmanTree *, HuffmanTreeCompare<HuffmanTree>>(
				treeArray, count, count);
		HuffmanTree *temp1, *temp2, *temp3 = NULL;
		while (forest->size() > 1) {
			temp1 = forest->dequeue();
			temp2 = forest->dequeue();
			temp3 = new HuffmanTree(temp1, temp2);
			forest->enqueue(temp3);
			delete temp1;
			delete temp2;
		}
		delete forest;
		return temp3;
	}

	void freeNodes() {
		HuffmanNode* root = this->root();
		std::queue<HuffmanNode*> q;
		q.push(root);
		while (!q.empty()) {
			HuffmanNode *node = q.front();
			q.pop();
			if (!node->isLeaf()) {
				IntlNode* inode = static_cast<IntlNode*>(node);
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

	HuffmanFileEncoder toFileEncoder() {
		std::string code;
		EncodeMap encodeMap;
		travel(this->root(), code, encodeMap);
		return HuffmanFileEncoder(encodeMap);
	}

	HuffmanFileDecoder toFileDecoder() {
		return HuffmanFileDecoder(this->root());
	}

	~HuffmanTree() { }
private:
	void travel(HuffmanNode* node, std::string& code, 
		EncodeMap& encodeMap) {

		if (node->isLeaf()) {
			LeafNode* leaf = static_cast<LeafNode*>(node);
			encodeMap.setCode(leaf->val(), code);
		}
		else {
			IntlNode* inode = static_cast<IntlNode*>(node);
			if (inode->left() != NULL) {
				code.push_back('0');
				travel(inode->left(), code, encodeMap);
				code.pop_back();
			}
			if (inode->right() != NULL) {
				code.push_back('1');
				travel(inode->right(), code, encodeMap);
				code.pop_back();
			}
		}
	}
};


class ByteCountTable {
public:
	void loadFromFile(std::istream &in) {
		byte b;
		while (in.read((char *)&b, sizeof(byte))) {
			this->byteCounts[b]++;
		}
	}

	void saveToFile(std::ostream& out) {
		int tableSize = byteCounts.size();
		out.write((char*)&tableSize, sizeof(int));

		for (auto& kv : this->byteCounts) {
			out.write((char*)&kv.first, sizeof(kv.first));
			out.write((char*)&kv.second, sizeof(kv.second));
		}
	}

	void readFromFile(std::istream& in) {

		int tableSize;
		in.read((char*)&tableSize, sizeof(int));

		int i;
		for (i = 0; i < tableSize; i++){
			byte b;
			int count;
			in.read((char*)&b, sizeof(byte));
			in.read((char*)&count, sizeof(int));
			this->byteCounts[b] = count;
		}
	}

	HuffmanTree **toSimpleHuffmanForest() {

		std::vector<HuffmanTree*> tmpForest;
		size_t i;
		for (i = 0; i < 256; i++) {
			if (this->byteCounts[static_cast<byte>(i)] != 0) {

				HuffmanTree *tmp = new HuffmanTree(
					static_cast<byte>(i),
					this->byteCounts[static_cast<byte>(i)]);
				tmpForest.push_back(tmp);
			}
		}
		HuffmanTree **forest = new HuffmanTree *[tmpForest.size()];
		this->count = tmpForest.size();
		for (i = 0; i < tmpForest.size(); i++) {
			forest[i] = tmpForest[i];
		}
		return forest;
	}

	int size() const {
		return this->count;
	}

private:
	std::map<byte, int> byteCounts;
	int count;
};

class HuffmanFileCompressor{
public:
	void compress(std::istream& in, std::ostream& out) {
		ByteCountTable table;
		table.loadFromFile(in);
		table.saveToFile(out);
		HuffmanTree** forest = table.toSimpleHuffmanForest();
		HuffmanTree* tree = HuffmanTree::biuldHuffmanTree(forest, table.size());

		in.clear();
		in.seekg(0, std::ios::beg);

		HuffmanFileEncoder encoder = tree->toFileEncoder();
		encoder.encodeFile(in);
		out.seekp(sizeof(int), std::ios::cur);
		int lengthOffset = out.tellp();
		int length = encoder.writeToFile(out);
		out.seekp(lengthOffset, std::ios::beg);
		out.write((char*)&length, sizeof(int));

		tree->freeNodes();
		delete forest;
		delete tree;
	}
};

class HuffmanFileExtractor {
public:
	void extract(std::istream& in, std::ostream& out) {
		ByteCountTable table;
		table.readFromFile(in);
		HuffmanTree** forest = table.toSimpleHuffmanForest();
		HuffmanTree* tree = HuffmanTree::biuldHuffmanTree(forest, table.size());

		int length;
		in.read((char*)&length, sizeof(int));

		HuffmanFileDecoder decoder = tree->toFileDecoder();
		decoder.decodeFile(in, length);
		decoder.writeToFile(out);

		tree->freeNodes();
		delete forest;
		delete tree;
	}
};

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

void testTree() {

	std::ifstream in("test.txt");
	ByteCountTable table;
	table.loadFromFile(in);
	in.close();
	HuffmanTree** forest = table.toSimpleHuffmanForest();
	HuffmanTree* tree = HuffmanTree::biuldHuffmanTree(forest, table.size());

	tree->freeNodes();
	delete forest;
	delete tree;
}

void testEncode() {

	std::ifstream in("test.txt");
	std::ofstream out("comp.txt");
	ByteCountTable table;
	table.loadFromFile(in);
	HuffmanTree** forest = table.toSimpleHuffmanForest();
	HuffmanTree* tree = HuffmanTree::biuldHuffmanTree(forest, table.size());

	in.clear();
	in.seekg(0, std::ios::beg);

	HuffmanFileEncoder encoder = tree->toFileEncoder();
	encoder.encodeFile(in);
	int length = encoder.writeToFile(out);

	in.close();
	out.close();

	HuffmanFileDecoder decoder = tree->toFileDecoder();
	std::ifstream cmp("comp.txt", std::ios::binary);
	std::ofstream ex("ext.txt", std::ios::binary);
	decoder.decodeFile(cmp, length);
	decoder.writeToFile(ex);

	tree->freeNodes();
	delete forest;
	delete tree;
}

int main() {

	testTree();
	testEncode();

	_CrtDumpMemoryLeaks();

	return 0;
}
