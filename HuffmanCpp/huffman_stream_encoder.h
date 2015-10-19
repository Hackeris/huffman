#ifndef HUFFMAN_STREAM_ENCODER
#define HUFFMAN_STREAM_ENCODER

#include <iostream>
#include "def.h"
#include "huffman_char_code_map.h"

class huffman_stream_encoder {
public:
	huffman_stream_encoder(
		huffman_char_code_map& cmap)
		:cmap(cmap) {
		start = end = 0;
		code_length = 0;
		buffer = new byte[260];
	}
	void encode(std::istream& in) {
		this->in = &in;
	}
	int output_to_stream(std::ostream& out) {
		byte c;
		while (this->read_encoded_byte(c)) {
			out.write((char*)&c, sizeof(byte));
		}
		return 0;
	}
	int get_code_length_of_bits() {
		return this->code_length;
	}
	~huffman_stream_encoder() {
		delete buffer;
		delete &cmap;
	}
private:
	int read_encoded_byte(byte& ch) {

		std::istream& in = *this->in;

		char bits[9];
		int n = read_bits(in, bits);
		bits[n] = '\0';
		code_length += n;
		if (n == 0) {
			return n;
		}
		else {
			ch = string_to_byte(bits, n);
			return n;
		}
	}
	int read_bits(std::istream &in, char* str) {
		if (start >= 8) {
			int i;
			for (i = 0; i < end - start; i++) {
				buffer[i] = buffer[i + start];
			}
			end -= start;
			start = 0;
		}
		while (end - start < 8) {
			byte c;
			if (in.read((char*)&c, sizeof(byte))) {
				strcpy((char*)&buffer[end], cmap[c].c_str());
				end += cmap[c].length();
			}
			else {
				break;
			}
		}
		int n = min(end - start, 8);
		memcpy(str, &buffer[start], n);
		start += n;
		return n;
	}
	static inline int min(int x, int y) {
		return x > y ? y : x;
	}
	byte string_to_byte(char* str, int n) {
		byte c = 0;
		int i;
		for (i = 0; i < n; i++) {
			c |= (str[i] == '1' ? (1 << (7 - i)) : 0);
		}
		return c;
	}
private:
	unsigned char *buffer;
	int start, end;
	int code_length;
	huffman_char_code_map& cmap;
	std::istream* in;
};


#endif