#ifndef __UTILS_H_
#define __UTILS_H_
#include <time.h>

char * random_string(int length, char *res_string);

int get_mac_address(char *peth, char *mac_buffer, int buffer_len);

void get_time_to_string(char *time_string);

void timestamp2string(time_t timestamp, char *time_buffer);

#endif
