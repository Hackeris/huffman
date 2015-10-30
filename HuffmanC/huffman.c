#include "huffman.h"

byte_freq_map* get_freq_map(FILE* fp) {

	byte_freq_map* freq_map =
		(byte_freq_map*)malloc(sizeof(byte_freq_map));
	memset(freq_map, 0, sizeof(byte_freq_map));

	byte b;
	fread(&b, sizeof(byte), 1, fp);
	while (!feof(fp)) {
		freq_map->map[b] ++;
		fread(&b, sizeof(byte), 1, fp);
	}

	return freq_map;
}

static int get_available_count_of_map_item(byte_freq_map* fmap) {

	int available_count = 0;
	int i;
	for (i = 0; i < MAX_BYTE_COUNT; i++) {
		if (fmap->map[i] != 0) {
			available_count++;
		}
	}
	return available_count;
}

//	保存符号频数映射表到文件
void save_freq_map(byte_freq_map *bfmap, FILE* fp) {

	int count = get_available_count_of_map_item(bfmap);
	fwrite(&count, sizeof(int), 1, fp);

	int i;
	for (i = 0; i < MAX_BYTE_COUNT; i++) {
		if (bfmap->map[i] != 0) {
			fwrite(&i, sizeof(byte), 1, fp);
			fwrite(&bfmap->map[i], sizeof(bfmap->map[i]), 1, fp);
		}
	}
}

//	从文件中读取符号频数映射表
byte_freq_map* read_freq_map(FILE* fp) {

	byte_freq_map* bmap = (byte_freq_map*)
		malloc(sizeof(byte_freq_map));
	int i;
	int count;

	memset(bmap, 0, sizeof(byte_freq_map));
	fread(&count, sizeof(int), 1, fp);
	for (i = 0; i < count; i++) {
		byte b;
		fread(&b, sizeof(byte), 1, fp);
		fread(&bmap->map[b], sizeof(int), 1, fp);
	}
	return bmap;
}

static int huffman_cmp(const void* lc, const void* rc) {
	return -((*(huffman_tree**)lc)->freq
		- (*(huffman_tree**)rc)->freq);
}

//	将符号到频数的映射表转化为构造哈夫曼树前所需要的森林
static huffman_tree** dump_to_forest(byte_freq_map* fmap, int* size) {

	int available_count = get_available_count_of_map_item(fmap);
	huffman_tree** forest = (huffman_tree**)malloc(
		sizeof(huffman_tree*) * available_count);
	memset(forest, 0, sizeof(huffman_tree*) * available_count);
	int i, j = 0;
	for (i = 0; i < MAX_BYTE_COUNT; i++) {
		if (fmap->map[i] != 0) {
			forest[j] = (huffman_tree*)malloc(sizeof(huffman_tree));
			forest[j]->value = i;
			forest[j]->freq = fmap->map[i];
			forest[j]->left = forest[j]->right = 0;
			j++;
		}
	}
	*size = available_count;
	return forest;
}

//	将构造过程中的哈夫曼树重新按顺序插入到森林中
static void insert_to_huffman_forest(huffman_tree *tree,
	huffman_tree** forest, int* size) {

	forest[(*size)++] = tree;
	int last = *size - 1;
	while (last > 0 && forest[last]->freq > forest[last - 1]->freq) {
		huffman_tree* tmp = forest[last];
		forest[last] = forest[last - 1];
		forest[last - 1] = tmp;
		last--;
	}
}

static huffman_tree *get_minimal_tree(huffman_tree** forest, int*size) {
	return forest[--*size];
}

//	构建哈夫曼树，先将森林中的树按照权值进行排序，然后每取出两个权值最小的，组合出新的树
//	放入森林，同时进行排序
//	重复上述过程，直到森林中只留下一棵树
huffman_tree *build_huffman_tree(byte_freq_map* fmap) {

	int forest_size;
	huffman_tree *htree;
	huffman_tree** forest = dump_to_forest(fmap, &forest_size);
	qsort(forest, forest_size, sizeof(huffman_tree*), huffman_cmp);

	while (forest_size > 1) {
		huffman_tree* tmp1 = get_minimal_tree(forest, &forest_size);
		huffman_tree* tmp2 = get_minimal_tree(forest, &forest_size);
		huffman_tree* tmp3 = (huffman_tree*)malloc(sizeof(huffman_tree));
		tmp3->left = tmp1;
		tmp3->right = tmp2;
		tmp3->freq = tmp1->freq + tmp2->freq;
		insert_to_huffman_forest(tmp3, forest, &forest_size);
	}
	htree = forest[0];
	free(forest);
	return htree;
}

static void free_buffman_node(huffman_tree* hnode) {
	if (hnode->left == NULL && hnode->right == NULL) {
		free(hnode);
		return;
	}
	if (hnode->left != NULL) {
		free_buffman_node(hnode->left);
	}
	if (hnode->right != NULL) {
		free_buffman_node(hnode->right);
	}
	free(hnode);
}

void free_huffman_tree(huffman_tree* htree) {
	free_buffman_node(htree);
}

