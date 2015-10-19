#ifndef BYTE_STREAM_TO_BIT_ADAPTER 
#define BYTE_STREAM_TO_BIT_ADAPTER 

#include <iostream>
#include "def.h"

class byte_stream_to_bit_adapter {
public:
	byte_stream_to_bit_adapter(std::istream& in)
		:in(in) {
		start = 0;
		cur = 0;
		this->in.read((char*)&buffer, sizeof(byte));
	}
	int read_bit() {
		if (start == 8) {
			start = 0;
			if (!(this->in.read((char*)&buffer, sizeof(byte)))) {
				return -1;
			}
		}
		int bit = (buffer & (1 << (7 - start))) ? 1 : 0;
		start++;
		cur++;
		return bit;
	}
	int current() const {
		return this->cur;
	}
private:
	std::istream& in;
	byte buffer;
	int start;
	int cur;
};

#endif