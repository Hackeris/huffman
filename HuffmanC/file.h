#ifndef _H_FILE_
#define _H_FILE_

#include "encode.h"
#include "decode.h"

void compress_file(FILE* fp_in, FILE* fp_out);

void extract_file(FILE* fp_in, FILE* fp_out);

#endif