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
