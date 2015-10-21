#ifndef HEAP 
#define HEAP 

template<typename E, typename Comp>
class heap {
public:
	heap(E *h, int num, int max) {
		this->buffer = h;
		this->n = num;
		this->max_size = max;
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
	int max_size;
};

#endif