#ifndef _H_HUFFMAN_
#define _H_HUFFMAN_

#include "def.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BYTE_COUNT	256

typedef struct _huffman_node {
	byte value;
	int freq;
	struct _huffman_node *left, *right;
}huffman_node;

typedef huffman_node huffman_tree;

typedef struct _byte_freq_map {
	int map[MAX_BYTE_COUNT];
}byte_freq_map;

byte_freq_map* get_freq_map(FILE* fp);

int get_available_count_of_map_item(byte_freq_map* fmap);

void save_freq_map(byte_freq_map *bfmap, FILE* fp);

byte_freq_map* read_freq_map(FILE* fp);

huffman_tree *build_huffman_tree(byte_freq_map* fmap);

void free_huffman_tree(huffman_tree* htree);

#endif