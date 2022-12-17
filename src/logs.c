#include "../include/mstring.h"
#include "../include/logs.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>


extern unsigned short forswitch;
extern unsigned short connectnum;


void formatter_hex_to_string(
		const unsigned char *inbuffer,
		int bytes,
		char *outbuffer,
		int olen,
		char bigorder,
		char convert)
{
	unsigned char i;

	register unsigned char c, b;

	assert(olen >= 85);
	memset(outbuffer, ' ', olen);
	outbuffer[olen - 1] = '\0';

	if (bigorder) {
		for (i = 0; i < bytes; i++) {
			b = inbuffer[i] >> 4;

			if (9 >= b) {
				c = b + '0';
			} else {
				c = (b - 10) + 'A';
			}

			outbuffer[i * 3] = (char)c;

			b = inbuffer[i] & 0x0F;

			if (9 >= b) {
				c = b + '0';
			} else {
				c = (b - 10) + 'A';
			}
			outbuffer[i * 3 + 1] = (char)c;
			outbuffer[i * 3 + 2] = ' ';
		}
	} else {
		for (i = 0; i < bytes; i++) {
			b = inbuffer[i] & 0x0F;

			if (9 >= b) {
				c = b + '0';
			} else {
				c = (b - 10) + 'A';
			}

			outbuffer[i * 3] = (char)c;

			b = inbuffer[i] >> 4;

			if (9 >= b) {
				c = b + '0';
			} else {
				c = (b - 10) + 'A';
			}

			outbuffer[i * 3 + 1] = (char)c;
			outbuffer[i * 3 + 2] = ' ';
		}
	}

	if (convert) {
		b = 52;
		for (i = 0; i < bytes; i++) {
			if(isprint(inbuffer[i])) {
				outbuffer[b++] = (char)inbuffer[i];
			} else {
				switch(inbuffer[i]) {
					case '\r':
						outbuffer[b++] = '\\';
						outbuffer[b++] = 'r';
						break;
					case '\t':
						outbuffer[b++] = '\\'; 
						outbuffer[b++] = 't';
						break;
					case '\n':
						outbuffer[b++] = '\\'; 
						outbuffer[b++] = 'n';
						break;
					case '\f':
						outbuffer[b++] = '\\'; 
						outbuffer[b++] = 'f';
						break;
					case '\v':
					    	outbuffer[b++] = '\\'; 
					    	outbuffer[b++] = 'v';
					    	break;
					case ' ':
					    	outbuffer[b++] = 's'; 
						outbuffer[b++] = 'p';
					    	break;
					case 0x08:
					    	outbuffer[b++] = 'b'; 
						outbuffer[b++] = 's';
					    	break;
					case 0x7f:
					    	outbuffer[b++] = 'd'; 
					    	outbuffer[b++] = 'l';
					    	break;
					case '\0':
					    	outbuffer[b++] = '\\'; 
					    	outbuffer[b++] = '0';
					    	break;
					default:
					    	outbuffer[b++] = '.';
					    	break;
				}
			}
		}
		str_trim_tor(outbuffer);
	} else {
		for (i = 0; i < bytes; i++) {
			if (isprint(inbuffer[i])) {
				outbuffer[52 + i] = (char)inbuffer[i];
			} else {
				outbuffer[52 + i] = '.';
			}
		}
	}
}

logs_t * init_log(const char * filename, int max_size, char max_count, char nwsyslog) {
	assert(filename != NULL && max_size > 0 && max_size < (0x01 << 26) && max_count > 0 && max_count < 110);

	logs_t * p_log = (logs_t *) calloc(1, sizeof(logs_t));

	p_log->max_size = max_size;
	p_log->mcount = max_count;

	if (NULL == (p_log->logfd = fopen(filename, "a+"))) {
		free(p_log);
		return NULL;
	}

	p_log->filename = strdup(filename);

	fchmod(fileno(p_log->logfd), S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	clearerr(p_log->logfd);

	if (!nwsyslog) {
		p_log->wsyslog = 1;
	}

#ifdef _REENTRANT
	pthread_mutex_init(*p_log->lock, NULL);
#endif
	return p_log;

}

void print_log(logs_t *log, const char *fmt, ...) {}

void release_log(logs_t *log) {
	assert(log != NULL && log->filename != NULL && log->logfd != NULL);
	
	if (log->wsyslog)
		closelog();

#ifdef _REENTRANT
	pthread_mutex_destory(&log->lock);
#endif
	free(log->filename);
	fflush(log->logfd);
	fclose(log->logfd);
	log->logfd = NULL;
	free(log);
}

void log_2_hexprint(FILE * logfd, const char * buffer, int len, char bigorder, char convert) {}

void syslog_2_hexprint(const char * buffer, int len, char bigorder, char convert) {}

void log_2_hexprint_data(FILE *logfd, const char * buffer, int len, char bigorder, char convert) {}

