#ifndef _H_FILE_
#define _H_FILE_

#include "encode.h"
#include "decode.h"

//	压缩文件
void compress_file(FILE* fp_in, FILE* fp_out);

//	解压缩文件
void extract_file(FILE* fp_in, FILE* fp_out);

#endif