#ifndef BYTE_TO_BIT_STREAM_ADAPTER
#define BYTE_TO_BIT_STREAM_ADAPTER

#include "def.h"
#include <iostream>

class byte_to_bit_adapter {
public:
	byte_to_bit_adapter(std::istream& in);

	void init(int count_of_bit);

	bool end();

	int read_bit();

private:
	std::istream& in;
	byte buffer;
	int buffer_offset;
	int offset;
	int stream_size_of_bit;
};

#endif