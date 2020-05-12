//使用规范
//B_vector* vector = B_vectorCreat(sizeof(T));
//T为所要存储的数据类型
//只能以上面这种方式创建
//下面的创建方式会出现问题
//B_vecor vector；
#pragma once
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define B_Vector_DEFAULT_CAPACITY  3
typedef  int Rank;
typedef struct {//B_vector的基本数据
	size_t _esize;//单元素大小
	size_t _size;//元素数量
	size_t _capicity;//容量
	char* _elem;//存储元素的位置
} B_vector;
//public
B_vector* B_vectorCreat(size_t esize);//创建B_vector,成功返回指针，失败返回NULL;
int B_vectorInsert(B_vector* vector, const void* ve, Rank r);//将新元素作为秩为r元素插入，成功返回位置，失败返回-1；
int B_vectorPushBack(B_vector* vector, const void* ve);//将新元素作为末尾元素插入，成功返回位置，失败返回-1；
int B_vectorRemoveRank(B_vector* vector, Rank r);//删除秩为r的元素，成功返回1，失败返回0；
int B_vectorRemoveInteral(B_vector* vector, Rank lo, Rank hi);//删除区间[lo,hi),成功返回删除元素个数,异常则返回-1
char* B_vectorGet(B_vector* vector, Rank r);//得到指定位置的元素，支持对元素修改
void B_vectorClear(B_vector* vector);//清空数据
//private(不推荐调用）
int B_vectorExpand(B_vector* vector);//有必要时扩容，返回值1表示运行正常（无需扩容或已成功扩容），0表示需要扩容但扩容失败
int B_vectorShrink(B_vector* vector);//有必要时缩容，返回值1表示运行正常（无需缩容或已成功缩容），0表示需要缩容但缩容失败
void B_vectorCpyRank(B_vector* vector, char* _Dst, Rank _Dstr, char* _Src, Rank _Srcr);//将_Src的元素拷贝到_Dst上
void B_vectorSort(B_vector* vector, int (*cmp)(const void *,const void *));//排序
