#include <stdio.h>

int main() {

	char s[256];
	printf("���ڸ� �Է��ϼ���(100�� �̳�).");
	scanf_s(" %s", s, sizeof(s));
	printf("�Էµ� ���ڴ� %s �Դϴ�.", s);

	return 0;
}