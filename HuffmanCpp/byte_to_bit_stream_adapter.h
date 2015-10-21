#ifndef BYTE_TO_BIT_STREAM_ADAPTER
#define BYTE_TO_BIT_STREAM_ADAPTER

#include "def.h"
#include <iostream>

class byte_to_bit_stream_adapter {
public:
	byte_to_bit_stream_adapter(std::istream& in)
		:in(in) {
		this->offset = 0;
		this->buffer_offset = 0;
	}
	void init(int count_of_bit) {
		this->stream_size_of_bit = count_of_bit;
		in.read((char*)&this->buffer, sizeof(byte));
	}
	bool end() {
		return offset >= stream_size_of_bit;
	}
	int read_bit() {
		if (buffer_offset >= 8) {
			buffer_offset = 0;
			in.read((char*)&this->buffer, sizeof(byte));
		}
		int bit = (buffer & (1 << (7 - buffer_offset))) ? 1 : 0;
		buffer_offset++;
		offset++;
		return bit;
	}
private:
	std::istream& in;
	byte buffer;
	int buffer_offset;
	int offset;
	int stream_size_of_bit;
};

#endif