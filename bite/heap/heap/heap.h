#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<stdbool.h>


typedef int HPDataType;

typedef struct Heap
{
	HPDataType* a;
	int size;
	int capacity;
}HP;

void Swap(HPDataType* p1, HPDataType* p2);
void AdjustUp(HPDataType* a, int child);
void AdjustDown(HPDataType* a, int n, int parent);

void HPInit(HP* php);
void HPDestroy(HP* php);
void HPPush(HP* php, HPDataType x);
void HPPop(HP* php);
HPDataType HPTop(HP* php);
bool HPEmpty(HP* php);#pragma once
