#include "file.h"

//	文件压缩函数，将输入文件进行哈夫曼编码
//	先统计文件中符号出现的频数，然后构造哈夫曼树，构造编码映射关系，最后进行编码
//	将符号频数统计结果和编码结果写入文件，保存
void compress_file(FILE* fp_in, FILE* fp_out) {

	byte_freq_map *freq_map = get_freq_map(fp_in);

	huffman_tree* htree = build_huffman_tree(freq_map);
	byte_code_map* bcmap = build_byte_to_code_map(htree);

	fseek(fp_in, 0, SEEK_SET);
	save_freq_map(freq_map, fp_out);
	int offset = ftell(fp_out);
	fseek(fp_out, sizeof(long long), SEEK_CUR);
	long long code_length = encode_file_to(bcmap, fp_in, fp_out);
	fseek(fp_out, offset, SEEK_SET);
	fwrite(&code_length, sizeof(long long), 1, fp_out);

	free_huffman_tree(htree);
	free(freq_map);
	free_byte_to_code_map(bcmap);
}

//	解压文件，先读取符号频数映射表，通过映射表构建哈夫曼树
//	再通过哈夫曼树，进行解码，写入解压文件
void extract_file(FILE* fp_in, FILE* fp_out) {

	byte_freq_map* freq_map = read_freq_map(fp_in);
	huffman_tree* htree = build_huffman_tree(freq_map);

	long long code_length;
	fread(&code_length, sizeof(long long), 1, fp_in);

	decode_to_file(htree, fp_in, fp_out, code_length);

	free_huffman_tree(htree);
	free(freq_map);
}
