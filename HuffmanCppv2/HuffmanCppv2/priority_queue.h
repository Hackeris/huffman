#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include "heap.h"

template<typename E, typename Comp>
class priority_queue {
public:
	priority_queue(E *buffer, int num, int max)
		: hp(buffer, num, max) {
		this->hp.build_heap();
	}

	void enqueue(const E &it) {
		this->hp.insert(it);
	}

	E dequeue() {
		return this->hp.remove_first();
	}

	int size() const {
		return this->hp.size();
	}

private:
	heap<E, Comp> hp;
};

#endif