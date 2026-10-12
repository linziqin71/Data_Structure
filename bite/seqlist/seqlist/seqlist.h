#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
//定义顺序表的结构

//#define N 100
//
////静态顺序表
//struct SeqList
//{
//	int arr[N];
//	int size;//有效数据个数
//};

typedef int SLDataType;
//动态顺序表
typedef struct SeqList
{
	SLDataType* arr;
	int size; //有效数据个数
	int capacity; //空间大小
}SL;

//typedef struct SeqList SL;

//顺序表初始化
void SLInit(SL* ps);
//顺序表的销毁
void SLDestroy(SL* ps);
void SLPrint(SL s);

//头部插入删除 / 尾部插入删除
void SLPushBack(SL* ps, SLDataType x);
void SLPushFront(SL* ps, SLDataType x);

void SLPopBack(SL* ps);
void SLPopFront(SL* ps);

//指定位置之前插入/删除数据
void SLInsert(SL* ps, int pos, SLDataType x);
void SLErase(SL* ps, int pos);
int SLFind(SL* ps, SLDataType x);
