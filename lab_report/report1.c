#include <stdio.h>
#define OK 1
#define ERROR 0
typedef int Elemtype;
typedef int Status;

typedef struct Triplet
{
	Elemtype data[3];
}Tri;

Status InitTriplet(Tri* T, Elemtype v1, Elemtype v2, Elemtype v3)
{
	T->data[0] = v1;
	T->data[1] = v2;
	T->data[2] = v3;
	return OK;
}

Status Get(Tri T, int i, Elemtype* e)
{
	if (i < 1 || i>3)
	{
		return ERROR;
	}
	 *e = T.data[i-1];
	return OK;
}

Status Put(Tri* T, int i, Elemtype e)
{
	if (i < 1 || i>3)
	{
		return ERROR;
	}
	T->data[i-1] = e;
	return OK;
}

Status Max(Tri T, Elemtype* e)
{
	*e = T.data[0]; //假设第一个是最大值
	if (T.data[1] > *e) *e= T.data[1];
	if (T.data[2] > *e)*e = T.data[2];
	return OK;
	
}



int main()
{
	//构建三元组
	Tri T;
   InitTriplet(&T, 10, 5, 20);
   printf("构造三元组成功：（%d, %d, %d)\n", T.data[0], T.data[1], T.data[2]);

   //测试读取
   Elemtype x = 0;
   Get(T, 2,&x );
   printf("读取第二个元素：%d\n", x);

   //存入数据
   Put(&T, 1, 99);
   printf("修改后：（%d, %d, %d)\n", T.data[0], T.data[1], T.data[2]);

   //求最大值
   Max(T, &x);
   printf("最大值：%d\n", x);




	return 0;
}