#ifndef HUFFMAN_STREAM_ENCODER
#define HUFFMAN_STREAM_ENCODER

#include <iostream>
#include "def.h"
#include "huffman_char_code_map.h"

class huffman_stream_encoder {
public:
	huffman_stream_encoder(huffman_char_code_map& cmap);
	void encode(std::istream& in);
	int output_to_stream(std::ostream& out);
	int get_code_length_of_bits();
	~huffman_stream_encoder();
private:
	int read_encoded_byte(byte& ch);
	int read_bits(std::istream &in, char* str);
	static inline int min(int x, int y);
	byte string_to_byte(char* str, int n);
private:
	unsigned char *buffer;
	int start, end;
	int code_length;
	huffman_char_code_map& cmap;
	std::istream* in;
};


#endif