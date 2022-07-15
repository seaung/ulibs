#ifndef __FILE_H_
#define __FILE_H_

int * get_file_size(const char *filename, int *size);

char * get_file_content(const char *filename, char *content_buffer);
#endif
