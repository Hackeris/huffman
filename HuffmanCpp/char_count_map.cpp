#include "char_count_map.h"

char_count_map::char_count_map() {
	memset(count_table, 0, sizeof(int) * 256);
}

void char_count_map::load(std::istream& in) {
	byte c;
	while (in.read((char*)&c, sizeof(byte))) {
		count_table[c] ++;
	}
}

void char_count_map::from_stream(std::istream& in) {
	int n;
	in.read((char*)&n, sizeof(int));
	int i;
	for (i = 0; i < n; i++) {
		byte b;
		in.read((char*)&b, sizeof(byte));
		in.read((char*)&this->count_table[b],
			sizeof(this->count_table[b]));
	}
}

void char_count_map::to_stream(std::ostream& out) {
	int n = 0;
	int i;
	for (i = 0; i < TABLE_SIZE; i++) {
		if (this->count_table[i] != 0) {
			n++;
		}
	}
	out.write((char*)&n, sizeof(int));
	for (i = 0; i < TABLE_SIZE; i++) {
		if (this->count_table[i] != 0) {
			out.write((char*)&i, sizeof(byte));
			out.write((char*)&this->count_table[i], sizeof(this->count_table[i]));
		}
	}
}

const int &char_count_map::operator[](byte c) const {
	return count_table[c];
}

int char_count_map::length() const {
	return TABLE_SIZE;
}