#ifndef _H_HUFFMAN_
#define _H_HUFFMAN_

#include "def.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BYTE_COUNT	256

//	哈夫曼树的节点
typedef struct _huffman_node {
	byte value;
	int freq;
	struct _huffman_node *left, *right;
}huffman_node;

typedef huffman_node huffman_tree;

//	统计频数的时候用到的符号对应其频数的映射表
typedef struct _byte_freq_map {
	int map[MAX_BYTE_COUNT];
}byte_freq_map;

//	从文件中统计影射表
byte_freq_map* get_freq_map(FILE* fp);

//	获取映射表中有效条目的数量
int get_available_count_of_map_item(byte_freq_map* fmap);

//	将映射表保存到文件
void save_freq_map(byte_freq_map *bfmap, FILE* fp);

//	从文件中读取已经保存的映射表
byte_freq_map* read_freq_map(FILE* fp);

//	通过映射表构造出哈夫曼树
huffman_tree *build_huffman_tree(byte_freq_map* fmap);

void free_huffman_tree(huffman_tree* htree);

#endif
