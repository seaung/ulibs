#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "file.h"

#define MAX_LENGTH 1024


/* 函数功能: 返回文件大小
 * 入参: filename: 文件的名字, size: 文件大小
 * 出参: size 文件大小
 * */
int * get_file_size(const char *filename, int *size) {
	int status;
	struct stat stat_buffer;

	status = stat(filename, &stat_buffer);
	if (status != 0) {
		return 0;
	}

	size = (int *)stat_buffer.st_size;
	return size;
}

/* 函数功能: 一次性获取文件的内容
 * 入参: filename: 文件名字, content_buffer: 文件内容缓冲区
 * 出参: content_buffer
 * */
char * get_file_content(const char *filename, char *content_buffer) {
	FILE *file;
	char *tmp_buffer;
	int f_index;
	int res;

	file = fopen(filename, "r");

	if (file == NULL) {
		return "";
	}

	memset(&tmp_buffer, 0x00, sizeof(tmp_buffer));
	f_index = 0;

	while(!feof(file)) {
		res = fread(tmp_buffer, 1, MAX_LENGTH, file);
		if (res < 0) {
			break;
		}
		memcpy(content_buffer + f_index, tmp_buffer, res);
		f_index += res;
	}
	content_buffer[f_index] = '\0';
	fclose(file);
	return content_buffer;
}
