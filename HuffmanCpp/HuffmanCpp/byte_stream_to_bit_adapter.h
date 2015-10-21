#ifndef BYTE_STREAM_TO_BIT_ADAPTER 
#define BYTE_STREAM_TO_BIT_ADAPTER 

#include <iostream>
#include "def.h"

class byte_stream_to_bit_adapter {
public:
	byte_stream_to_bit_adapter(std::istream& in);
	int read_bit();
	int current() const;
private:
	std::istream& in;
	byte buffer;
	int start;
	int cur;
};

#endif