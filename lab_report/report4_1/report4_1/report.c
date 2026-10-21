#include<stdio.h> 
int main() {
	char S[100];
	printf("请输入字符串：");
	scanf_s("%s", S, sizeof(S));
	printf("\n");
	char result[100] = { 0 };
	int i = 0;
	int resultIndex = 0;
	for (i = 0; S[i] != '\0'; i++) {
		char ch = S[i];
		int j = 0;
		int found = 0;
		for (j = 0; result[j] != '\0' && j < resultIndex; j++) {
			//相等，则舍弃 
			if (ch == result[j]) {
				found = 1;
				break;
			}
		}
		//不等，则打印 
		if (!found) {
			result[resultIndex++] = ch;
		}
		result[resultIndex] = '\0';
	}
	//打印最后结果 
	printf("去除重复的字符之后，字符串中的元素为：\n");
	printf("%s ", result);
}