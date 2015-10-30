#include <iostream>
#include <fstream>
#include "huffman_file_compressor.h"
#include "huffman_file_extractor.h"

// application �࣬������ں����õĵ�һ����
// ����������е������Ĵ�����װ��������
class application {
public:
	application(int argc, char** argv) {
		this->argc = argc;
		this->argv = argv;
	}
	virtual int run() = 0;
	virtual ~application(){}
protected:
	int argc;
	char** argv;
};

//	command�ӿڣ����в�����ʵ��command�ӿ�
class command {
public:
	virtual int execute() = 0;
	virtual ~command() {}
};

// ѹ�������ʵ�֣�����compressor����ļ�����ѹ��
class compress_command 
	: public command{
public:
	compress_command(const char* in, const char* out) 
		:input_file_path(in), output_file_path(out){
	}
	int execute() {

		huffman_file_compressor compressor;
		std::ifstream in(input_file_path, std::ios::binary);
		std::ofstream out(output_file_path, std::ios::binary);
		compressor.compress(in, out);
		in.close();
		out.close();
		return 0;
	}
private:
	std::string input_file_path;
	std::string output_file_path;
};


// ��ѹ�����ʵ�֣�����extractor����ļ����н�ѹ
class extract_command 
	: public command{
public:
	extract_command(const char* in, const char* out)
		:input_file_path(in), output_file_path(out) {
	}
	int execute() {

		huffman_file_extractor extractor;
		std::ifstream in(input_file_path, std::ios::binary);
		std::ofstream out(output_file_path, std::ios::binary);
		extractor.extract(in, out);
		in.close();
		out.close();
		return 0;
	}
private:
	std::string input_file_path;
	std::string output_file_path;
};

// ��������ܣ���application��̳�
class huffman_app 
	:public application {
public:
	huffman_app(int argc, char** argv) 
		: application(argc, argv){
	}
	// �������е��ڲ���ڣ����жϲ����������Ӧ��command��Ȼ��ִ��
	int run() {

		command* cmd = NULL;
		
		if (argc < 4) {
			usage();
			return 1;
		}

		if (strcmp(argv[1], "-c") == 0) {
			cmd = new compress_command(argv[2], argv[3]);
		}
		else if (strcmp(argv[1], "-x") == 0) {
			cmd = new extract_command(argv[2], argv[3]);
		}

		int ret = cmd->execute();
		delete cmd;
		return ret;
	}
	void usage() {
		printf("Usage:\n");
		printf("\tcompress: %s -c in out\n", get_command_file_name());
		printf("\textract: %s -x in out\n", get_command_file_name());
	}
	char* get_command_file_name() {
		char *cmd = &argv[0][strlen(argv[0]) -1];
		while (*cmd != '\\' && *cmd != '/') {
			cmd--;
		}
		return ++cmd;
	}
};

// �����õ���ؿ⣬���ڼ���ڴ�й¶
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
// ������ڣ�����app����ִ��
int main(int argc, char** argv) {

	application* app = new huffman_app(argc, argv);
	app->run();
	delete app;

//	�ڴ�й¶���
//	_CrtDumpMemoryLeaks();

	return 0;
}
