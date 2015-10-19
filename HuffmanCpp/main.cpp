#include <fstream>
#include <iostream>
#include "huffman_file_decoder.h"
#include "huffman_file_encoder.h"

void test_encode() {
	std::ifstream file("test.txt", std::ios::binary);
	std::ofstream comp("comp.txt", std::ios::binary);
	huffman_file_encoder encoder(file);
	encoder.output_to_file(comp);
	file.close();
	comp.close();
}

void test_decode() {
	std::ifstream comp("comp.txt", std::ios::binary);
	std::ofstream uncom("uncom.txt", std::ios::binary);
	huffman_file_decoder decoder(comp);
	decoder.output_to_file(uncom);
	comp.close();
	uncom.close();
}

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main() {

	test_encode();
	test_decode();

	_CrtDumpMemoryLeaks();

	return 0;
}
