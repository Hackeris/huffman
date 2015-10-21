#ifndef HUFFMAN_CHAR_CODE_MAP
#define HUFFMAN_CHAR_CODE_MAP

#include <map>
#include "def.h"

class huffman_char_code_map {
public:
	std::map<unsigned char, std::string> ccmap;
public:
	std::string &operator[](unsigned char c) {
		return ccmap[c];
	}
};

#endif