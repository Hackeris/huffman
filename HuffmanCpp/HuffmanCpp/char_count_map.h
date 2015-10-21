#ifndef CHAR_COUNT_MAP
#define CHAR_COUNT_MAP

#include <string>
#include "def.h"

class char_count_map {
public:
	char_count_map();
	void load(std::istream& in);
	void from_stream(std::istream& in);
	void to_stream(std::ostream& out);
	const int &operator[](byte c) const;
	int length() const;
private:
	const static int TABLE_SIZE = 256;
	int count_table[TABLE_SIZE];
};

#endif