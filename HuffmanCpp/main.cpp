#include <iostream>
#include <fstream>
#include "huffman_file_compressor.h"
#include "huffman_file_extractor.h"

// application 类，程序入口后会调用的第一个类
// 负责程序运行的上下文处理，封装程序的入口
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

//	command接口，所有操作都实现command接口
class command {
public:
	virtual int execute() = 0;
	virtual ~command() {}
};

// 压缩命令的实现，调用compressor类对文件进行压缩
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


// 解压命令的实现，调用extractor类对文件进行解压
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

// 程序主框架，从application类继承
class huffman_app 
	:public application {
public:
	huffman_app(int argc, char** argv) 
		: application(argc, argv){
	}
	// 程序运行的内部入口，先判断参数，构造对应的command，然后执行
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

// 调试用的相关库，用于检测内存泄露
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
// 程序入口，构造app对象，执行
int main(int argc, char** argv) {

	application* app = new huffman_app(argc, argv);
	app->run();
	delete app;

//	内存泄露检查
//	_CrtDumpMemoryLeaks();

	return 0;
}
