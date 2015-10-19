#include "byte_stream_to_bit_adapter.h"

byte_stream_to_bit_adapter::byte_stream_to_bit_adapter(std::istream& in)
	:in(in) {
	start = 0;
	cur = 0;
	this->in.read((char*)&buffer, sizeof(byte));
}

int byte_stream_to_bit_adapter::read_bit() {
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

int byte_stream_to_bit_adapter::current() const {
	return this->cur;
}
