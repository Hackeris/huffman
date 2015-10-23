#include "byte_to_bit_adapter.h"

byte_to_bit_adapter::byte_to_bit_adapter(std::istream& in)
	:in(in) {
	this->offset = 0;
	this->buffer_offset = 0;
}

void byte_to_bit_adapter::init(int count_of_bit) {
	this->stream_size_of_bit = count_of_bit;
	in.read((char*)&this->buffer, sizeof(byte));
}

bool byte_to_bit_adapter::end() {
	return offset >= stream_size_of_bit;
}

int byte_to_bit_adapter::read_bit() {
	if (buffer_offset >= 8) {
		buffer_offset = 0;
		in.read((char*)&this->buffer, sizeof(byte));
	}
	int bit = (buffer & (1 << (7 - buffer_offset))) ? 1 : 0;
	buffer_offset++;
	offset++;
	return bit;
}