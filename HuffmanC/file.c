#include "file.h"

//	�ļ�ѹ���������������ļ����й���������
//	��ͳ���ļ��з��ų��ֵ�Ƶ����Ȼ��������������������ӳ���ϵ�������б���
//	������Ƶ��ͳ�ƽ���ͱ�����д���ļ�������
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

//	��ѹ�ļ����ȶ�ȡ����Ƶ��ӳ���ͨ��ӳ�������������
//	��ͨ���������������н��룬д���ѹ�ļ�
void extract_file(FILE* fp_in, FILE* fp_out) {

	byte_freq_map* freq_map = read_freq_map(fp_in);
	huffman_tree* htree = build_huffman_tree(freq_map);

	long long code_length;
	fread(&code_length, sizeof(long long), 1, fp_in);

	decode_to_file(htree, fp_in, fp_out, code_length);

	free_huffman_tree(htree);
	free(freq_map);
}
