#include "encode.h"

//	对哈夫曼树进行DFS遍历，通过维护一个编码栈来记录当前节点对应的编码
//	没遍历到叶子节点，拷贝编码栈中的编码串到符号编码映射表中
static void travel_huffman_tree(huffman_node* node, byte_code_map* bcmap,
	char* tmp_code, int *code_len) {

	if (node->left == NULL && node->right == NULL) {
		bcmap->map[node->value] = strdup(tmp_code);
	}
	if (node->left != NULL) {
		tmp_code[(*code_len)++] = '0';
		travel_huffman_tree(node->left, bcmap, tmp_code, code_len);
		tmp_code[--(*code_len)] = '\0';
	}
	if (node->right != NULL) {
		tmp_code[(*code_len)++] = '1';
		travel_huffman_tree(node->right, bcmap, tmp_code, code_len);
		tmp_code[--(*code_len)] = '\0';
	}
}

//	通过哈夫曼树构建出符号到编码的映射表
byte_code_map* build_byte_to_code_map(huffman_tree* htree) {

	char tmp_code[256] = { 0 };
	int code_length = 0;
	byte_code_map* bcmap = (byte_code_map*)malloc(sizeof(byte_code_map));
	memset(bcmap, 0, sizeof(byte_code_map));

	travel_huffman_tree(htree, bcmap, tmp_code, &code_length);
	return bcmap;
}

void free_byte_to_code_map(byte_code_map* bcmap) {
	int i;
	for (i = 0; i < MAX_BYTE_COUNT; i++) {
		if (bcmap->map[i] != NULL) {
			free(bcmap->map[i]);
		}
	}
	free(bcmap);
}

static int mini(int a, int b) {
	return a < b ? a : b;
}

//	将01编码串，通过位运算转化为字节
static byte string_to_byte(const char* str, int n) {
	byte c = 0;
	int i;
	for (i = 0; i < n; i++) {
		c |= (str[i] == '1' ? (1 << (7 - i)) : 0);
	}
	return c;
}

//	整理编码时所用到的缓冲区
static void shift_left_buffer(char* buffer, int* start, int* end) {

	if (*start > 8) {
		int i;
		for (i = 0; i < *end - *start; i++) {
			buffer[i] = buffer[i + *start];
		}
		*end -= *start;
		*start = 0;
	}
}

//	将输入文件中的符号通过编码映射表，编码后写入到输出文件中
//	每读出一个符号，找到对应编码，放入编码缓冲
//	编码缓冲中的串足够组成字节的时候，将其转化为字节，重复进行编码
//	直到整个文件结束，将缓冲中的余下部分转为字节，不足8位用0补齐
long long encode_file_to(byte_code_map* bcmap, FILE* fp_in, FILE* fp_out) {

	char code_buffer[MAX_CODE_LENGTH];
	int start = 0, end = 0;
	long long code_length = 0;

	byte b;
	fread(&b, sizeof(byte), 1, fp_in);
	while (!feof(fp_in)) {

		strcpy(&code_buffer[end], bcmap->map[b]);
		code_length += strlen(bcmap->map[b]);
		end += strlen(bcmap->map[b]);

		while (end - start > 8) {
			byte bo = string_to_byte(&code_buffer[start], 8);
			fwrite(&bo, sizeof(byte), 1, fp_out);
			start += 8;
		}

		shift_left_buffer(code_buffer, &start, &end);

		fread(&b, sizeof(byte), 1, fp_in);
	}
	int n = mini(end - start, 8);
	if (n == 0) {
		return code_length;
	}
	b = string_to_byte(&code_buffer[start], n);
	start += n;
	fwrite(&b, sizeof(byte), 1, fp_out);
	return code_length;
}