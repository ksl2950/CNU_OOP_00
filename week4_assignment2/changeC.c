#include <stdio.h>

int main() {

	char s[256];
	printf("문자를 입력하세요(100자 이내).");
	scanf_s(" %s", s, sizeof(s));
	printf("입력된 문자는 %s 입니다.", s);

	return 0;
}