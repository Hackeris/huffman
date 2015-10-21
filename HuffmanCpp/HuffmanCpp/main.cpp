#include <iostream>

template <typename E>
class Compare {
public:
	static bool smaller(const E& l, const E& r) {
		return l < r;
	}
};

template <typename E, typename Comp>
class Heap {
public:
	Heap(E* h, int num, int max) {
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
	void biuldHeap() {
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
private:
	void siftdown(int pos) {
		while (!isLeaf(pos)) {
			int j = leftChild(pos);
			int rc = rightChild(pos);
			if ((rc < n) && Comp::smaller(heap[rc], heap[j])) {
				j = rc;
			}
			if (Comp::smaller(heap[pos], heap[j])){
				return;
			}
			swap(heap, pos, j);
			pos = j;
		}
	}
	void swap(E* h, int p, int q) {
		E tmp = h[p];
		h[p] = h[q];
		h[q] = tmp;
	}
	void insert(const E& it) {
		int curr = n++;
		this->heap[curr] = it;
		while ((curr != 0) &&
			Comp::smaller(heap[curr], heap[parent(curr)])) {
			swap(this->heap, curr, parent(curr));
			curr = parent(curr);
		}
	}
private:
	E* heap;
	int n;
	int maxSize;
};



int main() {

	int buffer[] = { 4,8,2,1,5,9,3,7,0,6 };
	int n = 10;
	Heap<int, Compare<int>> heap(buffer, n, n);
	heap.biuldHeap();
	while (heap.size() > 0) {
		std::cout << heap.removeFirst() << std::endl;
	}

	return 0;
}
