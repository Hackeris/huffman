#include "encode_map.h"
#include "huffman_file_encoder.h"

huffman_file_encoder::huffman_file_encoder(const encode_map& code_map) {
	this->code_map = code_map;
	this->code_length = 0;
	this->start = this->end = 0;
}

void huffman_file_encoder::encode_file(std::istream& in) {
	this->in = &in;
}

//	将压缩后的编码写入文件
//	每次读取需要压缩的内容，通过编码映射得到编码，写入缓冲区
//	缓冲区到达一定数量的数据，则写入文件
long long huffman_file_encoder::write_to_file(std::ostream& out) {
	byte b;
	while (this->in->read((char*)&b, sizeof(byte))) {
		const std::string& code = code_map.get_code(b);
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

void huffman_file_encoder::write_code_to_buffer(const std::string& code) {
	strcpy(&buffer[end], code.c_str());
	end += code.length();
}

//	写缓冲区中的数据到文件
void huffman_file_encoder::write_buffer_to_file(std::ostream& out) {
	while (end - start > 8) {
		byte b = string_to_byte(&buffer[start], 8);
		out.write((char*)&b, sizeof(b));
		start += 8;
		code_length += 8;
	}
	reload_buffer();
}

//	调整缓冲区中的数据
void huffman_file_encoder::reload_buffer() {
	int i;
	for (i = 0; i < end - start; i++) {
		buffer[i] = buffer[i + start];
	}
	end -= start;
	start = 0;
}

inline int huffman_file_encoder::min(int a, int b) {
	return a < b ? a : b;
}

//	将编码串转化为字节
byte huffman_file_encoder::string_to_byte(const char* str, int n) {
	byte c = 0;
	int i;
	for (i = 0; i < n; i++) {
		c |= (str[i] == '1' ? (1 << (7 - i)) : 0);
	}
	return c;
}
