#include"SeqList.h"
void SLInit(SL* ps)
{
	ps->arr = NULL;
	ps->size = ps->capacity = 0;
}
//顺序表的销毁
void SLDestroy(SL* ps)
{
	if (ps->arr) //等价于  if(ps->arr != NULL)
	{
		free(ps->arr);
	}
	ps->arr = NULL;
	ps->size = ps->capacity = 0;
}
void SLCheckCapacity(SL* ps)
{
	//插入数据之前先看空间够不够
	if (ps->capacity == ps->size)
	{
		//申请空间
		//malloc calloc realloc  int arr[100] --->增容realloc
		//三目表达式
		int newCapacity = ps->capacity == 0 ? 4 : 2 * ps->capacity;
		SLDataType* tmp = (SLDataType*)realloc(ps->arr, newCapacity * sizeof(SLDataType));//要申请多大的空间
		if (tmp == NULL)
		{
			perror("realloc fail!");
			exit(1);//直接退出程序，不再继续执行
		}
		//空间申请成功
		ps->arr = tmp;
		ps->capacity = newCapacity;
	}
}
//尾插
void SLPushBack(SL* ps, SLDataType x)
{
	////温柔的解决方式
	//if (ps == NULL)
	//{
	//	return;
	//}
	assert(ps); //等价与assert(ps != NULL)

	//ps->arr[ps->size] = x;
	//++ps->size;
	SLCheckCapacity(ps);
	ps->arr[ps->size++] = x;
}
//头插
void SLPushFront(SL* ps, SLDataType x)
{
	assert(ps);
	SLCheckCapacity(ps);
	//先让顺序表中已有的数据整体往后挪动一位
	for (int i = ps->size; i > 0; i--)
	{
		ps->arr[i] = ps->arr[i - 1];//arr[1] = arr[0]
	}
	ps->arr[0] = x;
	ps->size++;
}

void SLPrint(SL s)
{
	for (int i = 0; i < s.size; i++)
	{
		printf("%d ", s.arr[i]);
	}
	printf("\n");
}
void SLPopBack(SL* ps)
{
	assert(ps);
	assert(ps->size);
	//顺序表不为空
	//ps->arr[ps->size - 1] = -1;
	--ps->size;
}
void SLPopFront(SL* ps)
{
	assert(ps);
	assert(ps->size);

	//数据整体往前挪动一位
	for (int i = 0; i < ps->size - 1; i++)
	{
		ps->arr[i] = ps->arr[i + 1]; //arr[size-2] = arr[size-1]
	}
	ps->size--;
}


int main()
{
	return 0;
}