#ifndef _LOGS_H
#define _LOGS_H

#include <stdio.h>
#include <pthread.h>

#pragma pack(push, 1)
struct logs {
	FILE* 	logfd;
	int 	max_size;
	char 	filename;
#ifdef _PEENTRANT
	pthread_mutex_t lock;
#endif
	char 	ccount;
	char 	mcount;
	char 	wsyslog;
};
#pragma pack(pop)

typedef struct logs logs_t;

logs_t init_log(const char * filename, int max_size, char max_count, char nwsyslog);
void print_log(logs_t *log, const char *fmt, ...);
void release_log(logs_t *log);
void log_2_hexprint(FILE * logfd, const char * buffer, int len, char bigorder, char convert);
void syslog_2_hexprint(const char * buffer, int len, char bigorder, char convert);
void log_2_hexprint_data(FILE *logfd, const char * buffer, int len, char bigorder, char convert);

#endif
