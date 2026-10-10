#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
typedef int Status;
typedef int Elemtype;
#define SQLMAXSIZE 100
struct SqList {
	Elemtype* base;
	int length;
};
Status InitList(struct SqList* L) {
	L->base = (Elemtype*)malloc(SQLMAXSIZE * sizeof(Elemtype));
	if (!L->base) return 1;
	L->length = 4;
	L->base[0] = 2;
	L->base[1] = 3;
	L->base[2] = 4;
	L->base[3] = 5;
	return 0;
}
Status ListInsert(struct SqList* L, int i, Elemtype e) {
	if (i < 1 || i > L->length + 1 || L->length >= SQLMAXSIZE) return 1;
	Elemtype* p = &(L->base[i - 1]);
	for (Elemtype* q = &(L->base[L->length - 1]); q >= p; --q) {
		*(q + 1) = *q; // 插入位置及之后的元素右移
	}
	*p = e;
	++L->length;
	return 0;
}
Status ListDelete(struct SqList* L, int i, Elemtype* e) {
	if (i < 1 || i > L->length) return 1;
	Elemtype* p = &(L->base[i - 1]);
	*e = *p;
	for (Elemtype* q = L->base + L->length - 1; p < q; ++p) {
		*(p) = *(p + 1);
	}
	--L->length;
	return 0;
}
int main() {
	struct SqList L;
	Elemtype e = 0;
	if (InitList(&L) == 0) {
		printf("原始顺序表为：");
		for (int i = 0; i < L.length; i++) {
			printf("%d ", L.base[i]);
		}
		printf("\n");
		Status res = ListInsert(&L, 4, 9);
		if (res == 0) {
			printf("在第四个位置前插入元素 9 之后，新的顺序表变为：");
			for (int i = 0; i < L.length; i++) {
				printf("%d ", L.base[i]);
			}
			printf("\n");
		}
		Status ret = ListDelete(&L, 3, &e);
		if (ret == 0) {
			printf("删除第三个位置上的元素%d 后，新的顺序表变为：", e);
			for (int i = 0; i < L.length; i++) {
				printf("%d ", L.base[i]);
			}
			printf("\n");
		}
	}
	free(L.base); // 释放内存
	return 0;
}
