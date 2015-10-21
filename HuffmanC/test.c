//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//
//#include "file.h"
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//void test_freq_map() {
//
//	FILE* fp = fopen("test.txt", "rb");
//	byte_freq_map *freq_map = get_freq_map(fp);
//
//	free(freq_map);
//	fclose(fp);
//}
//
//void test_build_huffman_tree() {
//
//	FILE* fp = fopen("test.txt", "rb");
//	byte_freq_map *freq_map = get_freq_map(fp);
//
//	huffman_tree* htree = build_huffman_tree(freq_map);
//
//	free_huffman_tree(htree);
//	free(freq_map);
//	fclose(fp);
//}
//
//void test_build_code_map() {
//
//	FILE* fp = fopen("test.txt", "rb");
//	byte_freq_map *freq_map = get_freq_map(fp);
//
//	huffman_tree* htree = build_huffman_tree(freq_map);
//	byte_code_map* bcmap = build_byte_to_code_map(htree);
//
//	free_huffman_tree(htree);
//	free(freq_map);
//	free_byte_to_code_map(bcmap);
//	fclose(fp);
//}
//
//void test_encode_file() {
//
//	FILE* fp = fopen("test.txt", "rb");
//	byte_freq_map *freq_map = get_freq_map(fp);
//
//	huffman_tree* htree = build_huffman_tree(freq_map);
//	byte_code_map* bcmap = build_byte_to_code_map(htree);
//
//	FILE* fp_out = fopen("com.txt", "wb");
//	fseek(fp, 0, SEEK_SET);
//	int code_length = encode_file_to(bcmap, fp, fp_out);
//
//	free_huffman_tree(htree);
//	free(freq_map);
//	free_byte_to_code_map(bcmap);
//	fclose(fp);
//	fclose(fp_out);
//}
//
//void test_decode_file() {
//
//	FILE* fp = fopen("test.txt", "rb");
//	byte_freq_map *freq_map = get_freq_map(fp);
//
//	huffman_tree* htree = build_huffman_tree(freq_map);
//	byte_code_map* bcmap = build_byte_to_code_map(htree);
//
//	FILE* fp_out = fopen("com.txt", "wb");
//	fseek(fp, 0, SEEK_SET);
//	int code_length = encode_file_to(bcmap, fp, fp_out);
//	fclose(fp_out);
//
//	fp_out = fopen("com.txt", "rb");
//	FILE* fp_to = fopen("out.txt", "wb");
//	decode_to_file(htree, fp_out, fp_to, code_length);
//	
//	free_huffman_tree(htree);
//	free(freq_map);
//	free_byte_to_code_map(bcmap);
//	fclose(fp);
//	fclose(fp_out);
//	fclose(fp_to);
//}
//
//void test_compress_and_extract() {
//
//	FILE* fp_src = fopen("test.txt", "rb");
//	FILE* fp_com = fopen("comp.txt", "wb");
//	FILE* fp_ext = fopen("ext.txt", "wb");
//
//	compress_file(fp_src, fp_com);
//	fclose(fp_com);
//
//	fp_com = fopen("comp.txt", "rb");
//	extract_file(fp_com, fp_ext);
//
//	fclose(fp_src);
//	fclose(fp_com);
//	fclose(fp_ext);
//}
//
//int main() {
//
//	test_freq_map();
//	test_build_huffman_tree();
//	test_build_code_map();
//	test_encode_file();
//	test_decode_file();
//	test_compress_and_extract();
//
//	_CrtDumpMemoryLeaks();
//
//	return 0;
//}
