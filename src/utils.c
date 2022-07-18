#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <net/if.h>

#include "utils.h"


/* 函数功能: 生成指定长度的随机字符串
 * 入参: length, 生成字符串的长度
 * 出参: res_string,生成的目标字符串
 * 返回值: 成功返回生成的字符串,失败返回NULL
 * */
char * random_string(int length, char *res_string) {
	int flag, index;
	char *tmp_string;

	srand((unsigned) time(NULL));

	if ((tmp_string = (char *) malloc(length)) == NULL) {
		return NULL;
	}

	for (index = 0; index < length; index++) {
		flag = rand() % 3;

		switch(flag) {
			case 0:
				tmp_string[index] = 'A' + rand() % 26;
				break;
			case 1:
				tmp_string[index] = 'a' + rand() % 26;
				break;
			case 2:
				tmp_string[index] = '0' + rand() % 10;
				break;
			default:
				tmp_string[index] = 'x';
				break;
		}
	}

	tmp_string[length] = '\0';

	memcpy(res_string, tmp_string, length);
	return res_string;
}

/* 函数功能: 获取mac地址
 * 入参: peth,网卡名称, buffer_len,缓冲区长度
 * 出参: mac_buffer, mac地址
 * 返回值: 成功返回0,失败返回-1
 * */
int get_mac_address(char *peth, char *mac_buffer, int buffer_len) {
	struct ifreq ifreq;
	int sock = 0;
	int index;
	char mac[32] = "";

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return -1;
	}

	strcpy(ifreq.ifr_name, peth);
	if (ioctl(sock, SIOCGIFHWADDR, &ifreq) < 0) {
		close(sock);
		return -1;
	}

	for (index = 0; index < 6; index++) {
		sprintf(mac + 3 * index, "%02x", (unsigned char)ifreq.ifr_hwaddr.sa_data[index]);
	}
	mac[strlen(mac) - 1] = '\0';
	memcpy(mac_buffer, mac, buffer_len);
	close(sock);
	return 0;
}

/* 函数功能: 获取当前日期时间
 * 出参: time_string,
 * */
void get_time_to_string(char *time_string) {
	time_t td;
	struct tm *p;
	time(&td);
	p = localtime(&td);
	snprintf(time_string, sizeof(time_string), "%d-%d-%d %d:%d:%d\n", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}

void timestamp2string(time_t timestamp, char *time_buffer) {
	struct tm *time;

	timestamp += 8 * 60 * 60;
	time = localtime(&timestamp);

	strftime(time_buffer, sizeof(time_buffer), "%F %T", time);
}
