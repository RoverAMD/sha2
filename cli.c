/*
 * FIPS 180-2 SHA-224/256/384/512 implementation
 * Command-line interface
 *
 * Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>
 *               2019, 2020 Tim K <timprogrammer@rambler.ru>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include "sha2.h"

void usage(const char* ownName) {
	fprintf(stderr, "Usage: %s <SHA algorithm> <filename>\n", ownName);
	fprintf(stderr, "       %s <SHA algorithm> string:<string value>\n", ownName);
	return;
}

void error(const char* arg, ...) {
	if (!arg)
		return;
	
	va_list vl;
	va_start(vl, arg);
	fprintf(stderr, "%s ", "{EasySHA Error} ");
	vfprintf(stderr, arg, vl);
	va_end(vl);
}

bool isHelpFlag(const char* flag) {
	return (strcmp(flag, "-?") == 0 || strcmp(flag, "-h") == 0 || strcmp(flag, "--help") == 0);
}

bool hasFilename(const char* filename) {
	return (filename && access(filename, F_OK) == 0);
}

char* readFilename(const char* filename, size_t* dataSizePtr) {
	if (!filename)
		return NULL;
	
	FILE* desc = fopen(filename, "r");
	if (!desc)
		return NULL;
	
	fseek(desc, 0L, SEEK_END);
	size_t bufSize = ftell(desc);
	rewind(desc);
	
	if (dataSizePtr)
		(*dataSizePtr) = bufSize;
	
	char* buffer = calloc(bufSize, sizeof(char));
	fread(buffer, sizeof(char), bufSize, desc);
	
	fclose(desc);
	return buffer;
}

int main(const int argc, const char** argv) {
	if (argc < 3 || isHelpFlag(argv[1])) {
		usage(argv[0]);
		return 1;
	}
	
	const char* filename = argv[2];
	bool isFile = true;
	if (strncmp(filename, "string:", 7) == 0) {
		isFile = false;
		filename += 7;
	}
	
	char* data = NULL;
	size_t dataSize = 0;
	
	if (isFile) {
		if (!hasFilename(filename)) {
			error("'%s' - no such file or the file is unaccessible.", filename);
			return 2;
		}
		
		data = readFilename(filename, &dataSize);
	} else {
		data = strdup(filename);
		dataSize = strlen(filename);
	}
	
	char* result = easysha((unsigned)(atoi(argv[1])), data, dataSize);
	free(data);
	
	if (!result) {
		error("Invalid algorithm specified or the data could not be read.");
		return 3;
	}
	
	fprintf(stdout, "%s\n", result);
	free(result);
	
	return 0;
}