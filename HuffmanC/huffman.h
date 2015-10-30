#ifndef _H_HUFFMAN_
#define _H_HUFFMAN_

#include "def.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BYTE_COUNT	256

//	���������Ľڵ�
typedef struct _huffman_node {
	byte value;
	int freq;
	struct _huffman_node *left, *right;
}huffman_node;

typedef huffman_node huffman_tree;

//	ͳ��Ƶ����ʱ���õ��ķ��Ŷ�Ӧ��Ƶ����ӳ���
typedef struct _byte_freq_map {
	int map[MAX_BYTE_COUNT];
}byte_freq_map;

//	���ļ���ͳ��Ӱ���
byte_freq_map* get_freq_map(FILE* fp);

//	��ȡӳ�������Ч��Ŀ������
int get_available_count_of_map_item(byte_freq_map* fmap);

//	��ӳ����浽�ļ�
void save_freq_map(byte_freq_map *bfmap, FILE* fp);

//	���ļ��ж�ȡ�Ѿ������ӳ���
byte_freq_map* read_freq_map(FILE* fp);

//	ͨ��ӳ��������������
huffman_tree *build_huffman_tree(byte_freq_map* fmap);

void free_huffman_tree(huffman_tree* htree);

#endif
