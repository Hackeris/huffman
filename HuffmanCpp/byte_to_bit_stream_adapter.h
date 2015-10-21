#ifndef BYTE_TO_BIT_STREAM_ADAPTER
#define BYTE_TO_BIT_STREAM_ADAPTER

#include "def.h"
#include <iostream>

class byte_to_bit_stream_adapter {
public:
	byte_to_bit_stream_adapter(std::istream& in)
		:in(in) {
		this->pos = 0;
		this->start = 0;
	}
	void init(int count_of_bit) {
		this->stream_size_of_bit = count_of_bit;
		in.read((char*)&this->buffer, sizeof(byte));
	}
	bool end() {
		return pos >= stream_size_of_bit;
	}
	int read_bit() {
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
	int stream_size_of_bit;
};

#endif