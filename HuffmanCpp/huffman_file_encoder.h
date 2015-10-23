#ifndef HUFFMAN_FILE_ENCODER
#define HUFFMAN_FILE_ENCODER

#include "def.h"
#include <iostream>
#include "encode_map.h"

class huffman_file_encoder {
public:
	huffman_file_encoder(const encode_map& code_map);

	void encode_file(std::istream& in);

	int write_to_file(std::ostream& out);

private:
	void write_code_to_buffer(const std::string& code);

	void write_buffer_to_file(std::ostream& out);

	void reload_buffer();

	static inline int min(int a, int b);

	byte string_to_byte(const char* str, int n);
private:
	int start, end;
	char buffer[256];
	encode_map code_map;
	int code_length;
	std::istream *in;
};

#endif