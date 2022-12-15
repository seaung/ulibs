#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "mstring.h"


/* 函数功能: 截取指定位置字符串
 * 入参: src,带截取的原始字符串, index, 从那里开始截取, length, 取多少长度
 * 出参: dest,截取后的字符串
 * 返回值: 成功返回0,失败返回-1
 * */
int cut_string(char * dest, const char * src, int index, int length) {
	int len;

	len = strlen(dest);

	if (length > len) {
		return -1;
	}

	strncpy(dest, src + index, length);

	dest[len + 1] = '\0';

	return 0;
}

char *str_trim_tol(char *s) {
	register char *p, *q;
	p = q = s;
	while(isspace(*q))
		++q;
	while (*q)
		*p++ = *q++;
	*p = '\0';
	return s;
}

char *str_trim_tor(char *s) {
	register char *p;

	p = s + strlen(s);
	if (p > s) {
		do { --p; } while(isspace(*p));
		p[1] = '\0';
	}
	return s;
}
