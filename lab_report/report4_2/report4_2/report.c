#include <stdio.h> 
#include <string.h> 

struct string {
	char ch[100];
	int length;
};

// 修正后的 get_next，假设字符串从下标 1 开始存储
void get_next(struct string T, int next[]) {
	int i = 1;
	next[1] = 0;
	int j = 0;
	while (i < T.length) {
		if (j == 0 || T.ch[i] == T.ch[j]) {
			++i; ++j;
			next[i] = j;
		}
		else {
			j = next[j];
		}
	}
}

// 修正后的 Index，从下标 1 开始匹配
int Index(struct string s, struct string T, int next[]) {
	int i = 1;
	int j = 1;
	while (i <= s.length && j <= T.length) {
		if (j == 0 || s.ch[i] == T.ch[j]) {
			i++; j++;
		}
		else {
			j = next[j];
		}
	}
	if (j > T.length) {
		// 返回逻辑位置（从1开始）
		return i - T.length;
	}
	else {
		return 0; // 没找到通常返回 0
	}
}

int main() {
	struct string s;
	struct string T;
	int next[100] = { 0 };

	printf("请输入 s 字符串的值：\n");
	scanf_s("%s", s.ch + 1, sizeof(s.ch) - 1); // 从下标 1 开始接收输入
	printf("请输入 T 字符串的值：\n");
	scanf_s("%s", T.ch + 1, sizeof(T.ch) - 1); // 从下标 1 开始接收输入

	// 计算长度
	T.length = strlen(T.ch + 1);
	s.length = strlen(s.ch + 1);

	// 获取 next 数组
	get_next(T, next);

	// 获取位置
	int pos = Index(s, T, next);

	if (pos != 0) {
		printf("子串 T 在主串 S 中首次出现的位置: %d\n", pos);
	}
	else {
		printf("未找到该子串\n");
	}

	return 0;
}