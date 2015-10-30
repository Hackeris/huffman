#ifndef _H_DECODE_
#define _H_DECODE_

#include "huffman.h"

// 将字符流转化为编码位流的适配器结构体
typedef struct _byte_to_bit_stream {
	FILE* fp_in;
	byte buffer;
	long long code_length;
	long long offset;
	int buffer_offset;
}byte_to_bit_stream;

//	打开一个输入流
byte_to_bit_stream* open_bit_stream(FILE* fp_in, long long code_length);

//	关闭输入流
void close_bit_stream(byte_to_bit_stream* btbs);

//	将输入流中的数据解码并输出到文件
void decode_to_file(huffman_tree* htree, FILE* fp_in,
	FILE* fp_out, long long code_length);

#endif