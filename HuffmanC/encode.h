#ifndef _H_ENCODE_
#define _H_ENCODE_

#include "huffman.h"

#define MAX_CODE_LENGTH 256

//	编码前符号到对应编码的映射表
typedef struct _byte_code_map {
	char* map[MAX_BYTE_COUNT];
}byte_code_map;

//	通过哈夫曼树构造出符号到编码的映射表
byte_code_map* build_byte_to_code_map(huffman_tree* htree);

void free_byte_to_code_map(byte_code_map* bcmap);

//	将输入文件，按照映射表的编码方式进行编码，并输出到文件
long long encode_file_to(byte_code_map* bcmap, FILE* fp_in, FILE* fp_out);

#endif