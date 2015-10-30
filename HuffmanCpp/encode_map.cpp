#include "encode_map.h"

void encode_map::set_code(const byte &key, const std::string &code) {
	this->code_map[key] = code;
}

const std::string &encode_map::get_code(const byte &key) {
	return this->code_map[key];
}