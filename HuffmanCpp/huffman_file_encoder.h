#ifndef HUFFMAN_FILE_ENCODER
#define HUFFMAN_FILE_ENCODER

#include "def.h"
#include <iostream>
#include "encode_map.h"

class huffman_file_encoder {
public:
	huffman_file_encoder(const encode_map& code_map) {
		this->encode_map = code_map;
		this->code_length = 0;
		this->start = this->end = 0;
	}
	void encode_file(std::istream& in) {
		this->in = &in;
	}
	int write_to_file(std::ostream& out) {
		byte b;
		while (this->in->read((char*)&b, sizeof(byte))) {
			const std::string& code = encode_map.get_code(b);
			write_code_to_buffer(code);
			write_buffer_to_file(out);
		}
		int n = min(end - start, 8);
		if (n == 0) {
			return this->code_length;
		}
		this->code_length += n;
		b = string_to_byte(&buffer[start], n);
		out.write((char*)&b, sizeof(b));
		start += n;

		return code_length;
	}
private:
	void write_code_to_buffer(const std::string& code) {
		strcpy(&buffer[end], code.c_str());
		end += code.length();
	}
	void write_buffer_to_file(std::ostream& out) {
		while (end - start > 8) {
			byte b = string_to_byte(&buffer[start], 8);
			out.write((char*)&b, sizeof(b));
			start += 8;
			code_length += 8;
		}
		reload_buffer();
	}
	void reload_buffer() {
		int i;
		for (i = 0; i < end - start; i++) {
			buffer[i] = buffer[i + start];
		}
		end -= start;
		start = 0;
	}
	static inline int min(int a, int b) {
		return a < b ? a : b;
	}
	byte string_to_byte(const char* str, int n) {
		byte c = 0;
		int i;
		for (i = 0; i < n; i++) {
			c |= (str[i] == '1' ? (1 << (7 - i)) : 0);
		}
		return c;
	}
private:
	int start, end;
	char buffer[256];
	encode_map encode_map;
	int code_length;
	std::istream *in;
};

#endif