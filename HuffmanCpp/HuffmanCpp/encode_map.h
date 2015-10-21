#ifndef ENCODE_MAP
#define ENCODE_MAP

#include <string>
#include <map>
#include "def.h"

class encode_map {
public:
	void set_code(const byte &key, const std::string &code) {
		this->code_map[key] = code;
	}

	const std::string &get_code(const byte &key) {
		return this->code_map[key];
	}

private:
	std::map<byte, std::string> code_map;
};

#endif